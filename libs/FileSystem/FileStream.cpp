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

#include "Base/adhoc_sizes.h"
#include "Base/adhoc_unistd.h"
#include "FileSystem/FileStream.h"
#include "IO/StreamException.h"

#include <sys/stat.h>

#include <stdio.h>
#include <fcntl.h>
#include <assert.h>

using namespace IO;
using namespace FileSystem;

using namespace std;

FileStream::FileStream(Path const& fileName, OpenMode mode) :
#ifdef _MSC_VER
  handle_(INVALID_HANDLE_VALUE),
#else
  handle_(-1),
#endif
  mode_(None),
  length_(0)
{
#ifdef _MSC_VER
  OFSTRUCT lpReOpenBuff = {0};
  if (mode == Read) {
    this->handle_ = CreateFileA(fileName.toString().c_str(), GENERIC_READ,
      FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  } else if (mode == Write) {
    this->handle_ = CreateFileA(fileName.toString().c_str(), GENERIC_WRITE, 0,
      NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
  } else if (mode == ReadWrite) {
    this->handle_ = CreateFileA(fileName.toString().c_str(),
      GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
      FILE_ATTRIBUTE_NORMAL, NULL);
  }
#else
  if (mode == Read)
    this->handle_ = open(fileName.toString().c_str(), O_RDONLY);
  else if (mode == Write)
    this->handle_ = open(fileName.toString().c_str(), O_WRONLY | O_CREAT);
  else if (mode == ReadWrite)
    this->handle_ = open(fileName.toString().c_str(), O_RDWR);
#endif

#ifdef _MSC_VER
  if (this->handle_ != INVALID_HANDLE_VALUE)
#else
  if (this->handle_ != -1)
#endif
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
  size_t bytesRead = 0;
  off_t pos = this->position();
  this->assertIsOpen();
  if (!this->canRead()) throw StreamException(Unsupported);
  if (this->endOfStream() && length > 0) throw StreamException(StreamError::endOfStream);
  while (!this->endOfStream() && length > 0) {
#ifdef _MSC_VER
    DWORD lenRead;
    size_t toRead = length >= SZ_2G ? SZ_2G - 1 : length;
    if (!ReadFile(handle_, buffer, toRead, &lenRead, nullptr)) {
      this->position(pos);
      throw StreamException(StreamError::ReadError);
    }
    length -= lenRead;
    buffer += lenRead;
    bytesRead += lenRead;
#else
    ssize_t ret;
    ret = ::read(handle_, buffer, length);
    if (ret < 0) {
      this->position(pos);
      throw StreamException(StreamError::ReadError);
    } else if (ret == 0) {
	    break;
    }
    bytesRead += ret;
#endif
  }
  return bytesRead;
}

void FileStream::write(const char* buffer, size_t length)
{
  this->assertIsOpen();
  if (!this->canWrite()) throw StreamException(Unsupported);
#ifdef _MSC_VER
  DWORD lenWritten;
  while (length > 0) {
    if (!WriteFile(handle_, buffer, length, &lenWritten, NULL))
      throw StreamException(StreamError::WriteError);
    auto pos = this->position();
    if (pos > (ssize_t)this->length_)
      this->length_ = pos;
    length -= lenWritten;
    buffer += lenWritten;
  }
#else
  ::write(handle_, buffer, length);
  auto pos = this->position();
  if (pos > (ssize_t)this->length_)
    this->length_ = pos;
#endif
}

off_t FileStream::position() const
{
  this->assertIsOpen();
#ifdef _MSC_VER
  LARGE_INTEGER pos;
  pos.QuadPart = 0;
  pos.LowPart = SetFilePointer(handle_, pos.LowPart, &pos.HighPart,
    FILE_CURRENT);
  return (off_t)pos.QuadPart;
#else
  return lseek(handle_, 0, SEEK_CUR);
#endif
}

void FileStream::position(off_t position)
{
  this->assertIsOpen();
#ifdef _MSC_VER
  LARGE_INTEGER new_pos;
  new_pos.QuadPart = position;
  new_pos.LowPart = SetFilePointer(handle_, new_pos.LowPart, &new_pos.HighPart,
    FILE_BEGIN);
#else
  lseek(handle_, position, SEEK_SET);
#endif
}

size_t FileStream::length() const
{
  this->assertIsOpen();
  return this->length_;
}

void FileStream::length(size_t length)
{
  this->assertIsOpen();
#ifdef _MSC_VER
  LARGE_INTEGER orig_pos;
  orig_pos.QuadPart = 0;

  orig_pos.LowPart = SetFilePointer(handle_, 0, &orig_pos.HighPart, FILE_CURRENT);
  if (orig_pos.LowPart == INVALID_SET_FILE_POINTER)
    throw StreamException(StreamError::WriteError);

  LARGE_INTEGER new_end;
  new_end.QuadPart = length;
  new_end.LowPart = SetFilePointer(handle_, new_end.LowPart, &new_end.HighPart,
    FILE_BEGIN);
  if (new_end.LowPart == INVALID_SET_FILE_POINTER)
    throw StreamException(StreamError::WriteError);

  bool truncated = SetEndOfFile(handle_);
  length_ = (size_t)new_end.QuadPart;
  SetFilePointer(handle_, orig_pos.LowPart, &orig_pos.HighPart, FILE_BEGIN);
  if (!truncated)
    throw StreamException(StreamError::WriteError);
#else
  if (ftruncate(handle_, length) != 0)
  {
    struct stat st;
    fstat(handle_, &st);
    this->length_ = st.st_size;
    throw StreamException(StreamError::WriteError);
  }
#endif
  length_ = length;
}

bool FileStream::isOpen() const
{
#ifdef _MSC_VER
  return this->handle_ != INVALID_HANDLE_VALUE;
#else
  return this->handle_ != -1;
#endif
}

void FileStream::close()
{
#ifdef _MSC_VER
  if (this->isOpen())
    CloseHandle(this->handle_);
  this->handle_ = INVALID_HANDLE_VALUE;
#else
  if (this->isOpen())
    ::close(this->handle_);
  this->handle_ = -1;
#endif
  this->length_ = 0;
  this->mode_ = None;
}

FileStream::~FileStream()
{
  this->close();
}
