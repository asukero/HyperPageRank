#pragma once
#include "URL.h"

class WebPage
{
private:
	int id;
	URL url;
public:
	WebPage(int id);
	WebPage(URL url, int id);
	URL getUrl() const;
	int getId() const;
	void setURL(URL url);
	void setID(int id);
};

std::ostream & operator<<(std::ostream & flux, WebPage & webPage);
bool operator==(WebPage & wp1, WebPage & wp2);