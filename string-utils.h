#include <vector>
#include <string>
#define sswitch(str)\
	std::string _s = str;
#define scase(str,statement)\
	if(_s == str){statement}\
	else
#define close_sswitch {}
#define sdefault
namespace nstd{
	std::vector<std::string> str_split (std::string&);
}

