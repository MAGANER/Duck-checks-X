#ifndef COMPARE_H
#define COMPARE_H
#include"strcomp.h"
#include"listdir.h"
#include<string>

struct Comparision
{
  double high_level;

  Comparision(double high_level):high_level(high_level){}
  ~Comparision(){}
};

static double compare_on_high_level(const std::string& file1, const std::string& file2)
{
  auto content1 = get_high_level_content(file1+".ext");
  auto content2 = get_high_level_content(file2+".ext");
  return match_svec(content1,content2);
}
  
extern Comparision* compare(const std::string& file1, const std::string& file2)
{
  auto high_level_diff = compare_on_high_level(file1,file2);
  return new Comparision(high_level_diff);
}
#endif
