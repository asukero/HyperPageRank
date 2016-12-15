#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Graph.hpp"
#include "Hypergraph.hpp"
#include "WebPage.h"

typedef std::vector<double> PageRank;

class PageRankComputer {
private:
	vector<std::string> _split(const string &s, char delim) const;
public:
	// Constructor
	PageRankComputer();
	// Destructor
	~PageRankComputer();
	// Loads an graph from a file
	Graph<WebPage> loadGraph(const std::string & nodeFilename, const std::string & edgeFilename);
	// Loads an hypergraph from a graph
	Hypergraph<WebPage> loadHypergraph(const std::string & nodeFilename, const std::string & edgeFilename);
	// Calculates the PageRank of a graph
	PageRank computePageRank(Graph<std::string> & graph, const bool articleVersion);
	// Calculates the pageRank of an hypergraph
	PageRank computeHyperPageRank(Hypergraph<std::string> & hypergraph);
	// Applies the Indegree algorithm on a graph
	PageRank computeIndegree(Graph<std::string> & graph);
	// Applies the Indegree algorithm on a hypergraph
	PageRank computeHyperIndegree(Hypergraph<std::string> & hypergraph);
};

std::ostream & operator<<(std::ostream & flux, PageRank & pageRank);