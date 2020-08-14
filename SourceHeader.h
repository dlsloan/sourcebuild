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

#ifndef __SourceHeader_h
#define __SourceHeader_h

#include "Base/Dictionary.h"
#include "Base/String.h"
#include "FileSystem/Path.h"
#include "GitRepo.h"

#include <sys/stat.h>

class SourceProject;

class SourceHeader {
public:
  SourceHeader() {}
  SourceHeader(Base::String header);

  bool getHTime(time_t& time);

  FileSystem::Path const& src() const { return source_; }
  FileSystem::Path const& file() const { return filename_; }
  Base::Dictionary<Base::String, FileSystem::Path> const& hdrDeps() const { return headers_; }
  Base::List<GitRepo> const& repos() const { return repos_; }

  ~SourceHeader() {}
private:
  friend class SourceProject;

  FileSystem::Path source_;
  FileSystem::Path filename_;
  Base::List<GitRepo> repos_;
  Base::Dictionary<Base::String, FileSystem::Path> headers_;
};

#endif//SourceHeader