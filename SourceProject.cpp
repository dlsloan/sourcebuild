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

SourceProject::SourceProject(String fileMain) :
  fileMain_(fileMain)
{
  Queue<SourceHeader*> h2Parse;
  Queue<Source*> c2Parse;
  Source* mainSource = new Source(String(fileMain.c_str()));
  c2Parse.enqueue(mainSource);
  //sources_.add(fileMain, std::move(unique_ptr<Source>(mainSource)));
  sources_[fileMain] = unique_ptr<Source>(mainSource);
  List<String> keys;
  while(c2Parse.count() > 0 || h2Parse.count() > 0)
  {
    while(c2Parse.count() > 0)
    {
      auto src = c2Parse.dequeue();
      auto keys = src->headers_.keys();
      for (off_t i = 0; i < (ssize_t)keys.count(); i++)
      {
        tryAddHeader(src->headers_[keys[i]].toString(), h2Parse, c2Parse);
      }
    }
    while(h2Parse.count() > 0)
    {
      auto hdr = h2Parse.dequeue();
      auto keys = hdr->headers_.keys();
      for (off_t i = 0; i < (ssize_t)keys.count(); i++)
      {
        tryAddHeader(hdr->headers_[keys[i]].toString(), h2Parse, c2Parse);
      }
    }
  }
  keys = sources_.keys();
  for (off_t i = 0; i < (ssize_t)keys.count(); i++)
  {
    auto src = sources_[keys[i]].get();
    //std::set<std::string>* deps = &itt->second->dependancies_;
    addDeps(src->dependancies_, src->headers_);
  }
}

void SourceProject::clean()
{
  Path targetFile = fileMain_.trimExt();
#ifdef _MSC_VER
  targetFile += ".exe";
#endif
  if (targetFile.fileExists())
    targetFile.remove();
  auto keys = sources_.keys();
  for (off_t i = 0; i < (ssize_t)keys.count(); i++)
  {
    auto src = sources_[keys[i]].get();
    targetFile = src->getObjFile();
    if (targetFile.fileExists())
      targetFile.remove();
  }
}

void SourceProject::build(String buildArgs)
{
  Path targetFile = fileMain_.trimExt();
#ifdef _MSC_VER
  targetFile += ".exe";
#endif
  String objs = "";
  bool build = false;
  time_t targTime;
  if (!getFileTime(targetFile.toString().c_str(), targTime)) build = true;

  auto keys = sources_.keys();
  for (off_t i = 0; i < (ssize_t)keys.count(); i++)
  {
    auto src = sources_[keys[i]].get();
    buildObj(src, buildArgs);
    time_t objTime;
    src->getObjTime(objTime);
    if (difftime(objTime, targTime) > 0) build = true;
    if (i != 0)
      objs += " ";
    objs += "\"" + src->getObjFile().toString() + "\"";
  }

  if (build)
  {
    cout << "building: " << targetFile.toString().c_str() << endl;
#ifdef _MSC_VER
    int rv = system(("cl " + buildArgs + " " + objs + " /Fe\"" + targetFile.toString() + "\"").c_str());
#else
    int rv = system(("g++ " + buildArgs + " -o \"" + targetFile.toString() + "\" " + objs).c_str());
#endif
    if (rv != 0)
    {
      cout << "build failed" << endl;
      exit(1);
    }
  }
}

void SourceProject::buildObj(Source* src, String buildArgs)
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
    headers_[keys[i]]->getHTime(hdrTime);
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

void SourceProject::addDeps(Base::Dictionary<Base::String, FileSystem::Path>& deps, Base::Dictionary<Base::String, FileSystem::Path> const& headers)
{
  auto keys = headers.keys();
  for (off_t i = 0; i < (ssize_t)keys.count(); i++)
  {
    if (deps.containsKey(keys[i])) continue;
    deps.add(keys[i], headers[keys[i]]);
    addDeps(deps, headers_[keys[i]]->headers_);
  }
}

bool SourceProject::tryAddHeader(Path name, Queue<SourceHeader*>& h2Parse, Queue<Source*>& c2Parse)
{
  if (!this->containsHeader(name.toString()))
  {
    SourceHeader* header = new SourceHeader(name);
    h2Parse.enqueue(header);
    headers_[name.toString()] = unique_ptr<SourceHeader>(header);
    Path sourceName = name.trimExt() + ".cpp";
    if (!sourceName.fileExists()) sourceName = "libs" / sourceName;
    if (sourceName.fileExists() && !this->containsSource(sourceName.toString()))
    {
      Source* source = new Source(sourceName.toString());
      c2Parse.enqueue(source);
      sources_[sourceName.toString()] = unique_ptr<Source>(source);
    }
    return true;
  }
  else
  {
    return false;
  }
}

bool SourceProject::containsHeader(String name)
{
  return headers_.containsKey(name);
}

bool SourceProject::containsSource(String name)
{
  return sources_.containsKey(name);
}