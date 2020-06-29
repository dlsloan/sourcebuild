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

#include "IO/TextStream.h"

#include <assert.h>
#include <stdint.h>
#include <sstream>
#include <string>

using namespace IO;
using namespace Base;

class __EOL {
  public:
    static std::string genEOL() {
      std::stringstream ss;
      ss << std::endl;
      return ss.str();
    }

    __EOL() : value(genEOL()) {}
    std::string const value;
};

static __EOL const _eol;

TextStream::TextStream(BaseStream& stream) :
  stream_(&stream)
{
}

String TextStream::readLine() {
  String ret("");
  assert(canRead());
  ReadStream* s = dynamic_cast<ReadStream*>(stream_);
  while(!s->endOfStream())
  {
    char ch = s->readByte();
    if (ch == '\r') continue;
    else if (ch == '\n') break;
    ret += ch;
  }
  return ret;
}

void TextStream::writeLine(String const& line) {
  WriteStream* s = dynamic_cast<WriteStream*>(stream_);
  s->write(line.c_str(), line.length());
  s->write(_eol.value.c_str(), _eol.value.length());
}

void TextStream::write(String const& line) {
  WriteStream* s = dynamic_cast<WriteStream*>(stream_);
  s->write(line.c_str(), line.length());
}
