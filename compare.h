#ifndef COMPARE_H
#define COMPARE_H
#include"strcomp.h"
#include"listdir.h"
#include"colors.h"
#include<string>
#include<utility>
#include<algorithm>
#include<stdio.h>
#include<fstream>
#include<unistd.h>
#include<map>

typedef std::pair<std::string,std::string> spair;
struct CountedDiff
{
  std::string s1,s2;
  double value;
  CountedDiff(const std::string& s1, const std::string& s2, double value)
    : s1(s1), s2(s2),value(value) {}
  ~CountedDiff(){}
};
struct DiffHub
{
  std::string target_name;
  std::vector<CountedDiff*> diffs;
  double high_level_diff;
  DiffHub(const std::string& target_name,
	  std::vector<CountedDiff*>& diffs,
	  double high_level_diff)
    :target_name(target_name),diffs(diffs), high_level_diff(high_level_diff)
  {}
  ~DiffHub(){}
};

#define NOTFOUND(x,sub) x.find(sub) == std::string::npos
class Comparerer
{
  std::string file1,file2;
  svec content1_files, content1_dirs;
  svec content2_files, content2_dirs;

  std::map<std::string, svec> content1, content2;
  std::vector<DiffHub*> diff_hubs;

  // function that is used for testing
  svec get_map_keys(std::map<std::string,svec>& map)
  {
    svec keys;
    for(auto el: map)
      keys.push_back(el.first);

    return keys;
  }
public:
  Comparerer(const std::string& file1, const std::string& file2)
    :file1(file1),file2(file2)
  {
      get_high_level_content(file1+".ext",content1_files,content1_dirs);
      get_high_level_content(file2+".ext",content2_files,content2_dirs);

      for(auto& dir:content1_dirs)
      {
	read_dir(file1+".ext/"+dir,content1);
      }
      for(auto& dir:content2_dirs)
      {
	read_dir(file2+".ext/"+dir,content2);
      }    
      //print_docx_content(content1,file1);
      //print_docx_content(content2,file2); 

     compare(".ext/",".",content1_files,content2_files);
     for(auto& el: content1)
       {
	 auto dir = el.first;
	 auto dir_files = el.second;
	 compare(trim(dir)+"/",get_dir(dir),content1[dir],content2[file2+trim(dir)]);
       }
  }
  ~Comparerer(){}

  
  void print()
  {
    for(auto& hub: diff_hubs)
    {
      printf("%s\n",hub->target_name.c_str());
      printf("high level difference: %s %f%% %s\n",ANSI_COLOR_RED,hub->high_level_diff, ANSI_COLOR_END);
      for(auto& d: hub->diffs)
      {
	printf("%s --- %s difference: %s %f%% %s\n",d->s1.c_str(),d->s2.c_str(),ANSI_COLOR_RED,d->value,ANSI_COLOR_END);
      }
    }
    printf("-----------\n");
  }
private:
  std::vector<spair> merge(svec& vec1, svec& vec2)
    {
      std::vector<spair> result;
      std::sort(vec1.begin(),vec1.end());
      std::sort(vec2.begin(),vec2.end());
      for(auto& s1: vec1)
	for(auto& s2:vec2)
	  if(s1 == s2)
	    {
	      result.push_back({s1,s2});
	    }

      return result;
    }
  std::string read_file(const std::string& filename)
  {
    std::string content;
    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
      return "";
    }
    
    std::string line;
    while (getline(file, line))
      content += line;
      
    file.close();
    return content;
  }
  void read_dir(const std::string& dir, std::map<std::string,svec>& out)
  {
    out[dir] = svec();
    get_high_level_content(dir,out[dir],out[dir]);
  }

  void compare(const std::string top_dir,const std::string& target_name,svec& content1, svec& content2)
  {
    double high_level_diff = match_svec(content1,content2);
    auto same_files = merge(content1,content2);

    std::vector<CountedDiff*> diffs;
    for(auto& s:same_files)
      {
	//skip since these are directories
	if(s.first == "_rels" or s.first == "theme") continue;
	
	auto f1 = file1+top_dir+s.first;
	auto f2 = file2+top_dir+s.second;

	auto f1_content = read_file(f1);
	auto f2_content =read_file(f2);

	diffs.push_back(new CountedDiff(f1,f2, match_s(f1_content,f2_content)));
      }

    diff_hubs.push_back(new DiffHub(target_name,diffs,high_level_diff));
  }
  std::string get_dir(const std::string& full)
  {
    auto last = full.find_last_of("/");
    auto end  = full.size();

    return full.substr(last,end);
  }
  std:: string trim(const std::string& full)
  {
    auto dot = full.find(".");
    return full.substr(dot,full.size());
  }
  void print_docx_content(std::map<std::string, svec>& content, const std::string& name)
  {
     for(auto& dir: content)
	{
	  printf("%s :",dir.first.c_str());
	  for(auto& f:content[dir.first])
	    {
	      printf("%s , ",f.c_str());
	    }
	  printf("\n");
	}
     printf("---\n");
  }
};
#endif
