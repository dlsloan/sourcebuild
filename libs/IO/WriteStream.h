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

#ifndef __WriteStream_h
#define __WriteStream_h

#include "IO/BaseStream.h"

namespace IO
{
  class WriteStream : public virtual BaseStream
  {
  public:
    virtual ~WriteStream() {}

    WriteStream(WriteStream const&) = delete;
    WriteStream& operator = (WriteStream const&) = delete;

    /////////////////////////////////////////////////////////////////////////
    // Write data to stream
    //   This must be overridden in child classes.
    // buffer: byte array to write to stream
    // length: number of bytes to write
    virtual void write(char const* buffer, size_t length) = 0;

    /////////////////////////////////////////////////////////////////////////
    // Write byte to stream
    // value: byte to write to stream
    void writeByte(char value);

    /////////////////////////////////////////////////////////////////////////
    // Get Write capability of stream
    //  override in child class if writeability is non-constant
    // returns: true if not overridden
    virtual bool canWrite() const override { return true; }
  protected:
    WriteStream() {}
  };
}

#endif