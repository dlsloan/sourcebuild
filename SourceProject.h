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

#include <map>
#include <set>
#include <string>
#include <memory>
#include <queue>
#include <sys/stat.h>

#include "Source.h"
#include "SourceHeader.h"

class Source;
class SourceHeader;

class SourceProject {
public:
  SourceProject(std::string fileMain);
  SourceProject(const SourceProject&);  
  SourceProject& operator=(const SourceProject&); 
  ~SourceProject();

  void build(std::string buildArgs);
  void clean();

private:
  std::string fileMain_;
  std::map<std::string, std::unique_ptr<Source>> sources_;
  std::map<std::string, std::unique_ptr<SourceHeader>> headers_;

  bool tryAddHeader(std::string name, std::queue<SourceHeader*>& h2Parse, std::queue<Source*>& c2Parse);

  void addDeps(std::set<std::string>* deps, std::set<std::string>* headers);

  void buildObj(Source* src, std::string buildArgs);

  bool containsHeader(std::string name);
  bool containsSource(std::string name);
};

#endif