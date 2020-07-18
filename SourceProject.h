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
  SourceProject(Base::String fileMain);
  SourceProject(const SourceProject&);  
  SourceProject& operator=(const SourceProject&); 
  ~SourceProject();

  void build(Base::String buildArgs);
  void clean();
  Base::Dictionary<Base::String, std::unique_ptr<Source>> const& srcDeps() const
  {
    return sources_;
  }

  Base::Dictionary<Base::String, std::unique_ptr<SourceHeader>> const& hdrDeps()
  {
    return headers_;
  }

private:
  FileSystem::Path fileMain_;
  Base::Dictionary<Base::String, std::unique_ptr<Source>> sources_;
  Base::Dictionary<Base::String, std::unique_ptr<SourceHeader>> headers_;

  bool tryAddHeader(FileSystem::Path name, Base::Queue<SourceHeader*>& h2Parse, Base::Queue<Source*>& c2Parse);

  void addDeps(Base::Dictionary<Base::String, FileSystem::Path>& deps, Base::Dictionary<Base::String, FileSystem::Path> const& headers);

  void buildObj(Source* src, Base::String buildArgs);

  bool containsHeader(Base::String name);
  bool containsSource(Base::String name);
};

#endif