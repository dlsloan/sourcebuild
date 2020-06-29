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

#ifndef __ReadStream_h
#define __ReadStream_h

#include "IO/BaseStream.h"

namespace IO
{
  class ReadStream : public virtual BaseStream
  {
  public:
    virtual ~ReadStream() {}

    ReadStream(ReadStream const&) = delete;
    ReadStream& operator = (ReadStream const&) = delete;

    /////////////////////////////////////////////////////////////////////////
    // Read data from stream
    //   This must be overridden in child classes.
    //   This must always read at least 1 byte if end of stream has not 
    //     been reached
    // buffer: byte array to fill with stream data
    // length: number of bytes to attempt to read
    // returns number of bytes read
    virtual size_t read(char* buffer, size_t length) = 0;

    /////////////////////////////////////////////////////////////////////////
    // Read read a single byte from stream
    // returns: byte from stream, undefined if endOfStream has been reached
    char readByte();

    /////////////////////////////////////////////////////////////////////////
    // Get Read capability of stream
    //  override in child class if readability is non-constant
    // returns: true if not overridden
    virtual bool canRead() const override { return true; }
    
  protected:
    ReadStream() {}
  };
}

#endif
