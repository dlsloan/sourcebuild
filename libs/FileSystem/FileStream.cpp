/* Copyright (C) 2020 David Sloan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#include "FileSystem/FileStream.h"
#include "IO/StreamException.h"

#include <sys/stat.h>

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

using namespace IO;
using namespace FileSystem;

using namespace std;

FileStream::FileStream(Path const& fileName, OpenMode mode) :
  handle_(-1),
  mode_(None),
  length_(0)
{
  if (mode == Read)
    this->handle_ = open(fileName.toString().c_str(), O_RDONLY);
  else if (mode == Write)
    this->handle_ = open(fileName.toString().c_str(), O_WRONLY | O_CREAT);
  else if (mode == ReadWrite)
    this->handle_ = open(fileName.toString().c_str(), O_RDWR);

  if (this->handle_ != -1)
  {
    this->mode_ = mode;
    if (this->canRead())
    {
      struct stat st;
      assert(stat(fileName.toString().c_str(), &st) == 0);
      this->length_ = st.st_size;
    }
  }
}

size_t FileStream::read(char* buffer, size_t length)
{
  this->assertIsOpen();
  if (!this->canRead()) throw StreamException(Unsupported);
  if (this->endOfStream() && length > 0) throw StreamException(StreamError::endOfStream);
  return ::read(handle_, buffer, length);
}

void FileStream::write(const char* buffer, size_t length)
{
  this->assertIsOpen();
  if (!this->canWrite()) throw StreamException(Unsupported);
  ::write(handle_, buffer, length);
  auto pos = this->position();
  if (pos > (ssize_t)this->length_)
    this->length_ = pos;
}

off_t FileStream::position() const
{
  this->assertIsOpen();
  return lseek(handle_, 0, SEEK_CUR);
}

void FileStream::position(off_t position)
{
  this->assertIsOpen();
  lseek(handle_, position, SEEK_SET);
}

size_t FileStream::length() const
{
  this->assertIsOpen();
  return this->length_;
}

void FileStream::length(size_t length)
{
  this->assertIsOpen();
  if (ftruncate(handle_, length) != 0)
  {
    struct stat st;
    fstat(handle_, &st);
    this->length_ = st.st_size;
    throw StreamException(StreamError::WriteError);
  }
  length_ = length;
}

bool FileStream::isOpen() const
{
  return this->handle_ != -1;
}

void FileStream::close()
{
  if (this->isOpen())
    ::close(this->handle_);
  this->handle_ = -1;
  this->length_ = 0;
  this->mode_ = None;
}

FileStream::~FileStream()
{
  this->close();
}
