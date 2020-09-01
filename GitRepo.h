#ifndef __GitRepo_h
#define __GitRepo_h

#include "Base/String.h"
#include "FileSystem/Path.h"

class GitRepo {
  public:
    GitRepo(const Base::String& url) :
      url_(url)
    {
      Base::String str_path =  url_.split("/")[-1];
      if (str_path.endsWith(".git"))
        str_path = str_path.substring(0, str_path.length() - 4);
      if (str_path.startsWith("lib_"))
        str_path = str_path.substring(4);
      path_ = "libs" / FileSystem::Path(str_path);
    }

    void clone() {
      errno = ENOMSG;
      neg_except(int, system, ("git clone " + url_ + " " + path_.toString()).c_str());
      errno = 0;
    }

    void pull() {
      errno = ENOMSG;
      neg_except(int, system, ("git -C " + path_.toString() + " pull").c_str());
      errno = 0;
    }

    const Base::String& url() const { return url_; }
    const FileSystem::Path& path() const { return path_; }

  private:
    Base::String url_;
    FileSystem::Path path_;
};

#endif