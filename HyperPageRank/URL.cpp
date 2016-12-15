#include "URL.h"
#include <locale>
#include <codecvt>
using namespace std;

URL::URL(string url)
{
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	wstring wstring = converter.from_bytes(url);
	this->Parse(wstring);
}

URL::URL(wstring url)
{
	this->Parse(url);
}

URL URL::Parse(const wstring url)
{
	URL result;

	typedef wstring::const_iterator iterator_t;

	if (url.length() == 0)
		return result;

	iterator_t urlEnd = url.end();

	// get query start
	iterator_t queryStart = find(url.begin(), urlEnd, L'?');

	// protocol
	iterator_t protocolStart = url.begin();
	iterator_t protocolEnd = find(protocolStart, urlEnd, L':');

	if (protocolEnd != urlEnd)
	{
		wstring prot = &*(protocolEnd);
		if ((prot.length() > 3) && (prot.substr(0, 3) == L"://"))
		{
			result.Protocol = wstring(protocolStart, protocolEnd);
			protocolEnd += 3;
		}
		else protocolEnd = url.begin();
	}
	else protocolEnd = url.begin();

									
	iterator_t hostStart = protocolEnd;
	iterator_t pathStart = find(hostStart, urlEnd, L'/');

	iterator_t hostEnd = find(protocolEnd,
		(pathStart != urlEnd) ? pathStart : queryStart,
		L':');

	result.Host = wstring(hostStart, hostEnd);

	if ((hostEnd != urlEnd) && ((&*(hostEnd))[0] == L':'))
	{
		++hostEnd;
		iterator_t portEnd = (pathStart != urlEnd) ? pathStart : queryStart;
		result.Port = wstring(hostEnd, portEnd);
	}

	if (pathStart != urlEnd)
		result.Path = wstring(pathStart, queryStart);

	if (queryStart != urlEnd)
		result.QueryString = wstring(queryStart, url.end());

	return result;
}
