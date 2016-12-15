#pragma once

#include <string>
using namespace std;
class URL
{
public:
	URL();
	URL(wstring url);
	URL(string url);
	static URL Parse(const wstring url);
	wstring QueryString, Path, Protocol, Host, Port, Domain;
};