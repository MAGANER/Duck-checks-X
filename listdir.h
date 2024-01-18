#ifndef LISTDIR_H
#define LISTDIR_H
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include<string>

/*
  based on https://www.gnu.org/software/libc/manual/html_node/Simple-Directory-Lister.html
*/
extern std::string get_high_level_content(const std::string& docx_dir)
{
  std::string content;

  DIR *dp;
  struct dirent *ep;

  dp = opendir (docx_dir.c_str());
  if (dp != NULL)
    {
      while (ep = readdir (dp))
      {
	std::string dir  = std::string(ep->d_name);
	if(dir != "." && dir != "..")
	  content += std::string(ep->d_name) + " ";
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
