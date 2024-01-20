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


//contains difference for 2 files
struct CountedDiff
{
  std::string s1,s2; //files
  double value;
  CountedDiff(const std::string& s1, const std::string& s2, double value)
    : s1(s1), s2(s2),value(value) {}
  ~CountedDiff(){}
};

//contains differences of files for the same 2 directories
//_rels and _rels for example
struct DiffHub
{
  std::string target_name;
  std::vector<CountedDiff*> diffs;
  double high_level_diff; //difference on directory level
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
    printf("%s %s directory's structure %s\n",ANSI_COLOR_MAGENTA,file1.c_str(),ANSI_COLOR_END);
    print_docx_content(content1,file1);

    printf("%s %s directory's structure %s\n",ANSI_COLOR_MAGENTA,file2.c_str(),ANSI_COLOR_END);
    print_docx_content(content2,file2); 

    print_file_version(file1);
    print_file_version(file2);
    printf("\n");
    
    for(auto& hub: diff_hubs)
    {
      if(hub->diffs.empty())continue;
      
      //show common info: what's the dir and high level difference
      auto target = ANSI_COLOR_CYAN + hub->target_name + ANSI_COLOR_END;
      printf(".docx's dir - %s\n",target.c_str());
      printf("high level difference: %s %f%% %s\n",ANSI_COLOR_RED,hub->high_level_diff, ANSI_COLOR_END);

      auto left_max_size = compute_max_size_for_hub_target(hub);
      auto right_max_size = compute_max_size_for_hub_target(hub,false);
      for(auto& d: hub->diffs)
      {	
	auto diff_str = ANSI_COLOR_RED+std::to_string(d->value)+"%" + ANSI_COLOR_END;
	auto left     = ANSI_COLOR_YELLOW + d->s1;
	auto right    = ANSI_COLOR_GREEN  + d->s2;
	
	prepare_result_string(left);
	prepare_result_string(right);
	
	auto output = left + " --- "+right+" difference : "+diff_str;
	make_align(d->s1,output,left_max_size,"-");
	make_align(d->s2,output,right_max_size,"difference");
	printf("%s\n",output.c_str());
      }
      
      printf("\n");
    }
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
	//skip since these are directories. TODO: process them later, recursively
	if(s.first == "_rels" or s.first == "theme") continue;
	
	auto f1 = file1+top_dir+s.first;
	auto f2 = file2+top_dir+s.second;

	auto f1_content = read_file(f1);
	auto f2_content = read_file(f2);

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
  std::string trim(const std::string& full)
  {
    //remove root directory
    auto dot = full.find(".");
    return full.substr(dot,full.size());
  }
  void make_align(const std::string& str,std::string& out, size_t max_len, const std::string& point)
  {
    auto start = out.find(point)-1;
    int diff = abs((int)max_len-(int)str.size());
    
    std::string align(diff,' ');
    out.insert(start,align);
  }
  size_t compute_max_size_for_hub_target(DiffHub* hub, bool s1=true)
  {
    auto vals = svec();
    for(auto& h:hub->diffs)
      if(s1)vals.push_back(h->s1);
      else  vals.push_back(h->s2);
    
    return  (*std::max_element(vals.begin(),vals.end())).size();  
  }
  
  void prepare_result_string(std::string& full)
  {
    //insert END sequence to make colored only root directory
    auto last = full.find_last_of("/");
    full.insert(last,ANSI_COLOR_END);
  }
  void print_docx_content(std::map<std::string, svec>& content, const std::string& name)
  {
     for(auto& dir: content)
	{
	  printf("%s :",dir.first.c_str());
	  for(auto& f:content[dir.first])
	    {
	      printf("%s ",f.c_str());
	    }
	  printf("\n");
	}
  }

  std::string get_app_version(const std::string& file)
  {
    auto dest    = file+".ext/docProps/app.xml";
    auto content = read_file(dest);
    
    const std::string start_val = "<AppVersion>";
    auto start = content.find(start_val);
    auto end = content.find("</AppVersion>");
    if(start == std::string::npos or end == std::string::npos)return "";

    auto sub = content.substr(start+start_val.size(),end);
    end = sub.find("</AppVersion>");
    return sub.substr(0,end);
  }
  void print_file_version(const std::string& file)
  {
    auto ver1 = ANSI_COLOR_YELLOW+get_app_version(file)+ANSI_COLOR_END;
    printf("%s's version: %s\n",file.c_str(),ver1.c_str());
  }
};
#endif
