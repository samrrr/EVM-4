

#include "usefull.h"

//using namespace mtl;
//using namespace itl;
using namespace std;


std::string ftos(float number)
{
	std::stringstream buff;
	
	buff << number;
	string s;
	buff >> s;
	return s;
}

double atof1(string s)
{
	for (int i = 0; i < s.length(); i++)
	{
		if ((s[i]<'0' || s[i]>'9') && s[i] != ','&& s[i] != 'e'&& s[i] != 'E'&& s[i] != '-')
		{
			cout << "convert broken  " << s << " isnt number" << endl;
			system("pause");
		}

	}
	return std::atof(s.data());
}
/*
double atof1(string s)
{
	myreplace(s,".",",");
	return std::atof(s.data());
}
*/

string comp_to_s(complex<double> v)
{
	string res;

	if (v._Val[1] == 0)
	{
		return ftos(v._Val[0]);
	}
	else
	{
		if (v._Val[1] > 0)
			return ftos(v._Val[0]) + "+" + ftos(v._Val[1]) + "*I";
		else
			return ftos(v._Val[0]) + ftos(v._Val[1]) + "*I";
	}
}





