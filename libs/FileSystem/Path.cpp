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

#include "FileSystem/Path.h"
#include <assert.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "Base/adhoc_unistd.h"
#include "Base/Char.h"

using namespace Base;
using namespace FileSystem;

Path::Path() :
  parts_(),
  absolute_(false)
  {
    
  }

Path::Path(String const& path_arg) :
  parts_(),
  absolute_(false)
{
  String path = path_arg;
  if (path.contains("\\"))
    path = path.replace("\\", "/");
  auto parts = path.split("/");
  parts_.size(parts.count());
  if (path.startsWith("/"))
    absolute_ = true;
  for (size_t i = 0; i < parts.count(); ++i)
  {
    if (parts[i] == "" || parts[i] == ".")
      continue;
    else if (parts[i] == "..")
    {
      if (parts_.count() == 0 && isAbsolute())
        throw PathException("cannot backtrack past root");
      else if (parts_.count() == 0 || parts_[parts_.count() - 1] == "..")
        parts_ += "..";
      else
        parts_.remove(parts_.count() - 1);
    }
    else
    {
      verifyPartName(parts[i]);
      parts_ += parts[i];
    }
  }
}

Path::Path(Path const& path) :
  parts_(path.parts_),
  absolute_(path.absolute_)
{
}

Path::Path(Path const& path, off_t index, size_t len) :
  absolute_(path.absolute_ && index == 0)
{
	parts_ = path.parts_.sublist(index, len);
}

Base::String Path::toString() const
{
  Base::String ret;
#ifdef _MSC_VER
  if (isAbsolute())
    ret += parts_[0] + ":\\";
  for (size_t i = isAbsolute() ? 1 : 0; i < parts_.count(); ++i)
  {
    if (i != (isAbsolute() ? 1 : 0))
      ret += "\\";
    ret += parts_[i];
  }
#else
  if (isAbsolute())
    ret += "/";
  for (size_t i = 0; i < parts_.count(); ++i)
  {
    if (i != 0)
      ret += "/";
    ret += parts_[i];
  }
#endif
  return ret;
}

void Path::verifyPartName(Base::String& part) const
{
  if (part.length() > 2 && part.startsWith(".."))
    throw PathException("Path segment may not start with \"..\"");
  for (size_t i = 0; i < part.length(); ++i)
  {
    if (!Char::isLetterOrDigit(part[i]) && part[i] != '-' && part[i] != '_' && part[i] != '.' &&
      part[i] != '(' && part[i] != ')')
      throw PathException(String("Invalid path character \'") + part[i] + "\'");
  }
}

size_t Path::count() const
{
  return parts_.count();
}

bool Path::isAbsolute() const
{
  return absolute_;
}

Path& Path::operator+= (Path const& value)
{
  if (value.isAbsolute())
    throw PathException("Cannot append absolute path to end of path");
  for (size_t i = 0; i < value.parts_.count(); ++i)
  {
    if (value.parts_[i] == "..")
    {
      if (parts_.count() == 0 && isAbsolute())
        throw PathException("cannot backtrack past root");
      else if (parts_.count() == 0 || parts_[parts_.count() - 1] == "..")
        parts_ += "..";
      else
        parts_.remove(parts_.count() - 1);
    }
    else
    {
      parts_ += value.parts_[i];
    }
  }
  return *this;
}

Path& Path::operator+= (Base::String const& value)
{
  Path path(String("./") + value);
  if (path.parts_.count() > 0 && path.parts_[0] != ".." && 
    parts_.count() > 0 && parts_[parts_.count() - 1] != "..")
  {
    parts_[parts_.count() - 1] += path.parts_[0];
    path.parts_.remove(0);
  }
  *this += path;
  return *this;
}

Path Path::operator+ (Path const& value)
{
  Path ret(*this);
  ret += value;
  return ret;
}

Path Path::operator+ (Base::String const& value)
{
  Path ret(*this);
  ret += value;
  return ret;
}

Path Path::operator/ (Path const& value) {
  Path ret(*this);
  ret += value;
  return ret;
}

Path Path::operator/ (Base::String const& value) {
  Path ret(*this);
  ret += Path(value);
  return ret;
}

Base::String Path::operator[] (const off_t index) const
{
  return parts_[index];
}

bool Path::dirExists() const
{
#ifdef _MSC_VER
  DWORD ret;
  ret = GetFileAttributesA(toString().c_str());
  if (ret == INVALID_FILE_ATTRIBUTES)
    return false;
  return (FILE_ATTRIBUTE_DIRECTORY & ret) == FILE_ATTRIBUTE_DIRECTORY;
#else
  struct stat path_stat;
  if (stat(toString().c_str(), &path_stat) != 0)
    return false;
  return S_ISDIR(path_stat.st_mode);
#endif
}

bool Path::fileExists() const
{
#ifdef _MSC_VER
  DWORD ret;
  ret = GetFileAttributesA(toString().c_str());
  if (ret == INVALID_FILE_ATTRIBUTES)
    return false;
  return (FILE_ATTRIBUTE_DIRECTORY & ret) != FILE_ATTRIBUTE_DIRECTORY;
#else
  struct stat path_stat;
  if (stat(toString().c_str(), &path_stat) != 0)
    return false;
  return S_ISREG(path_stat.st_mode);
#endif
}

void Path::remove() const
{
#ifdef _MSC_VER
  if (fileExists())
    DeleteFileA(toString().c_str());
  else if (dirExists())
    RemoveDirectoryA(toString().c_str());
#else
  ::remove(toString().c_str());
#endif
}

void Path::createDir() const
{
#ifdef _MSC_VER
  if (!dirExists())
    CreateDirectoryA(toString().c_str(), nullptr);
#else
  if (!dirExists())
    mkdir(toString().c_str(), 0700);
#endif
}
