#ifndef STRCOMP_H
#define STRCOMP_H
#include<string>

//thanks to this JS solution
//https://stackoverflow.com/questions/41561452/is-there-any-way-to-calculate-match-between-2-strings

template<typename T>
extern double match(T& str1, T& str2)
{
  double tmpValue = 0;
  int minSize = str1.size();
  if(str1.size() > str2.size())
  {
    minSize = str2.size();
  }
  
  int maxSize = str1.size();
  if(str1.size() < str2.size())
    {
      maxSize = str2.size();
    }
  for(int i = 0; i < minSize; i++)
    {
      if(str1[i] == str2[i])
      {
	tmpValue++;
      }
    }

  double weight = tmpValue / maxSize;
  return (weight * 100);
}
#define match_s(s1,s2) match<std::string>(s1,s2)
#define match_svec(s1,s2) match<svec>(s1,s2)

#endif
