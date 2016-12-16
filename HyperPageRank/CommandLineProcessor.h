#pragma once
#include "PageRankComputer.hpp"

class CommandLineProcessor
{
/*private:
	PageRankComputer pageRankComputer;
	Graph<WebPage> webGraph;
	Hypergraph<WebPage> webHypergraph;*/
public:
	void run();
	static void displayCommands();
	void readCommand(Graph<WebPage> & webGraph, Hypergraph<WebPage> & webHypergraph, PageRankComputer & pageRankComputer) const;
};