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

#ifndef __IO_TextStream_h
#define __IO_TextStream_h

#include "IO/BaseStream.h"
#include "IO/ReadStream.h"
#include "IO/WriteStream.h"

#include "base/String.h"

namespace IO {
  class TextStream {
  public:
    TextStream(IO::BaseStream& stream);

    Base::String readLine();

    void writeLine(Base::String const& line);
    void write(Base::String const& line);

    virtual bool endOfStream() {
      return stream_->endOfStream();
    }

    virtual bool canRead() const { return stream_->canRead(); }
    virtual bool canWrite() const { return stream_->canWrite(); }
  private:
    IO::BaseStream* stream_;
  };
}

#endif