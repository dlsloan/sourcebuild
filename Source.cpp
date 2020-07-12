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

#include "parsing_helpers.h"

using namespace std;
using namespace FileSystem;
using namespace Base;

static void splitArgs(List<String>& args, String line)
{
  line = line.trim();
  while(line.length() > 0)
  {
    int index = line.indexOf(" ");
    String val;
    if (index == -1) val = line;
    else val = line.substring(0, index);
    args.add(val);
    line = line.substring(val.length()).trim();
  }
}

Source::Source(Path const& filename_arg) :
  filename_(filename_arg),
  objPath_(""),
  objFile_("")
{
  Path filename = filename_arg;
  cout << "scanning: " << filename.toString().c_str() << endl;
  if (!filename.fileExists())
    filename = "libs" / filename;
  if (!filename.fileExists())
  {
    cout << "ERROR: could not find file \"" << filename.toString().c_str() << endl;
    assert(filename.fileExists());
  }
  ifstream stream(filename.toString().c_str(), ifstream::in);
  while (!stream.eof())
  {
    string line_str;
    getline(stream, line_str);
    String line(line_str.c_str());
    line = line.trim();
    if (isQuoteInclude(line))
    {
      String file = getIncludeFile(line);
      if (headers_.containsKey(file))
        continue;
      headers_.add(file, Path(file));
    }
    else if (line.startsWith("#pragma"))
    {
      line = line.substring(7, line.length() - 7).trim();
      if (line.startsWith("git"))
      {
        line = line.substring(3, line.length() - 3).trim();
        List<String> args;
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
  if (filename.count() == 1)
  {
    objPath_ = Path(".obj");
    objFile_ = objPath_ / (filename + 
#ifdef _MSC_VER
      ".obj");
#else
      ".o");
#endif
  }
  else
  {
    objPath_ = filename.subpath(0, filename.count() - 1) / ".obj/";
    objFile_ = objPath_ / (filename[-1] +
#ifdef _MSC_VER
    ".obj");
#else
    ".o");
#endif
  }
}

bool Source::getObjTime(time_t& time)
{
    struct stat f_stat;
    if (stat(getObjFile().toString().c_str(), &f_stat) != 0)
      return false;
    time = f_stat.st_mtime;
    return true;
}

bool Source::getCppTime(time_t& time)
{
    struct stat f_stat;
    if (stat(filename_.toString().c_str(), &f_stat) != 0)
      return false;
    time = f_stat.st_mtime;
    return true;
}
