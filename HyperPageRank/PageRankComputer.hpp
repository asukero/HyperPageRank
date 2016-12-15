#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Graph.hpp"
#include "Hypergraph.hpp"

typedef std::vector<double> PageRank;

class PageRankComputer {
public:
	// Constructor
	PageRankComputer();
	// Destructor
	~PageRankComputer();
	// Loads an graph from a file
	Graph<std::string> loadGraph(const std::string & fileName);
	// Loads an hypergraph from a graph
	Hypergraph<std::string> loadHypergraph(const std::string & fileName);
	// Calculates the PageRank of a graph
	PageRank computePageRank(Graph<std::string> & graph, const bool articleVersion);
	// Calculates the pageRank of an hypergraph
	PageRank computeHyperPageRank(Hypergraph<std::string> & hypergraph);
};

std::ostream & operator<<(std::ostream & flux, PageRank & pageRank);