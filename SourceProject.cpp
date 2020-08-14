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
#include "Source.h"

#include <iostream>

#include "parsing_helpers.h"

using namespace std;
using namespace Base;
using namespace FileSystem;

SourceProject::SourceProject(Path fileMain) :
  fileMain_(fileMain)
{
  Queue<SourceHeader> h2Parse;
  Queue<Source> c2Parse;
  Source mainSource(fileMain.toString());
  main_ = mainSource;
  for (auto iter = mainSource.repos().iter(); iter.valid(); iter.next()) {
    if (repos_.containsKey(iter.value().path()))
      continue;
    repos_.add(iter.value().path(), iter.value());
    if (!iter.value().path().dirExists())
      iter.value().clone();
  }
  scanDeps(mainSource, c2Parse, h2Parse);
  while(c2Parse.count() > 0 || h2Parse.count() > 0)
  {
    while(c2Parse.count() > 0)
      scanDeps(c2Parse.dequeue(), c2Parse, h2Parse);
    while(h2Parse.count() > 0)
      scanDeps(h2Parse.dequeue(), c2Parse, h2Parse);
  }
}


void SourceProject::scanDeps(Source const &src, Queue<Source>& c2Parse, Queue<SourceHeader>& h2Parse)
{
  for (auto iter = src.repos().iter(); iter.valid(); iter.next()) {
    if (repos_.containsKey(iter.value().path()))
      continue;
    repos_.add(iter.value().path(), iter.value());
    if (!iter.value().path().dirExists())
      iter.value().clone();
  }
  for (auto iter = src.hdrDeps().iter(); iter.valid(); iter.next()) {
    if (headers_.containsKey(iter.value().key))
      continue;
    SourceHeader hdrInner(iter.value().key);
    for (auto iter = hdrInner.repos().iter(); iter.valid(); iter.next()) {
      if (repos_.containsKey(iter.value().path()))
        continue;
      repos_.add(iter.value().path(), iter.value());
      if (!iter.value().path().dirExists())
        iter.value().clone();
    }
    h2Parse.enqueue(hdrInner);
    headers_.add(iter.value().key, hdrInner);
  }
}

void SourceProject::scanDeps(SourceHeader const &hdr, Queue<Source>& c2Parse, Queue<SourceHeader>& h2Parse)
{
  if (hdr.src().fileExists() && !sources_.containsKey(hdr.src()) && hdr.src() != fileMain_) {
    Source srcInner(hdr.src());
    for (auto iter = srcInner.repos().iter(); iter.valid(); iter.next()) {
      if (repos_.containsKey(iter.value().path()))
        continue;
      repos_.add(iter.value().path(), iter.value());
      if (!iter.value().path().dirExists())
        iter.value().clone();
    }
    c2Parse.enqueue(srcInner);
    sources_.add(hdr.src(), srcInner);
  }
  for (auto iter = hdr.hdrDeps().iter(); iter.valid(); iter.next())
  {
    if (headers_.containsKey(iter.value().key))
      continue;
    SourceHeader hdrInner(iter.value().key);
    for (auto iter = hdrInner.repos().iter(); iter.valid(); iter.next()) {
      if (repos_.containsKey(iter.value().path()))
        continue;
      repos_.add(iter.value().path(), iter.value());
      if (!iter.value().path().dirExists())
        iter.value().clone();
    }
    h2Parse.enqueue(hdrInner);
    headers_.add(iter.value().key, hdrInner);
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
    auto src = sources_[keys[i]];
    targetFile = src.objFile();
    if (targetFile.fileExists())
      targetFile.remove();
  }
}

void SourceProject::build(String buildArgs)
{
  Path targetFile = this->targetFile();
  String objs = "";
  bool build = false;
  time_t targTime;
  time_t srcTime;
  if (!getFileTime(targetFile.toString().c_str(), targTime)) build = true;
  main_.cppTime(srcTime);
  if (difftime(srcTime, targTime) > 0) {
    buildObj(main_, buildArgs);
    build = true;
  }
  objs = "\"" + main_.objFile().toString() + "\"";

  auto keys = sources_.keys();
  for (off_t i = 0; i < (ssize_t)keys.count(); i++)
  {
    auto src = sources_[keys[i]];
    buildObj(src, buildArgs);
    time_t objTime;
    src.objTime(objTime);
    if (difftime(objTime, targTime) > 0) build = true;
    objs += " \"" + src.objFile().toString() + "\"";
  }

  if (build)
  {
    cout << "building: " << targetFile.toString().c_str() << endl;
    String cmd;
#ifdef _MSC_VER
    cmd = "cl " + buildArgs + " " + objs + " /Fe\"" + targetFile.toString() + "\"";
#else
    cmd = "g++ " + buildArgs + " -o \"" + targetFile.toString() + "\" " + objs;
#endif
    cout << cmd.c_str() << endl;
    int rv = system(cmd.c_str());
    if (rv != 0)
    {
      cout << "build failed" << endl;
      exit(1);
    }
  }
}

void SourceProject::buildObj(Source src, String buildArgs)
{
  bool build = false;
  time_t objTime;
  if (!src.objTime(objTime))
    build = true;
  else
  {
    time_t srcTime;
    src.cppTime(srcTime);
    if (difftime(srcTime, objTime) > 0) build = true;
  }

  auto keys = src.headers_.keys();
  for (int i = 0; i < (ssize_t)keys.count() && !build; i++)
  {
    time_t hdrTime;
    headers_[keys[i]].getHTime(hdrTime);
    if (difftime(hdrTime, objTime) > 0) build = true;
  }

  if (build)
  {
    cout << "building: " << src.objFile().toString().c_str() << endl;
    Path objDir = src.objPath();
    objDir.createDir();
#ifdef _MSC_VER
    int rv = system(("cl " + buildArgs + " /c \"" + src.srcFile().toString() + "\" /Fo\"" + src.objFile().toString() + "\" /I \"./\" /I \"./libs/\"").c_str());
#else
    int rv = system(("g++ " + buildArgs + " -c \"" + src.srcFile().toString() + "\" -o \"" + src.objFile().toString() + "\" -I\"./\" -I\"./libs/\"").c_str());
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
