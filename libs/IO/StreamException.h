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

#ifndef __StreamException_h
#define __StreamException_h

#include <exception>

namespace IO
{
  /////////////////////////////////////////////////////////////////////////
  // Stream error type
  // NoError: no error was found
  // endOfStream: read was attampted past the end of stream
  // WriteError: error encounterd on stream write
  // ReadError: error encounterd on stream read
  // Closed: stream operation attempted after stream was closed
  // Unsupported: stream does not support requested action 
  //   (ex. write(...) on read-only flie)
  enum StreamError
  {
    NoError, endOfStream, ReadError, WriteError, Closed, Unsupported
  };

  class StreamException : public std::exception
  {
  public:
    /////////////////////////////////////////////////////////////////////////
    // StreamException constructor
    // err: StreamException type (see above)
    StreamException(StreamError err);

    /////////////////////////////////////////////////////////////////////////
    // get stream error type
    // returns: stream error passed to constructor
    StreamError getType() const;

    /////////////////////////////////////////////////////////////////////////
    // Exception message string
    // returns: message string
    virtual char const* what() const noexcept override;
  private:
    StreamError err_;
  };
}

#endif
