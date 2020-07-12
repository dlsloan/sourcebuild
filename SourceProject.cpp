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

#include "SourceProject.h"

#include "Base/String.h"
#include "FileSystem/Path.h"

#include <iostream>

#include "parsing_helpers.h"

using namespace std;
using namespace Base;
using namespace FileSystem;

SourceProject::SourceProject(std::string fileMain) :
  fileMain_(fileMain)
{
  queue<SourceHeader*> h2Parse;
  queue<Source*> c2Parse;
  Source* mainSource = new Source(String(fileMain.c_str()));
  c2Parse.push(mainSource);
  sources_[fileMain] = unique_ptr<Source>(mainSource);
  while(!c2Parse.empty() || !h2Parse.empty())
  {
    while(!c2Parse.empty())
    {
      auto src = c2Parse.front();
      c2Parse.pop();
      auto keys = src->headers_.keys();
      for (off_t i = 0; i < (ssize_t)keys.count(); i++)
      {
        tryAddHeader(std::string(src->headers_[keys[i]].toString().c_str()), h2Parse, c2Parse);
      }
    }
    while(!h2Parse.empty())
    {
      auto hdr = h2Parse.front();
      h2Parse.pop();
      for (auto itt = hdr->headers_.begin(); itt != hdr->headers_.end(); ++itt)
      {
        tryAddHeader(*itt, h2Parse, c2Parse);
      }
    }
  }
  for (auto itt = sources_.begin(); itt != sources_.end(); ++itt)
  {
    //std::set<std::string>* deps = &itt->second->dependancies_;
    addDeps(itt->second->dependancies_, itt->second->headers_);
  }
}

void SourceProject::clean()
{
  string targetFile = trimExtension(fileMain_);
#ifdef _MSC_VER
  targetFile += ".exe";
#endif
  Path targetPath(targetFile.c_str());
  if (targetPath.fileExists())
    targetPath.remove();
  for (auto itt = sources_.begin(); itt != sources_.end(); ++itt)
  {
    targetPath = Path(itt->second->getObjFile().toString().c_str());
    if (targetPath.fileExists())
      targetPath.remove();
  }
}

void SourceProject::build(string buildArgs)
{
  string targetFile = trimExtension(fileMain_);
#ifdef _MSC_VER
  targetFile += ".exe";
#endif
  string objs = "";
  bool build = false;
  time_t targTime;
  if (!getFileTime(targetFile, targTime)) build = true;

  for (auto itt = sources_.begin(); itt != sources_.end(); ++itt)
  {
    buildObj(itt->second.get(), buildArgs);
    time_t objTime;
    itt->second->getObjTime(objTime);
    if (difftime(objTime, targTime) > 0) build = true;
    if (itt != sources_.begin())
      objs += " ";
    objs += "\"" + string(itt->second->getObjFile().toString().c_str()) + "\"";
  }

  if (build)
  {
    cout << "building: " << targetFile << endl;
#ifdef _MSC_VER
    int rv = system(("cl " + buildArgs + " " + objs + " /Fe\"" + targetFile + "\"").c_str());
#else
    int rv = system(("g++ " + buildArgs + " -o \"" + targetFile + "\" " + objs).c_str());
#endif
    if (rv != 0)
    {
      cout << "build failed" << endl;
      exit(1);
    }
  }
}

void SourceProject::buildObj(Source* src, string buildArgs)
{
  bool build = false;
  time_t objTime;
  if (!src->getObjTime(objTime))
    build = true;
  else
  {
    time_t srcTime;
    src->getCppTime(srcTime);
    if (difftime(srcTime, objTime) > 0) build = true;
  }

  auto keys = src->headers_.keys();
  for (int i = 0; i < (ssize_t)keys.count() && !build; i++)
  {
    time_t hdrTime;
    headers_[keys[i].toString().c_str()]->getHTime(hdrTime);
    if (difftime(hdrTime, objTime) > 0) build = true;
  }

  if (build)
  {
    cout << "building: " << src->getObjFile().toString().c_str() << endl;
    Path objDir = src->getObjPath();
    objDir.createDir();
#ifdef _MSC_VER
    int rv = system(("cl " + buildArgs + " /c \"" + src->getSrcFile().toString().c_str() + "\" /Fo\"" + src->getObjFile().toString().c_str() + "\" /I \"./\" /I \"./libs/\"").c_str());
#else
    int rv = system(("g++ " + buildArgs + " -c \"" + src->getSrcFile().toString().c_str() + "\" -o \"" + src->getObjFile().toString().c_str() + "\" -I\"./\" -I\"./libs/\"").c_str());
#endif
    if (rv != 0)
    {
      cout << "build failed" << endl;
      exit(1);
    }
  }
}

SourceProject::~SourceProject()
{

}

Dictionary<String, Path> base_header_list(std::set<std::string> const& headers)
{
  Dictionary<String, Path> hdrs;
  for (auto itt = headers.begin(); itt != headers.end(); ++itt) {
    hdrs.add(String(itt->c_str()), Path(itt->c_str()));
  }
  return hdrs;
}

void SourceProject::addDeps(Base::Dictionary<Base::String, FileSystem::Path>& deps, Base::Dictionary<Base::String, FileSystem::Path> const& headers)
{
  auto keys = headers.keys();
  for (off_t i = 0; i < (ssize_t)keys.count(); i++)
  {
    if (deps.containsKey(keys[i])) continue;
    deps.add(keys[i], headers[keys[i]]);
    addDeps(deps, base_header_list(headers_[keys[i].c_str()]->headers_));
  }
}

bool SourceProject::tryAddHeader(string name, queue<SourceHeader*>& h2Parse, queue<Source*>& c2Parse)
{
  if (!this->containsHeader(name))
  {
    SourceHeader* header = new SourceHeader(name);
    h2Parse.push(header);
    headers_[name] = unique_ptr<SourceHeader>(header);
    string sourceName = trimExtension(name) + ".cpp";
    if (!fileExists(sourceName)) sourceName = "libs/" + sourceName;
    if (fileExists(sourceName) && !this->containsSource(sourceName))
    {
      Source* source = new Source(String(sourceName.c_str()));
      c2Parse.push(source);
      sources_[sourceName] = unique_ptr<Source>(source);
    }
    return true;
  }
  else
  {
    return false;
  }
}

bool SourceProject::containsHeader(string name)
{
  return headers_.find(name) != headers_.end();
}

bool SourceProject::containsSource(string name)
{
  return sources_.find(name) != sources_.end();
}