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

#ifndef __IO_BaseStream_h
#define __IO_BaseStream_h

#include "Base/adhoc_stdint.h"
#include <stdlib.h>

namespace IO
{
    class BaseStream
    {
    public:
        virtual ~BaseStream() {}

        BaseStream(BaseStream const&) = delete;
        BaseStream& operator = (BaseStream const&) = delete;

        /////////////////////////////////////////////////////////////////////////
        // Get current position of the stream
        //  Must be overridden in non-abstract child classes
        // returns: current stream position in bytes
        virtual off_t position() const = 0;

        /////////////////////////////////////////////////////////////////////////
        // Set stream position
        //  Optional, will through StreamException(Unsupported) if not overridden
        // position: position to read/write next byte in stream
        virtual void position(off_t position);

        /////////////////////////////////////////////////////////////////////////
        // Get length of stream
        //  -this may be an estimate in non-seekable streams
        //  -must be greater than getPosition() if the end of stream has not been 
        //    reached
        // returns: length of stream in bytes
        virtual size_t length() const = 0;

        /////////////////////////////////////////////////////////////////////////
        // Set length of stream
        //  -optional support
        // returns: length of stream in bytes
        virtual void length(size_t);

        /////////////////////////////////////////////////////////////////////////
        // Get Read capability of stream
        //  This must be overridden in child classes supporting read capability
        // returns: true if stream is readable
        virtual bool canRead() const { return false; }

        /////////////////////////////////////////////////////////////////////////
        // Get Write capability of stream
        //  This must be overridden in child classes supporting write capability
        // returns: true if stream is writeable
        virtual bool canWrite() const { return false; }

        /////////////////////////////////////////////////////////////////////////
        // Get Seek capability of stream
        //  This must be overridden in child classes supporting seek capability
        // returns: true if stream position can be set
        virtual bool canSeek() const { return false; }

        /////////////////////////////////////////////////////////////////////////
        // Get Resize capability of stream
        //  This must be overridden in child classes supporting variable length through setLength(...) capability
        // returns: true if stream length can be set
        virtual bool canResize() const { return false; }

        /////////////////////////////////////////////////////////////////////////
        // Get Open state of stream
        //  Optional, incicates whether or required stream resources are avaliable
        //   eg. File handle is open/not-null
        // returns: true if stream is avaliable for use
        virtual bool isOpen() const { return true; }

        /////////////////////////////////////////////////////////////////////////
        // Check open state of stream and throw StreamException(Closed) if false
        void assertIsOpen() const;

        /////////////////////////////////////////////////////////////////////////
        // close stream, cleaning up internal resources
        //  Optional, streams without observable sideefects after cleanup
        //    (ex. flushing file buffers) do not need to implement this 
        //    and will remain open after a call to close()
        //  Must not throw an Exception or invalidate state if called multiple 
        //    times
        virtual void close() {}

        /////////////////////////////////////////////////////////////////////////
        // Has end of stream been reached?
        // returns: has all of the data in a stream been read/written?
        virtual bool endOfStream() const;
    protected:
        BaseStream() {}
    };
}
#endif
