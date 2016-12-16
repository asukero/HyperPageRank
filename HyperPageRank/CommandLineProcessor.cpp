#include "CommandLineProcessor.h"
#include <string>
#include <iostream>

using namespace std;

void CommandLineProcessor::run()
{
	PageRankComputer pageRankComputer;
	Graph<WebPage> webGraph;
	Hypergraph<WebPage> webHypergraph;
	cout << "\tCalcul du PageRank\n" << endl;
	cout << "Vous devez avoir deux fichiers .txt, le premier pour les noeuds du graphe, le deuxième pour les arcs.\n" << endl;
	cout << "Entrez le nom du fichier contenant les noeuds : " << endl;
	string nodeFileName;
	string edgeFileName;
	cin >> nodeFileName;
	cin.clear();
	cin.ignore(256, '\n');
	cout << "Entrez le nom du fichier contenant les arcs : " << endl;
	cin >> edgeFileName;
	bool fileFound = false;
	
	while (!fileFound)
	{
		
		try
		{
			cout << "Creation du graphe simple et de l'hypergraphe..." << endl;
			pageRankComputer.loadGraphAndHypergraph(webGraph, webHypergraph, nodeFileName, edgeFileName);
			//cout << "Creation du graphe simple..." << endl;
			//webGraph = pageRankComputer.loadGraph(nodeFileName, edgeFileName);
			//cout << "Creation de l'hypergraphe..." << endl;
			// Possible to change it for a pointer, avoiding the copy problem explained in PageRankComputer
			//webHypergraph = pageRankComputer.loadHypergraph(nodeFileName, edgeFileName);
			fileFound = true;
		}
		catch (...)
		{
			cout << "\nErreur le fichier de noeuds ou d'arc n'a pas ete trouve." << endl;
			cout << "Entrez le nom du fichier contenant les noeuds : " << endl;
			cin.clear();
			cin.ignore(256, '\n');
			cin >> nodeFileName;
			cout << "Entrez le nom du fichier contenant les arcs : " << endl;
			cin >> edgeFileName;
		}

	}

	displayCommands();
	cin.ignore();
	readCommand(webGraph, webHypergraph, pageRankComputer);
}

void CommandLineProcessor::displayCommands()
{
	cout << "---------- Les commandes possibles sont les suivantes ----------\n" << endl;
	cout << "- help, ? : affiche la liste des commandes" << endl;
	cout << "- pagerank : calcul le PageRank pour le WebGraph simple" << endl;
	cout << "- hyperpagerank: calcul le PageRank pour le Web Hypergraph" << endl;
	cout << "- recherche [i] : recherche une page web selon son id." << endl;
	cout << "- quitter : quitte le programme.\n" << endl;
}

void CommandLineProcessor::readCommand(Graph<WebPage> & webGraph, Hypergraph<WebPage> & webHypergraph, PageRankComputer & pageRankComputer) const
{
	cout << "commande : ";
	string command;
	getline(cin, command);
	PageRank pageRank;
	PageRank hyperPageRank;
	if (command == "help" || command == "?")
	{
		displayCommands();
	}
	else if (command == "pagerank")
	{
		pageRank = pageRankComputer.computePageRank(webGraph, true);
		cout << "PageRank : " << endl << pageRank << endl;
	}
	else if (command == "hyperpagerank")
	{
		//hyperPageRank = pageRankComputer.computeHyperPageRank(webHypergraph, true);
		//cout << "HyperPageRank : " << endl << hyperPageRank << endl;
	}
	else if (command == "recherche")
	{
		//webGraph.searchNode(node)
		//cout << node << endl;
	}
	else if (command == "quitter")
	{
		return;
	}
	else
	{
		cout << "commande non trouvee, veuillez reessayer ou entrez 'help' ou '?' pour afficher la liste des commandes" << endl;
	}
	readCommand(webGraph, webHypergraph, pageRankComputer);
}
