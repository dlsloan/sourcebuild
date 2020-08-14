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

#ifndef __SourceProject_h
#define __SourceProject_h

#include "Source.h"
#include "SourceHeader.h"

#include "Base/Dictionary.h"
#include "Base/Queue.h"
#include "FileSystem/Path.h"

#include <map>
#include <set>
#include <string>
#include <memory>
#include <queue>
#include <sys/stat.h>

class Source;
class SourceHeader;

class SourceProject {
public:
  SourceProject(FileSystem::Path fileMain);
  SourceProject(const SourceProject&);  
  SourceProject& operator=(const SourceProject&); 
  ~SourceProject();

  void build(Base::String buildArgs);
  void clean();
  Base::Dictionary<FileSystem::Path, Source> const& srcDeps() const { return sources_; }
  Base::Dictionary<Base::String, SourceHeader> const& hdrDeps() const { return headers_; }
  Base::Dictionary<FileSystem::Path, GitRepo> const& repos() const { return repos_; }
  Source const& main() const { return main_; }
  FileSystem::Path targetFile() {
    FileSystem::Path targ = fileMain_.trimExt();
#ifdef _MSC_VER
    targ += ".exe";
#endif
    return targ;
  }

private:
  Source main_;
  FileSystem::Path fileMain_;
  Base::Dictionary<FileSystem::Path, Source> sources_;
  Base::Dictionary<Base::String, SourceHeader> headers_;
  Base::Dictionary<FileSystem::Path, GitRepo> repos_;

  void buildObj(Source src, Base::String buildArgs);
  void scanDeps(Source const &src, Base::Queue<Source>& c2Parse, Base::Queue<SourceHeader>& h2Parse);
  void scanDeps(SourceHeader const &hdr, Base::Queue<Source>& c2Parse, Base::Queue<SourceHeader>& h2Parse);
};

#endif