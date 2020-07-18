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

#ifndef __Path_h
#define __Path_h

#include "Base/List.h"
#include "Base/String.h"
#include "Base/Exception.h"

namespace FileSystem
{
  class PathException : public Base::Exception {
    public:
      PathException(char const* msg) :
        Base::Exception(msg) {}

      PathException(Base::String const& msg) :
        Base::Exception(msg) {}
  };

  class Path : Base::Stringable {
    public:
      Path();
      Path(Base::String const& path);
      Path(Path const& path);

      Path& operator+= (Path const& value);
      Path& operator+= (Base::String const& value);

      Path operator+ (Path const& value);
      Path operator+ (Base::String const& value);

      Path operator/ (Path const& value);
      Path operator/ (Base::String const& value);
      friend Path operator/(char const* lhs, Path const& rhs)
      {
	      return Path(lhs) / rhs;
      }

      bool isAbsolute() const;
      size_t count() const;

      Path trimExt() const;

      bool dirExists() const;
      bool fileExists() const;
      void createDir() const;
      void remove() const;

      Path subpath(off_t index, size_t len)
      {
	      return Path(*this, index, len);
      }
      Path subpath(off_t index)
      {
	      return this->subpath(index, this->count() - index);
      }

      Base::String operator[] (const off_t index) const;

      Base::String toString() const override;

    private:
      Base::List<Base::String> parts_;
      bool absolute_;

      Path(Path const& path, off_t index, size_t len);

      void verifyPartName(Base::String& part) const;
  };
}

#endif
