#include "PageRankComputer.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>

// Computes the number of referenced pages in an hypergraph
double getNbReferencedPages(Matrix<int> & matrix) {
	int nbRows = matrix.getNbRows();
	int nbColumns = matrix.getNbColumns();
	double nbReferencedPages = 0;
	int i, j = 0;
	bool found = false;
	// Iterates on the list of nodes
	for (i = 0; i < nbRows; i++) {
		found = false;
		j = 0;
		while (j < nbColumns && !found) {
			// If a node has 1 as value at M(i,j), meaning it is referenced
			int value = matrix.getValue(i, j);
			if (matrix.getValue(i, j) == 1) {
				nbReferencedPages++;
				found = true;
			}
			j++;
		}
	}
	return nbReferencedPages;
}

PageRankComputer::PageRankComputer() {
}

PageRankComputer::~PageRankComputer() {
}

void PageRankComputer::loadGraphAndHypergraph(Graph<WebPage>& graph, Hypergraph<WebPage>& hypergraph, const std::string & nodeFilename, const std::string & edgeFilename) {
	// Mapping nodeID => <realID(graph) ; domainName>
	std::unordered_map<long, std::pair<long, std::wstring>> nodeMapping;
	// Mapping domainName => hyperarcID
	std::unordered_map<std::wstring, long> domainMapping;
	
	ifstream inNodefile(nodeFilename);
	ifstream inEdgefile(edgeFilename);
	cout << "Reading Nodes..." << endl;
	string dummyLine, totalLines;
	getline(inNodefile, totalLines);
	//skipping first lines
	getline(inNodefile, dummyLine);
	int lineNumber = -1;
	while (inNodefile)
	{
		string line;
		getline(inNodefile, line);
		vector<string> nodeLine = this->_split(line, ' ');
		// If the line has the correct format (not empty, enough arguments...)
		if (nodeLine.size() > 2) {
			URL url((nodeLine[2]));
			int nodeId = stoi(nodeLine[0]);
			// Creates the web page
			WebPage page = WebPage(url, nodeId);
			// Adds it to the graph and the hypergraph
			long realID = graph.addNode(page);
			realID = hypergraph.addNode(page);
			std::pair<long, std::wstring> pair = std::make_pair(realID, url.Domain);
			// Adds the node to the mapping
			nodeMapping.insert(std::pair<long, std::pair<long, std::wstring>>(nodeId, pair));
		}
		lineNumber++;
		cout << "\tNodes processed: " << lineNumber << "/" << totalLines << '\r' << flush;
	}
	cout << "\nDone." << endl;

	cout << "Reading Edges..." << endl;

	getline(inEdgefile, totalLines);
	//skipping first lines
	getline(inEdgefile, dummyLine);
	lineNumber = -1;
	while (inEdgefile)
	{
		string line;
		getline(inEdgefile, line);
		vector<string> nodeLine = this->_split(line, ' ');
		// If the line has the correct format (not empty, enough arguments...)
		if (nodeLine.size() > 1) {
			int sourceId = stoi(nodeLine[0]);
			int destinationId = stoi(nodeLine[1]);
			try {
				long realSourceId = nodeMapping.at(sourceId).first;
				long realDestinationId = nodeMapping.at(destinationId).first;
				wstring sourceDomainName = nodeMapping.at(sourceId).second;
				wstring destinationDomainName = nodeMapping.at(destinationId).second;
				// ********** GRAPH PART **********
				graph.addArc(realSourceId, realDestinationId);
				// ********************************
				// ******** HYPERGRAPH PART *******
				// If the two pages are from the same domain, they must not be linked, but a arc must be created
				// where both pages are source
				bool bothOrigins = false;
				long hyperArcIndex;
				if (sourceDomainName == destinationDomainName) {
					bothOrigins = true;
				}
				// Verifies if the domain name of the source already exists in the domain mapping
				bool alreadyExisting = domainMapping.find(sourceDomainName) != domainMapping.end();
				// If it does exist, the destination node must be added to the pointing nodes of the existing hyperarc,
				// using its index
				if (alreadyExisting) {
					hyperArcIndex = domainMapping.at(sourceDomainName);
					hypergraph.addNodeToHyperArc(hyperArcIndex, realDestinationId, bothOrigins);
				}
				// If the domain name is not present in the mapping, a new hyperarc must be created, 
				// and its index added to the mapping
				else {
					hyperArcIndex = hypergraph.addHyperArc(realSourceId, realDestinationId, bothOrigins);
					domainMapping.insert(std::pair<wstring, long>(sourceDomainName, hyperArcIndex));
				}
				// ********************************
			// If one of the two nodes does not exist (ie its id is not present in the mapping) ...
			} catch (...) {}
		}
		lineNumber++;
		cout << "\tEdges processed: " << lineNumber << "/" << totalLines << '\r' << flush;
	}
	cout << "\nDone." << endl;

}

