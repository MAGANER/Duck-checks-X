#ifndef STRCOMP_H
#define STRCOMP_H
#include<string>
#define CDP(size, count) 100.0 - (double)count/(double)size
#define MAX(a,b) a > b? a: b
#define ABS(x) x< 0? x*(-1): x 

static size_t count_equal_chars
(
	const std::string& a,
	const std::string& b,
	size_t size
)
{
	size_t count = 0;
	for (size_t i = 0; i < size; ++i)
		if (a[i] == b[i]) count++;

	return count;
}
extern double compute_string_difference_percentage
(
	const std::string& a,
	const std::string& b
)
{
	size_t len1 = a.length();
	size_t len2 = b.length();

	size_t equal_chars_count = 0;
	if (a.size() == b.size())
		equal_chars_count = count_equal_chars(a, b, a.size());
	else if (a.size() > b.size())
		equal_chars_count = count_equal_chars(a, b, b.size());
	else
		equal_chars_count = count_equal_chars(a, b, a.size());

	
	if (a.size() == b.size())
	{
		return CDP((int)a.size(),(int)equal_chars_count);
	}
	else
	{
		size_t max_len = MAX(a.size(), b.size());
		auto diff_count = CDP(max_len, equal_chars_count);
		return diff_count;
	}
}


#endif