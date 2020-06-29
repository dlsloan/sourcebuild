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

#include "IO/StreamException.h"

using namespace IO;

StreamException::StreamException(StreamError err) :
  err_(err)
{}

StreamError StreamException::getType() const
{
  return this->err_;
}

char const* StreamException::what() const noexcept
{
  switch (this->err_)
  {
  case endOfStream:
    return "Error: End of stream reached";
  case ReadError:
    return "Error: Reading stream";
  case WriteError:
    return "Error: Writing stream";
  case Closed:
    return "Error: Stream is closed";
  case Unsupported:
    return "Error: Operation unsupported";
  default:
    return "Error: Unknown";
  }
}
