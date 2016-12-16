#include "WebPage.h"

WebPage::WebPage(int id)
{
	this->id = id;
	this->url = URL();
}

WebPage::WebPage(URL url, int id = -1) : url(url)
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

std::ostream & operator<<(std::ostream & flux, WebPage & webPage) {
	flux << webPage.getId() << " : ";
	flux << webPage.getUrl();
	return flux;
}

bool operator==(WebPage & wp1, WebPage & wp2) {
	return wp1.getId() == wp2.getId();
}