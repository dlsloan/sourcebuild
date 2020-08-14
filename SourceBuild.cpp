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

#include <string>

#include "Base/String.h"
#include "FileSystem/Path.h"
#include "FileSystem/FileStream.h"
#include "IO/StdIO.h"

#include "SourceProject.h"
#include "Source.h"
#include "SourceHeader.h"

using namespace std;
using namespace Base;
using namespace FileSystem;
using namespace IO;

#ifdef _MSC_VER
const String buildArgs = "/EHsc /std:c++14 /W3";
const String dbgBuildArgs = "/EHsc /Zi /std:c++14 /W3";
#else
const String buildArgs = "-std=c++14 -Wall -Wno-unknown-pragmas";
const String dbgBuildArgs = "-g -std=c++14 -Wall -Wno-unknown-pragmas";
#endif

void printDeps(const Source& source, Dictionary<Path, bool>& touched);
void printDeps(const SourceHeader& source, Dictionary<Path, bool>& touched);

int main(int argc, char** argv)
{
  StandardOutput sout;
  String targetSourceStr(argc >= 2 ? argv[1] : "");
  bool helpRqu = targetSourceStr == "-h" || targetSourceStr == "--help" || targetSourceStr == "?";
  Path targetSource(targetSourceStr);
  bool valid = targetSource.fileExists();
  if (helpRqu || !valid)
  {
    if (!helpRqu)
    {
      sout.writeLine(String("source file: ") + targetSource.toString() + " not found");
    }
    sout.writeLine("usage: {main_source}.cpp [(*)release|debug|clear|run]");
    return 0;
  }
  SourceProject project(targetSource);
  if (argc > 2 && string(argv[2]) == "deps") {
    sout.writeLine("Src: " + project.main().srcFile().toString());
    for (auto iter = project.srcDeps().iter(); iter.valid(); iter.next())
      sout.writeLine("Src: " + iter.value().key.toString());
    for (auto iter = project.hdrDeps().iter(); iter.valid(); iter.next())
      sout.writeLine("Hdr: " + iter.value().value.file().toString());
    for (auto iter = project.repos().iter(); iter.valid(); iter.next())
      sout.writeLine("Repo: " + iter.value().key.toString() + " " + iter.value().value.url());
  } else if (argc > 2 && string(argv[2]) == "debug") {
    project.build(dbgBuildArgs);
  }
  else if (argc > 2 && string(argv[2]) == "clean")
  {
    project.clean();
  }
  else
  {
    project.build(buildArgs);
    if (argc > 2 && string(argv[2]) == "run") {
      String cmd = "./" + project.targetFile().toString();
      sout.writeLine(cmd);
      system(cmd.c_str());
    }
  }
  return 0;
}

void printDeps(const Source& source, Dictionary<Path, bool>& touched)
{
  
}

void printDeps(const SourceHeader& source, Dictionary<Path, bool>& touched)
{
  
}