vector<string> PageRankComputer::_split(const string &s, char delim) const
{
	vector<string> result = vector<string>();
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim)) {
		result.push_back(item);
	}
	return result;
}


PageRank PageRankComputer::computePageRank(Graph<WebPage> & graph, const bool articleVersion) {
	PageRank pageRank;
	PageRank futurePageRank;
	// c is the dampening factor
	double c = 0.15;
	
	pageRank.clear();
	long nbPages = graph.getNodeList().size();
	long i, j;
	// All the pages are first initialized with a default pageRank
	double initialValue = 1;
	cout << "Initializing default pageRank on each page..." << endl;
	for (i = 0; i < nbPages; i++) {
		pageRank.push_back(make_pair(initialValue, graph.getNodeList().at(i).getContent().getUrl()));
		futurePageRank.push_back(make_pair(0, graph.getNodeList().at(i).getContent().getUrl()));
		cout << "\tPages processed: " << i + 1 << "/" << nbPages << '\r' << flush;
	}
	cout << "\nDone." << endl;
	// Iterates until pageRank values are "constant"
	// WARNING : max number of iterations used for the moment
	cout << "Computing PageRank..." << endl;
	cout << "\tProgress: 0%" << '\r' << flush;
	int maxIt = 20;
	for (int it = 0; it < maxIt; it++) {
		// Iterates on all the pages
		for (i = 0; i < nbPages; i++) {
			// Iterates on all the pages pointed by the current one 
			IndexList pointedPages = graph.getPointedNodes(i);
			for (j = 0; j < pointedPages.size(); j++) {
				futurePageRank.at(pointedPages.at(j)).first += pageRank.at(i).first / pointedPages.size();
			}
		}
		// Once every page has given its pageRank, they receive from the others
		for (i = 0; i < nbPages; i++) {
			if (articleVersion) {
				// Usual version (found on the Internet)
				pageRank.at(i).first = (1 - c) * futurePageRank.at(i).first + c / nbPages;
			} else {
				// Version of the article
				pageRank.at(i).first = (1 - c) * futurePageRank.at(i).first + c;
			}
			futurePageRank.at(i).first = 0;
		}
		cout << "\tProgress: " << (100 / maxIt) + it * 100 / maxIt << "%" << '\r' << flush;
	}
	cout << "\nDone." << endl;
	return pageRank;
}

PageRank PageRankComputer::computeHyperPageRank(Hypergraph<WebPage> & hypergraph) {
	PageRank pageRank;
	PageRank futurePageRank;
	PageRank blockRank;
	pageRank.clear();
	futurePageRank.clear();
	blockRank.clear();
	// c is the dampening factor
	double c = 0.15;
	long nbPages = hypergraph.getNodeList().size();
	long nbBlocks = hypergraph.getHyperarcMatrix().getNbColumns();
	blockRank.resize(nbBlocks);
	long i, j;
	// All the pages are first initialized with a default pageRank : the number of pages with incoming
	// hyperarcs in the collection
	double nbReferencedPages = getNbReferencedPages(hypergraph.getHyperarcMatrix());
	double initialValue = 1 / nbReferencedPages;
	cout << "Initializing default pageRank on each page..." << endl;
	for (i = 0; i < nbPages; i++) {
		pageRank.push_back(make_pair(initialValue, hypergraph.getNodeList().at(i).getContent().getUrl()));
		futurePageRank.push_back(make_pair(0, hypergraph.getNodeList().at(i).getContent().getUrl()));
		cout << "\tPages processed: " << i + 1 << "/" << nbPages << '\r' << flush;
	}
	cout << "\nDone." << endl;
	// Iterates until pageRank values are "constant"
	// WARNING : max number of iterations used for the moment
	cout << "Computing HyperPageRank..." << endl;
	cout << "\tProgress: 0%" << '\r' << flush;
	int maxIt = 10;
	for (int it = 0; it < maxIt; it++) {
		// First computes the reputation of the blocks
		for (i = 0; i < nbBlocks; i++) {
			IndexList blockPages = hypergraph.getPointingNodes(i);
			double sum = 0;
			for (j = 0; j < blockPages.size(); j++) {
				sum += pageRank.at(blockPages.at(j)).first;
			}
			blockRank.at(i).first = sum;
		}
		// Then iterates on all the blocks
		for (i = 0; i < nbBlocks; i++) {
			// Iterates on all the pages pointed by the current block 
			IndexList pointedPages = hypergraph.getPointedNodes(i);
			for (j = 0; j < pointedPages.size(); j++) {
				futurePageRank.at(pointedPages.at(j)).first += blockRank.at(i).first / pointedPages.size();
			}
		}
		// Once every block has given its pageRank, sets the pages pageRank
		for (i = 0; i < nbPages; i++) {
			// If a page is not given any reputation by any other one, it means it is not referenced
			if (futurePageRank.at(i).first != 0) {
				pageRank.at(i).first = (1 - c) * futurePageRank.at(i).first + c / nbReferencedPages;
			} else {
				pageRank.at(i).first = 0;
			}
			futurePageRank.at(i).first = 0;
		}
		cout << "\tProgress: " << (100 / maxIt) + it * 100 / maxIt << "%" << '\r' << flush;
	}
	cout << "\nDone." << endl;
	return pageRank;
}

