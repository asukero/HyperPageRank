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

URL::URL()
{
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

	iterator_t realHostBegin;
	iterator_t realHostEnd;
	wstring realHost;

	if (result.Host.find(L"www.") == 0)
	{
		realHost = result.Host.erase(0, 4);
		realHostBegin = realHost.begin();
		realHostEnd = realHost.end();
	}
	else
	{
		realHost = result.Host;
		realHostBegin = hostStart;
		realHostEnd = hostEnd;
	}

	iterator_t domainStart = find(realHostBegin, realHostEnd, L'.');
	++domainStart;
	result.Domain = wstring(domainStart, realHostEnd);



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

std::ostream & operator<<(std::ostream & flux, URL & url)
{
	// TODO
	flux << "SHOULD BE THE URL TEXT";
	return flux;
}