#include "PageRankComputer.hpp"

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

Graph<std::string> PageRankComputer::loadGraph(const std::string & fileName) {
	return Graph<std::string>();
}

Hypergraph<std::string> PageRankComputer::loadHypergraph(const std::string & fileName) {
	return Hypergraph<std::string>();
}

PageRank PageRankComputer::computePageRank(Graph<std::string> & graph, const bool articleVersion) {
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

PageRank PageRankComputer::computeHyperPageRank(Hypergraph<std::string> & hypergraph) {
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

std::ostream & operator<<(std::ostream & flux, PageRank & pageRank) {
	for (int i = 0; i < pageRank.size(); i++) {
		flux << pageRank.at(i) << std::endl;
	}
	return flux;
}