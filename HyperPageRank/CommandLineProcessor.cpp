#include "CommandLineProcessor.h"
#include <string>
#include <iostream>

using namespace std;

void CommandLineProcessor::run()
{
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
			cout << "Creation du graphe simple..." << endl;
			webGraph = pageRankComputer.loadGraph(nodeFileName, edgeFileName);
			cout << "Creation de l'hypergraphe..." << endl;
			webHypergraph = pageRankComputer.loadHypergraph(nodeFileName, edgeFileName);
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
	readCommand();
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

void CommandLineProcessor::readCommand() const
{
	cout << "commande : ";
	string command;
	getline(cin, command);

	if (command == "help" || command == "?")
	{
		displayCommands();

	}
	else if (command == "pagerank")
	{
		//pageRankComputer.computePageRank(webGraph, true);
	}
	else if (command == "hyperpagerank")
	{
		//pageRankComputer.computeHyperPageRank(webHypergraph, true);
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
	readCommand();
}
