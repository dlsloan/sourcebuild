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

#ifndef __Source_h
#define __Source_h

#include <string>
#include <set>
#include <sys/stat.h>

#include "Base/String.h"
#include "FileSystem/Path.h"
#include "Base/Dictionary.h"
#include "SourceProject.h"
#include "SourceHeader.h"

class SourceProject;
class SourceHeader;

class Source {
public:
  Source(const Source&);
  Source& operator=(const Source&); 

  bool getObjTime(time_t& time);
  bool getCppTime(time_t& time);

  FileSystem::Path getObjFile() { return objFile_; }
  
  FileSystem::Path getObjPath() { return objPath_; }

  FileSystem::Path getSrcFile() { return filename_; }

  ~Source() {}
private:
  friend class SourceProject;

  FileSystem::Path filename_;
  FileSystem::Path objPath_;
  FileSystem::Path objFile_;
  Base::Dictionary<Base::String, FileSystem::Path> dependancies_;
  Base::Dictionary<Base::String, FileSystem::Path> headers_;

  Source(FileSystem::Path const& filename);
};

#endif
