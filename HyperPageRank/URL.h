#pragma once

#include <string>
using namespace std;
class URL
{
public:
	URL();
	URL(wstring url);
	URL(string url);
	void Parse(const wstring url);
	wstring QueryString, Path, Protocol, Host, Port, Domain;
};

std::wostream & operator<<(std::wostream & flux, URL & url);