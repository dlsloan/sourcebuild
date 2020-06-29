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

#include <iostream>

#include "parsing_helpers.h"

using namespace std;

SourceProject::SourceProject(std::string fileMain) :
  fileMain_(fileMain)
{
  queue<SourceHeader*> h2Parse;
  queue<Source*> c2Parse;
  Source* mainSource = new Source(fileMain);
  c2Parse.push(mainSource);
  sources_[fileMain] = unique_ptr<Source>(mainSource);
  while(!c2Parse.empty() || !h2Parse.empty())
  {
    while(!c2Parse.empty())
    {
      auto src = c2Parse.front();
      c2Parse.pop();
      for (auto itt = src->headers_.begin(); itt != src->headers_.end(); ++itt)
      {
        tryAddHeader(*itt, h2Parse, c2Parse);
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
    addDeps(&itt->second->dependancies_, &itt->second->headers_);
  }
}

void SourceProject::clean()
{
  string targetFile = trimExtension(fileMain_);
  if (fileExists(targetFile))
    system(("rm " + targetFile).c_str());
  for (auto itt = sources_.begin(); itt != sources_.end(); ++itt)
  {
    if (fileExists(itt->second->getObjFile()))
      system(("rm " + itt->second->getObjFile()).c_str());
  }
}

void SourceProject::build(string gccArgs)
{
  string targetFile = trimExtension(fileMain_);
  string objs = "";
  bool build = false;
  time_t targTime;
  if (!getFileTime(targetFile, targTime)) build = true;

  for (auto itt = sources_.begin(); itt != sources_.end(); ++itt)
  {
    buildObj(itt->second.get(), gccArgs);
    time_t objTime;
    itt->second->getObjTime(objTime);
    if (difftime(objTime, targTime) > 0) build = true;
    if (itt != sources_.begin())
      objs += " ";
    objs += "\"" + itt->second->getObjFile() + "\"";
  }

  if (build)
  {
    cout << "building: " << targetFile << endl;
    int rv = system(("g++ " + gccArgs + " -o \"" + targetFile + "\" " + objs).c_str());
    if (rv != 0)
    {
      cout << "build failed" << endl;
      exit(1);
    }
  }
}

void SourceProject::buildObj(Source* src, string gccArgs)
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

  for (auto itt = src->headers_.begin(); itt != src->headers_.end() && !build; ++itt)
  {
    time_t hdrTime;
    headers_[*itt]->getHTime(hdrTime);
    if (difftime(hdrTime, objTime) > 0) build = true;
  }

  if (build)
  {
    cout << "building: " << src->getObjFile() << endl;
    system(("mkdir -p " + src->getObjPath()).c_str());
    int rv = system(("g++ " + gccArgs + " -c \"" + src->getSrcFile() + "\" -o \"" + src->getObjFile() + "\" -I\"./\" -I\"./libs/\"").c_str());
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

void SourceProject::addDeps(std::set<std::string>* deps, std::set<std::string>* headers)
{
  for (auto itt = headers->begin(); itt != headers->end(); ++itt)
  {
    if (deps->find(*itt) != deps->end()) continue;
    deps->insert(*itt);
    addDeps(deps, &headers_[*itt]->headers_);
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
      Source* source = new Source(sourceName);
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