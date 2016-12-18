#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include "Graph.hpp"
#include "Hypergraph.hpp"
#include "WebPage.h"

typedef std::vector<std::pair<double,URL>> PageRank;

class PageRankComputer {
private:
	vector<std::string> _split(const string &s, char delim) const;
public:
	// Constructor
	PageRankComputer();
	// Destructor
	~PageRankComputer();
	// Loads a graph and an hypergraph from a file
	void loadGraphAndHypergraph(Graph<WebPage> & graph, Hypergraph<WebPage> & hypergraph, const std::string & nodeFilename, const std::string & edgeFilename);
	// Calculates the PageRank of a graph
	PageRank computePageRank(Graph<WebPage> & graph, const bool articleVersion);
	// Calculates the pageRank of an hypergraph
	PageRank computeHyperPageRank(Hypergraph<WebPage> & hypergraph);
	// Applies the Indegree algorithm on a graph
	PageRank computeIndegree(Graph<WebPage> & graph);
	// Applies the Indegree algorithm on a hypergraph
	PageRank computeHyperIndegree(Hypergraph<WebPage> & hypergraph);
};

std::wostream & operator<<(std::wostream & flux, PageRank & pageRank);