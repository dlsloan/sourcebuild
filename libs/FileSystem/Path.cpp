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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "Base/Char.h"

using namespace Base;
using namespace FileSystem;

Path::Path() :
  parts_(),
  absolute_(false)
  {
    
  }

Path::Path(String path) :
  parts_(),
  absolute_(false)
{
  if (path.contains("\\"))
    path = path.replace("\\", "/");
  auto parts = path.split("/");
  parts_.setSize(parts.getCount());
  if (path.startsWith("/"))
    absolute_ = true;
  for (size_t i = 0; i < parts.getCount(); ++i)
  {
    if (parts[i] == "" || parts[i] == ".")
      continue;
    else if (parts[i] == "..")
    {
      if (parts_.getCount() == 0 && isAbsolute())
        throw PathException("cannot backtrack past root");
      else if (parts_.getCount() == 0 || parts_[parts_.getCount() - 1] == "..")
        parts_ += "..";
      else
        parts_.remove(parts_.getCount() - 1);
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

Base::String Path::toString() const
{
  Base::String ret;
  if (isAbsolute())
    ret += "/";
  for (size_t i = 0; i < parts_.getCount(); ++i)
  {
    if (i != 0)
      ret += "/";
    ret += parts_[i];
  }
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

size_t Path::getCount() const
{
  return parts_.getCount();
}

bool Path::isAbsolute() const
{
  return absolute_;
}

Path& Path::operator+= (Path const& value)
{
  if (value.isAbsolute())
    throw PathException("Cannot append absolute path to end of path");
  for (size_t i = 0; i < value.parts_.getCount(); ++i)
  {
    if (value.parts_[i] == "..")
    {
      if (parts_.getCount() == 0 && isAbsolute())
        throw PathException("cannot backtrack past root");
      else if (parts_.getCount() == 0 || parts_[parts_.getCount() - 1] == "..")
        parts_ += "..";
      else
        parts_.remove(parts_.getCount() - 1);
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
  if (path.parts_.getCount() > 0 && path.parts_[0] != ".." && 
    parts_.getCount() > 0 && parts_[parts_.getCount() - 1] != "..")
  {
    parts_[parts_.getCount() - 1] += path.parts_[0];
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

Base::String Path::operator[] (const off_t index) const
{
  return parts_[index];
}

bool Path::dirExists()
{
    struct stat path_stat;
    if (stat(toString().c_str(), &path_stat) != 0)
      return false;
    return S_ISDIR(path_stat.st_mode);
}

bool Path::fileExists()
{
    struct stat path_stat;
    if (stat(toString().c_str(), &path_stat) != 0)
      return false;
    return S_ISREG(path_stat.st_mode);
}
