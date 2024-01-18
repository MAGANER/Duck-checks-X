#ifndef LISTDIR_H
#define LISTDIR_H
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include<string>
#include<vector>

typedef std::vector<std::string> svec;

/*
  based on https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html
*/
extern svec get_high_level_content(const std::string& docx_dir)
{
  svec content;

  DIR *dp;
  struct dirent *ep;

  dp = opendir (docx_dir.c_str());
  if (dp != NULL)
    {
      while (ep = readdir (dp))
      {
	std::string dir  = std::string(ep->d_name);
	if(dir != "." && dir != "..")
	  content.push_back(std::string(ep->d_name));
      }
      (void) closedir (dp);
    }
  else
    {
      printf("failed to open %s directory!\n",docx_dir.c_str());
    }
    
  return content;
}


#endif 
