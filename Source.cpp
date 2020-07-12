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

#include "Source.h"

#include <fstream>
#include <iostream>

#include "Base/String.h"
#include "FileSystem/Path.h"

#include "parsing_helpers.h"

using namespace std;
using namespace FileSystem;
using namespace Base;

static void splitArgs(vector<string>& args, string line)
{
  line = trim(line);
  while(line.length() > 0)
  {
    int index = indexOf(line, ' ');
    string val;
    if (index == -1) val = line;
    else val = line.substr(0, index);
    args.push_back(val);
    line = trim(line.substr(val.length()));
  }
}

Source::Source(string filename) :
  filename_(filename),
  objPath_(""),
  objFile_("")
{
  cout << "scanning: " << filename << endl;
  if (!fileExists(filename))
    filename = "libs/" + filename;
  if (!fileExists(filename))
  {
    cout << "ERROR: could not find file \"" << filename << endl;
    assert(fileExists(filename));
  }
  ifstream stream(filename, ifstream::in);
  while (!stream.eof())
  {
    string line;
    getline(stream, line);
    line = trim(line);
    if (isQuoteInclude(line))
    {
      string file = getIncludeFile(line);
      if (headers_.find(file) != headers_.end())
        continue;
      headers_.insert(file);
    }
    else if (startsWith(line, "#pragma"))
    {
      line = trim(line.substr(7, line.length() - 7));
      if (startsWith(line, "git"))
      {
        line = trim(line.substr(3, line.length() - 3));
        vector<string> args;
        splitArgs(args, line);
        String gitRepo(args[0].c_str());
	String branch = "master";
	if (args.size() > 1)
		branch = args[1].c_str();

	auto parts = gitRepo.split("/");
	String localDir = parts[parts.count() - 1];
	if (localDir.startsWith("lib_"))
		localDir = localDir.substring(4);
	if (localDir.endsWith(".git"))
		localDir = localDir.substring(0, localDir.length() - 4);

	Path libPath("libs");
	if (!libPath.dirExists())
	  libPath.createDir();
	
	Path gitPath = libPath / localDir;
	//TODO: This needs to be multi-pass for library deps
	int rv;
	if (!gitPath.dirExists()) {
          rv = system(("git clone " + gitRepo + " " + gitPath.toString().c_str()).c_str());
	  if (rv != 0) {
	    cout << "build failed" << endl;
            exit(1);
	  }
	  rv = system((String("git -C ") + gitPath.toString() + " checkout " + branch).c_str());
	  if (rv != 0) {
	    cout << "build failed" << endl;
            exit(1);
	  }
	}
	rv = system((std::string("git -C ") + gitPath.toString().c_str() + " pull").c_str());
	if (rv != 0) {
	  cout << "build failed" << endl;
	  exit(1);
	}
      }
    }
  }
  int index = lastIndexOf(filename, '/');
  if (index == -1)
  {
    objPath_ = ".obj/";
    objFile_ = objPath_ + filename + 
#ifdef _MSC_VER
      ".obj";
#else
      ".o";
#endif
  }
  else
  {
    objPath_ = filename.substr(0, index + 1) + ".obj/";
    objFile_ = objPath_ + filename.substr(index + 1, filename.length() - index - 1) +
#ifdef _MSC_VER
    ".obj";
#else
    ".o";
#endif
  }
}

bool Source::getObjTime(time_t& time)
{
    struct stat f_stat;
    if (stat(getObjFile().c_str(), &f_stat) != 0)
      return false;
    time = f_stat.st_mtime;
    return true;
}

bool Source::getCppTime(time_t& time)
{
    struct stat f_stat;
    if (stat(filename_.c_str(), &f_stat) != 0)
      return false;
    time = f_stat.st_mtime;
    return true;
}
