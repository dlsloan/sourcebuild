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
    sout.writeLine("usage: {main_source}.cpp [(*)release|debug|clear]");
    return 0;
  }
  SourceProject project(targetSource.toString().c_str());
  String buildArgs;
  if (argc > 2 && string(argv[2]) == "deps") {
    for (auto iter = project.srcDeps().iter(); iter.valid(); iter.next()) {
      sout.writeLine("Src: " + iter.value().key);
    }
    for (auto iter = project.hdrDeps().iter(); iter.valid(); iter.next()) {
      sout.writeLine("Hdr: " + iter.value().key);
    }
  } else if (argc > 2 && string(argv[2]) == "debug") {
#ifdef _MSC_VER
    buildArgs = "/EHsc /Zi /std:c++14 /W3";
#else
    buildArgs = "-g -std=c++14 -Wall -Wno-unknown-pragmas";
#endif
    project.build(buildArgs);
  }
  else if (argc > 2 && string(argv[2]) == "clean")
  {
    project.clean();
  }
  else
  {
#ifdef _MSC_VER
    buildArgs = "/EHsc /std:c++14 /W3";
#else
    buildArgs = "-std=c++14 -Wall -Wno-unknown-pragmas";
#endif
    project.build(buildArgs);
  }
  return 0;
}

