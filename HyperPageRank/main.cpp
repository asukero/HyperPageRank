#include <iostream>
#include <string>
#include "Node.hpp"
#include "Matrix.hpp"
#include "Graph.hpp"
#include "Hypergraph.hpp"
#include "PageRankComputer.hpp"
#include "CommandLineProcessor.h"


int main() {
	
	/*std::cout << "------------------------- Tests Noeuds -------------------------" << std::endl;
	std::string s1 = "Noeud1";
	Node<std::string> n1(s1);
	std::cout << n1 << std::endl;
	int i1 = 10;
	Node<int> n2(i1);
	std::cout << n2 << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "------------------------- Tests Graphes ------------------------" << std::endl;
	std::cout << "Insert tests" << std::endl;
	Graph<int> graph;
	std::cout << graph.addNode(10) << std::endl;
	std::cout << graph.addNode(2) << std::endl;
	std::cout << graph.addNode(4) << std::endl;
	std::cout << graph.addNode(8) << std::endl;
	std::cout << graph.addNode(100) << std::endl;
	std::cout << graph.addNode(0) << std::endl;
	std::cout << "Search tests" << std::endl;
	std::cout << graph.searchNode(2) << std::endl;
	std::cout << graph.searchNode(0) << std::endl;
	std::cout << graph.searchNode(40) << std::endl;
	std::cout << "Arcs tests" << std::endl;
	std::cout << graph.addArc(10, 2) << std::endl;
	std::cout << graph.addArc(10, 8) << std::endl;
	std::cout << graph.addArc(4, 100) << std::endl;
	std::cout << graph.addArc(-1, 10) << std::endl;
	std::cout << graph.addArc(0, 0) << std::endl;
	std::vector<long> list = graph.getPointedNodes(0);
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "------------------------- Tests Matrice ------------------------" << std::endl;
	Matrix<int> m(3, 3, 0);
	m.setValue(0, 0, 1);
	m.setValue(0, 1, 2);
	m.setValue(0, 2, 3);
	m.setValue(1, 0, 4);
	m.setValue(1, 1, 5);
	m.setValue(1, 2, 6);
	m.setValue(2, 0, 7);
	m.setValue(2, 1, 8);
	m.setValue(2, 2, 9);
	std::cout << m.getNbColumns() << std::endl;
	std::cout << m.getNbRows() << std::endl;
	m.addRow(m.getNbRows());
	m.addColumn(m.getNbColumns());
	m.display();
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "---------------------- Tests Hypergraphes ----------------------" << std::endl;
	Hypergraph<int> hypergraph;
	std::cout << hypergraph.addNode(1) << std::endl;
	std::cout << hypergraph.addNode(2) << std::endl;
	std::cout << hypergraph.addNode(3) << std::endl;
	std::cout << hypergraph.addNode(4) << std::endl;
	std::cout << hypergraph.addNode(5) << std::endl;
	long indHA1 = hypergraph.addHyperArc(1, 2);
	long indHA2 = hypergraph.addHyperArc(2, 5);
	long indHA3 = hypergraph.addHyperArc(4, 2);
	std::cout << indHA1 << std::endl;
	std::cout << indHA2 << std::endl;
	std::cout << indHA3 << std::endl;
	std::cout << hypergraph.addNodeToHyperArc(indHA1, 3, true) << std::endl;
	std::cout << hypergraph.addNodeToHyperArc(indHA3, 5, true) << std::endl;
	hypergraph.getHyperarcMatrix().display();
	IndexList HA1origin = hypergraph.getPointingNodes(0);
	IndexList HA1destination = hypergraph.getPointedNodes(0);
	IndexList HA2origin = hypergraph.getPointingNodes(1);
	IndexList HA2destination = hypergraph.getPointedNodes(1);
	IndexList HA3origin = hypergraph.getPointingNodes(2);
	IndexList HA3destination = hypergraph.getPointedNodes(2);
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "-------------------- Tests PageRank (Graphe) -------------------" << std::endl;
	PageRankComputer computer;
	// %% First example %%
	Graph<std::string> graphPR1;
	graphPR1.addNode("A");
	graphPR1.addNode("B");
	graphPR1.addArc("A", "B");
	graphPR1.addArc("B", "A");
	PageRank pr1A = computer.computePageRank(graphPR1, true);
	PageRank pr1 = computer.computePageRank(graphPR1, false);
	std::cout << "PR1A :" << std::endl;
	std::cout << pr1A;
	std::cout << "PR1 :" << std::endl;
	std::cout << pr1;
	// %% Second example %%
	Graph<std::string> graphPR2;
	graphPR2.addNode("A");
	graphPR2.addNode("B");
	graphPR2.addNode("C");
	graphPR2.addNode("D");
	graphPR2.addArc("A", "B");
	graphPR2.addArc("B", "C");
	graphPR2.addArc("A", "C");
	graphPR2.addArc("C", "A");
	graphPR2.addArc("D", "C");
	PageRank pr2A = computer.computePageRank(graphPR2, true);
	PageRank pr2 = computer.computePageRank(graphPR2, false);
	std::cout << "PR2A :" << std::endl;
	std::cout << pr2A;
	std::cout << "PR2 :" << std::endl;
	std::cout << pr2;
	// %% Third example %%
	Graph<std::string> graphPR3;
	graphPR3.addNode("A");
	graphPR3.addNode("B");
	graphPR3.addNode("C");
	graphPR3.addNode("D");
	graphPR3.addArc("A", "B");
	graphPR3.addArc("B", "A");
	graphPR3.addArc("A", "C");
	graphPR3.addArc("C", "A");
	graphPR3.addArc("A", "D");
	graphPR3.addArc("D", "A");
	PageRank pr3A = computer.computePageRank(graphPR3, true);
	PageRank pr3 = computer.computePageRank(graphPR3, false);
	std::cout << "PR3A :" << std::endl;
	std::cout << pr3A;
	std::cout << "PR3 :" << std::endl;
	std::cout << pr3;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "----------------- Tests PageRank (Hypergraphe) -----------------" << std::endl;
	// %% Test on an the naive hypergraph version of the previous graph
	Hypergraph<std::string> hyperGraph1;
	hyperGraph1.addNode("A");
	hyperGraph1.addNode("B");
	hyperGraph1.addNode("C");
	hyperGraph1.addNode("D");
	hyperGraph1.addHyperArc("A", "B");
	hyperGraph1.addNodeToHyperArc(0, "C", false);
	hyperGraph1.addHyperArc("B", "C");
	hyperGraph1.addHyperArc("C", "A");
	hyperGraph1.addHyperArc("D", "C");
	hyperGraph1.getHyperarcMatrix().display();
	PageRank HP1 = computer.computeHyperPageRank(hyperGraph1);
	std::cout << "HyperPageRank (HG1):" << std::endl;
	std::cout << HP1;
	// %% Another exemple
	Hypergraph<std::string> hyperGraph2;
	std::cout << hyperGraph2.addNode("1") << std::endl;
	std::cout << hyperGraph2.addNode("2") << std::endl;
	std::cout << hyperGraph2.addNode("3") << std::endl;
	std::cout << hyperGraph2.addNode("4") << std::endl;
	std::cout << hyperGraph2.addNode("5") << std::endl;
	hyperGraph2.addHyperArc("1", "2");
	hyperGraph2.addHyperArc("2", "5");
	hyperGraph2.addHyperArc("4", "2");
	hyperGraph2.addNodeToHyperArc(indHA1, "3", true);
	hyperGraph2.addNodeToHyperArc(indHA3, "5", true);
	hyperGraph2.getHyperarcMatrix().display();
	PageRank HP2 = computer.computeHyperPageRank(hyperGraph2);
	std::cout << "HyperPageRank (HG2):" << std::endl;
	std::cout << HP2;
	std::cout << "----------------------------------------------------------------" << std::endl;

	std::cout << "------------------------ Tests Indegree ------------------------" << std::endl;
	PageRank ID1 = computer.computeIndegree(graphPR1);
	std::cout << ID1 << std::endl;
	PageRank ID2 = computer.computeIndegree(graphPR2);
	std::cout << ID2 << std::endl;
	PageRank ID3 = computer.computeIndegree(graphPR3);
	std::cout << ID3 << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;
	std::cout << "---------------------- Tests HyperIndegree ---------------------" << std::endl;
	PageRank HID1 = computer.computeHyperIndegree(hyperGraph1);
	std::cout << HID1 << std::endl;
	PageRank HID2 = computer.computeHyperIndegree(hyperGraph2);
	std::cout << HID2 << std::endl;
	std::cout << "----------------------------------------------------------------" << std::endl;*/
	Graph<WebPage> graphe;
	WebPage p0(0);
	WebPage p1(1);
	WebPage p2(2);
	WebPage p3(3);
	graphe.addNode(p0);
	graphe.addNode(p1);
	graphe.addNode(p2);
	graphe.addNode(p3);
	/*graphe.addArc(p0, p1);
	graphe.addArc(p1, p2);
	graphe.addArc(p0, p2);
	graphe.addArc(p2, p0);
	graphe.addArc(p3, p2);*/
	graphe.addArc(0, 1);
	graphe.addArc(0, 2);
	graphe.addArc(1, 2);
	graphe.addArc(2, 0);
	graphe.addArc(3, 2);
	Hypergraph<WebPage> hypergraph;
	WebPage p4(4);
	WebPage p5(5);
	hypergraph.addNode(p1);
	hypergraph.addNode(p2);
	hypergraph.addNode(p3);
	hypergraph.addNode(p4);
	hypergraph.addNode(p5);
	/*long indHA1 = hypergraph.addHyperArc(0, 1, false);
	long indHA2 = hypergraph.addHyperArc(1, 4, false);
	long indHA3 = hypergraph.addHyperArc(3, 1, false);
	hypergraph.addNodeToHyperArc(indHA1, 2, true);
	hypergraph.addNodeToHyperArc(indHA3, 4, true);*/
	long indHA1 = hypergraph.addHyperArc(0, 2, true);
	long indHA2 = hypergraph.addHyperArc(3, 4, true);
	long indHA3 = hypergraph.addHyperArc(1, 4, false);
	hypergraph.addNodeToHyperArc(indHA1, 1, false);
	hypergraph.addNodeToHyperArc(indHA2, 1, false);
	hypergraph.getHyperarcMatrix().display();
	//COMMANDLINE PROCESSOR
	CommandLineProcessor cmd;
	cmd.run();
	system("PAUSE");
	return EXIT_SUCCESS;
}
