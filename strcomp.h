#ifndef STRCOMP_H
#define STRCOMP_H
#include<string>

//thanks to this JS solution
//https://stackoverflow.com/questions/41561452/is-there-any-way-to-calculate-match-between-2-strings
extern double match(const std::string& str1, const std::string& str2)
{
  double tmpValue = 0;
  int minLength = str1.length();
  if(str1.length() > str2.length())
  {
    minLength = str2.length();
  }
  
  int maxLength = str1.length();
  if(str1.length() < str2.length())
    {
      maxLength = str2.length();
    }
  for(int i = 0; i < minLength; i++)
    {
      if(str1[i] == str2[i])
      {
	tmpValue++;
      }
    }

  double weight = tmpValue / maxLength;
  return (weight * 100);
}

#endif
