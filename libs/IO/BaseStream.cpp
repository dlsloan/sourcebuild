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

#include "IO/BaseStream.h"
#include "IO/StreamException.h"

using namespace IO;

void BaseStream::position(off_t position)
{
  throw StreamException(Unsupported);
}

void BaseStream::length(size_t len)
{
  throw StreamException(Unsupported);
}

void BaseStream::assertIsOpen() const
{
  if (!this->isOpen())
  throw StreamException(Closed);
}

bool BaseStream::endOfStream() const
{
  auto pos = this->position();
  auto len = this->length();
  return pos >= (ssize_t)len;
}
