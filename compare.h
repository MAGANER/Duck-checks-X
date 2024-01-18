#ifndef COMPARE_H
#define COMPARE_H
#include"strcomp.h"
#include"listdir.h"
#include<string>
#include<utility>
#include<algorithm>
#include<stdio.h>
#include<fstream>
#include<unistd.h>

typedef std::pair<std::string,std::string> spair;
struct CountedDiff
{
  std::string s1,s2;
  double value;
  CountedDiff(const std::string& s1, const std::string& s2, double value)
    : s1(s1), s2(s2),value(value) {}
  ~CountedDiff(){}
};
class Comparerer
{
  std::string file1,file2;
  svec content1, content2;
  double high_level_diff = 0;

  std::vector<CountedDiff*> diffs;
public:
  Comparerer(const std::string& file1, const std::string& file2)
    :file1(file1),file2(file2)
  {
      content1 = get_high_level_content(file1+".ext");
      content2 = get_high_level_content(file2+".ext");
  }
  ~Comparerer(){}

  void compare()
  {
    high_level_diff = match_svec(content1,content2);

    auto same_files = merge(content1,content2);
    for(auto& s:same_files)
      {
	auto f1 = file1+".ext/"+s.first;
	auto f2 = file2+".ext/"+s.second;

	auto f1_content = read_file(f1);
	auto f2_content =read_file(f2);
	
	diffs.push_back(new CountedDiff(f1,f2, match_s(f1_content,f2_content)));
      }
    
  }  
  double get_high_level_diff(){ return high_level_diff; }
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
  
};
#endif
