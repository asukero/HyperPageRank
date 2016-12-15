#pragma once
#include "URL.h"

class WebPage
{
private:
	int id;
	URL url;
public:
	WebPage(int id);
	WebPage(int id, URL url);
	URL getUrl() const;
	int getId() const;
	void setURL(URL url);
	void setID(int id);
	
};
