#include <algorithm>

static inline void lTrim(std::string &s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
	{
		return !std::isspace(ch);
	}));
}

static inline void rTrim(std::string &s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
	{
		return !std::isspace(ch);
		}).base(), s.end());
}

static inline char* trim(std::string &s)
{
	lTrim(s);
	rTrim(s);

	return (char*) s.c_str();
}
