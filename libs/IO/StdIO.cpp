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

#include "IO/StdIO.h"

#include <iostream>

using namespace IO;

class __stdInputReader : public ReadStream {
  public:
    virtual ~__stdInputReader() {}

    off_t position() const override { return 0; }
    size_t length() const override { return 1; }

    size_t read(char* buffer, size_t length) override
    {
      std::cin.read(buffer, length);
      return length;
    }
};

class __stdOutputWriter : public WriteStream {
  public:
    off_t position() const override { return 0; }
    size_t length() const override { return 0; }

    void write(char const* buffer, size_t length) override
    {
      std::cout.write(buffer, length);
    }
};

class __stdErrorWriter : public WriteStream {
  public:
    off_t position() const override { return 0; }
    size_t length() const override { return 0; }

    void write(char const* buffer, size_t length) override
    {
      std::cerr.write(buffer, length);
    }
};

static __stdInputReader _stdInput;
static __stdOutputWriter _stdOutput;
static __stdErrorWriter _stdError;

StandardInput::StandardInput() :
  TextStream(_stdInput)
{
}

StandardOutput::StandardOutput() :
  TextStream(_stdOutput)
{
}

StandardError::StandardError() :
  TextStream(_stdError)
{
}
