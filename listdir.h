#ifndef LISTDIR_H
#define LISTDIR_H
#include <stdio.h>
#include <sys/types.h>
#include<sys/stat.h>
#include <dirent.h>
#include<string>
#include<vector>

typedef std::vector<std::string> svec;

static bool is_dir(const std::string& name)
{
  return name == "word" || name == "_rels" || name == "docProps" || name == "customXml";
}


/*
  based on https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html
*/
extern void get_high_level_content(const std::string& docx_dir, svec& files, svec& dirs)
{
  DIR *dp;
  struct dirent *ep;

  dp = opendir (docx_dir.c_str());
  if (dp != NULL)
    {
      while (ep = readdir (dp))
      {
	std::string dir  = std::string(ep->d_name);
	if(dir != "." && dir != "..")
	  {
	    if(is_dir(dir))
	      dirs.push_back(std::string(ep->d_name));
	    else
	      files.push_back(std::string(ep->d_name));
	  }
      }
      (void) closedir (dp);

      printf("\n");
    }
  else
    {
      printf("failed to open %s directory!\n",docx_dir.c_str());
    }
}


#endif 
