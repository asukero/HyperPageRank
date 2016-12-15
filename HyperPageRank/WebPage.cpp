#include "WebPage.h"

WebPage::WebPage(int id)
{
	this->id = id;
	this->url = URL();
}

WebPage::WebPage(int id = -1, URL url) : url(url)
{
	this->id = id;
	this->url = url;
}

int WebPage::getId() const
{
	return this->id;
}

void WebPage::setURL(URL url)
{
	this->url = url;
}

void WebPage::setID(int id)
{
	this->id = id;
}

URL WebPage::getUrl() const
{
	return this->url;
}