PageRank PageRankComputer::computeIndegree(Graph<WebPage>& graph) {
	PageRank indegree;
	indegree.clear();
	long nbPages = graph.getNodeList().size();
	long i, j;
	cout << "Initializing default indegree on each page..." << endl;
	for (i = 0; i < nbPages; i++) {
		indegree.push_back(make_pair(0, graph.getNodeList().at(i).getContent().getUrl()));
		cout << "\tPages processed: " << i << "/" << nbPages << '\r' << flush;
	}
	cout << "\nDone." << endl;
	// Iterates on all the pages
	for (i = 0; i < nbPages; i++) {
		// Iterates on all the pages pointed by the current one 
		IndexList pointedPages = graph.getPointedNodes(i);
		for (j = 0; j < pointedPages.size(); j++) {
			indegree.at(pointedPages.at(j)).first++;
		}
		cout << "\tPages processed: " << i + 1 << "/" << nbPages << '\r' << flush;
	}
	cout << "\nDone." << endl;
	return indegree;
}

PageRank PageRankComputer::computeHyperIndegree(Hypergraph<WebPage>& hypergraph) {
	long nbPages = hypergraph.getNodeList().size();
	long nbBlocks = hypergraph.getHyperarcMatrix().getNbColumns();
	PageRank blockRank(nbBlocks);
	PageRank hyperIndegree;
	hyperIndegree.clear();
	long i, j;
	cout << "Initializing default indegree on each page..." << endl;
	for (i = 0; i < nbPages; i++) {
		hyperIndegree.push_back(make_pair(0, hypergraph.getNodeList().at(i).getContent().getUrl()));
		cout << "\tPages processed: " << i + 1 << "/" << nbPages << '\r' << flush;
	}
	cout << "\nDone." << endl;
	// Iterates on all the blocks
	cout << "Iterating on blocks:" << endl;
	for (i = 0; i < nbBlocks; i++) {
		// Iterates on all the pages pointed by the current block 
		IndexList pointedPages = hypergraph.getPointedNodes(i);
		for (j = 0; j < pointedPages.size(); j++) {
			hyperIndegree.at(pointedPages.at(j)).first++;
		}
		cout << "\tBlocks processed: " << i + 1 << "/" << nbBlocks << '\r' << flush;
	}
	cout << "\nDone." << endl;
	return hyperIndegree;
}

wostream & operator<<(wostream & flux, PageRank & pageRank) {
	// Display only the top 20 values
	cout << "Sorting results..." << endl;
	sort(pageRank.begin(), pageRank.end(), [](const pair<double, URL> &left, const pair<double, URL> &right) {
		return left.first > right.first;
	});
	cout << "---------- Top 20 ----------" << endl;
	long double average = 0;
	for (int i = 0; i < pageRank.size(); i++) {
		if(i < 20)
		{
			flux << "Score: " << pageRank.at(i).first << " - URL: " << pageRank.at(i).second << endl;
		}
		average += pageRank.at(i).first;
	}
	average = average / pageRank.size();
	flux << "Average Score: " << average << endl;
	return flux;
}