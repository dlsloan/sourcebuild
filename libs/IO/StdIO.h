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

#ifndef __IO_StdIO_h
#define __IO_StdIO_h

#include "IO/TextStream.h"

#include <stdint.h>

//TODO: input, output, and error streams will be bidirectional for proc calls, etc

namespace IO
{
  class StandardInput : public IO::TextStream
  {
    public:
      StandardInput();
  };
  class StandardOutput : public IO::TextStream
  {
    public:
      StandardOutput();
  };
  class StandardError : public IO::TextStream
  {
    public:
      StandardError();
  };
}

#endif
