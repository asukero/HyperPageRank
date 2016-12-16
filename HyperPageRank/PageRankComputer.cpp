#include "PageRankComputer.hpp"
#include <fstream>
#include <sstream>

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

Graph<WebPage> PageRankComputer::loadGraph(const std::string & nodeFilename, const std::string & edgeFilename) {
	ifstream inNodefile(nodeFilename);
	ifstream inEdgefile(edgeFilename);
	
	Graph<WebPage> graph;
	cout << "Reading Nodes...";
	string dummyLine;

	//skipping first lines
	getline(inNodefile, dummyLine);

	while (inNodefile)
	{
		string line;
		getline(inNodefile, line);
		vector<string> nodeLine = this->_split(line, ' ');
		// If the line has the correct format (not empty, enough arguments...)
		if (nodeLine.size() > 2) {
			WebPage page = WebPage(URL(nodeLine[2]), stoi(nodeLine[0]));
			graph.addNode(page);
		}
	}
	cout << " Done." << endl;

	cout << "Reading Edges...";

	//skipping first lines
	getline(inEdgefile, dummyLine);

	while (inEdgefile)
	{
		string line;
		getline(inEdgefile, line);
		vector<string> nodeLine = this->_split(line, ' ');
		// If the line has the correct format (not empty, enough arguments...)
		if (nodeLine.size() > 1) {
			WebPage source = WebPage(stoi(nodeLine[0]));
			WebPage destination = WebPage(stoi(nodeLine[1]));
			graph.addArc(source, destination);
			//cout << "Arc : " << stoi(nodeLine[0]) << " => " << stoi(nodeLine[1]) << endl;
		}
	}
	cout << " Done." << endl;

	return graph;
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

// WARNING : read the explanation of the error in the hpp file
/*Hypergraph<WebPage> PageRankComputer::loadHypergraph(const std::string & nodeFilename, const std::string & edgeFilename) {
	return Hypergraph<WebPage>();
}*/

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
	for (i = 0; i < nbPages; i++) {
		pageRank.push_back(initialValue);
		futurePageRank.push_back(0);
	}
	// Iterates until pageRank values are "constant"
	// WARNING : max number of iterations used for the moment
	int maxIt = 20;
	for (int it = 0; it < maxIt; it++) {
		// Iterates on all the pages
		for (i = 0; i < nbPages; i++) {
			// Iterates on all the pages pointed by the current one 
			IndexList pointedPages = graph.getPointedNodes(i);
			for (j = 0; j < pointedPages.size(); j++) {
				futurePageRank.at(pointedPages.at(j)) += pageRank.at(i) / pointedPages.size();
			}
		}
		// Once every page has given its pageRank, they receive from the others
		for (i = 0; i < nbPages; i++) {
			if (articleVersion) {
				// Usual version (found on the Internet)
				pageRank.at(i) = (1 - c) * futurePageRank.at(i) + c / nbPages;
			} else {
				// Version of the article
				pageRank.at(i) = (1 - c) * futurePageRank.at(i) + c;
			}
			futurePageRank.at(i) = 0;
		}
	}
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
	for (i = 0; i < nbPages; i++) {
		pageRank.push_back(initialValue);
		futurePageRank.push_back(0);
	}
	// Iterates until pageRank values are "constant"
	// WARNING : max number of iterations used for the moment
	int maxIt = 20;
	for (int it = 0; it < maxIt; it++) {
		// First computes the reputation of the blocks
		for (i = 0; i < nbBlocks; i++) {
			IndexList blockPages = hypergraph.getPointingNodes(i);
			double sum = 0;
			for (j = 0; j < blockPages.size(); j++) {
				sum += pageRank.at(blockPages.at(j));
			}
			blockRank.at(i) = sum;
		}
		// Then iterates on all the blocks
		for (i = 0; i < nbBlocks; i++) {
			// Iterates on all the pages pointed by the current block 
			IndexList pointedPages = hypergraph.getPointedNodes(i);
			for (j = 0; j < pointedPages.size(); j++) {
				futurePageRank.at(pointedPages.at(j)) += blockRank.at(i) / pointedPages.size();
			}
		}
		// Once every block has given its pageRank, sets the pages pageRank
		for (i = 0; i < nbPages; i++) {
			// If a page is not given any reputation by any other one, it means it is not referenced
			if (futurePageRank.at(i) != 0) {
				pageRank.at(i) = (1 - c) * futurePageRank.at(i) + c / nbReferencedPages;
			} else {
				pageRank.at(i) = 0;
			}
			futurePageRank.at(i) = 0;
		}
	}
	return pageRank;
}

PageRank PageRankComputer::computeIndegree(Graph<WebPage>& graph) {
	PageRank indegree;
	indegree.clear();
	long nbPages = graph.getNodeList().size();
	long i, j;
	for (i = 0; i < nbPages; i++) {
		indegree.push_back(0);
	}
	// Iterates on all the pages
	for (i = 0; i < nbPages; i++) {
		// Iterates on all the pages pointed by the current one 
		IndexList pointedPages = graph.getPointedNodes(i);
		for (j = 0; j < pointedPages.size(); j++) {
			indegree.at(pointedPages.at(j))++;
		}
	}
	return indegree;
}

PageRank PageRankComputer::computeHyperIndegree(Hypergraph<WebPage>& hypergraph) {
	long nbPages = hypergraph.getNodeList().size();
	long nbBlocks = hypergraph.getHyperarcMatrix().getNbColumns();
	PageRank blockRank(nbBlocks);
	PageRank hyperIndegree;
	hyperIndegree.clear();
	long i, j;
	for (i = 0; i < nbPages; i++) {
		hyperIndegree.push_back(0);
	}
	// Iterates on all the blocks
	for (i = 0; i < nbBlocks; i++) {
		// Iterates on all the pages pointed by the current block 
		IndexList pointedPages = hypergraph.getPointedNodes(i);
		for (j = 0; j < pointedPages.size(); j++) {
			hyperIndegree.at(pointedPages.at(j))++;
		}
	}
	return hyperIndegree;
}

std::ostream & operator<<(std::ostream & flux, PageRank & pageRank) {
	for (int i = 0; i < pageRank.size(); i++) {
		flux << pageRank.at(i) << std::endl;
	}
	return flux;
}