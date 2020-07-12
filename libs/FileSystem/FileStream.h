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

#ifndef __FileStream_h
#define __FileStream_h

#include <stdint.h>

#include "IO/ReadStream.h"
#include "IO/WriteStream.h"
#include "FileSystem/Path.h"

#ifdef _MSC_VER
#define NOMINMAX
#include <windows.h>
#endif

namespace FileSystem
{
  enum OpenMode {
    Read, Write, ReadWrite, None
  };

  class FileStream : public IO::ReadStream, public IO::WriteStream
  {
  public:
    /////////////////////////////////////////////////////////////////////////
    // File Stream constructor
    //   creates a new file stream and either creates or opens a file based
    //     on the mode set by the mode flags
    // fileName: file path of file to create or open
    // mode: mode to open stream in (see Read/Write flags above)
    FileStream(Path const& fileName, OpenMode mode);

    virtual ~FileStream();

    FileStream(FileStream const&) = delete;
    FileStream& operator = (FileStream const&) = delete;

    /////////////////////////////////////////////////////////////////////////
    // Read data from file
    // buffer: byte array to fill with file data
    // length: number of bytes to attempt to read
    // returns number of bytes read
    size_t read(char* buffer, size_t length) override;

    /////////////////////////////////////////////////////////////////////////
    // Write data to file
    // buffer: byte array to write to file
    // length: number of bytes to write
    void write(const char* buffer, size_t length) override;

    /////////////////////////////////////////////////////////////////////////
    // Get current position from start of file for next read/write
    // returns: current file position in bytes
    off_t position() const override;

    /////////////////////////////////////////////////////////////////////////
    // Set current position from start of file for next read/write
    // position: next file position in bytes
    void position(off_t position) override;

    /////////////////////////////////////////////////////////////////////////
    // Get length of file
    // returns: length of file in bytes
    size_t length() const override;

    /////////////////////////////////////////////////////////////////////////
    // Set length of file
    // length: length of file in bytes
    void length(size_t length) override;

    /////////////////////////////////////////////////////////////////////////
    // Get Read capability of file
    // returns: true if file was opened with read access
    bool canRead() const override { return (mode_ == Read || mode_ == ReadWrite) != 0; }

    /////////////////////////////////////////////////////////////////////////
    // Get Write capability of file
    // returns: true if file was opened with write access
    bool canWrite() const override { return (mode_ == Write || mode_ == ReadWrite) != 0; }

    /////////////////////////////////////////////////////////////////////////
    // Get seek capability of file
    // returns: true, file streams can always write stream position
    bool canSeek() const override { return true; }

    /////////////////////////////////////////////////////////////////////////
    // Get resize capability of file
    // returns: true if stream is writable
    bool canResize() const override { return canWrite(); }

    /////////////////////////////////////////////////////////////////////////
    // Is the file open and ahndle held by this object
    // returns: true if file handle is valid
    bool isOpen() const override;

    /////////////////////////////////////////////////////////////////////////
    // Flush file buffer and close file
    void close() override;
  private:
#ifdef _MSC_VER
    HANDLE handle_;
#else
    int handle_;
#endif
    OpenMode mode_;
    size_t length_;
  };
}

#endif
