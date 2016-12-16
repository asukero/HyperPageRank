#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <unordered_map>
#include "Node.hpp"

typedef std::vector<long> IndexList;

/* Hypothesis :
- The nodes are all created first, and then the arcs are created to link nodes
- The content of a node is unique in the graph, it acts like an ID
*/
template<typename T>
class Graph {
public:
	// Constructor
	Graph();
	// Destructor
	~Graph();
	// Creates and adds a node to the graph from its content, returns its position in the list
	long addNode(T & content);
	// Adds an arc from the content of two nodes, returns the position of the first node in the list,
	// -1 if the arc could not be created
	long addArc(T & originContent, T & destinationContent);
	// Searches a node according to its content, returns its index if found, else -1
	long searchNode(T & content);
	// Returns a reference to the index list of the nodes pointed by the one with the index given as parameter
	IndexList & getPointedNodes(const long & index);
	// Returns a reference to the node list
	std::vector<Node<T>> & getNodeList();
	// Returns a reference to the adjacency list
	std::unordered_map<long, IndexList> & getArcList();
private:
	// List of the nodes
	std::vector<Node<T>> nodeList;
	// Adjacency list
	std::unordered_map<long, IndexList> arcList;
};

template<typename T>
Graph<T>::Graph() {
	this->nodeList.clear();
}

template<typename T>
Graph<T>::~Graph() {
}

template<typename T>
long Graph<T>::addNode(T & content) {
	nodeList.emplace_back(content);
	return nodeList.size() - 1;
}

template<typename T>
long Graph<T>::addArc(T & originContent, T & destinationContent) {
	// Search the nodes in the list
	long originIndex = this->searchNode(originContent);
	long destinationIndex = this->searchNode(destinationContent);
	// Creates the arc, adds it to the adjacency list
	if (originIndex != -1 && destinationIndex != -1) {
		this->arcList[originIndex].push_back(destinationIndex);
		return originIndex;
	}
	return -1;
}

template<typename T>
long Graph<T>::searchNode(T & content) {
	long index = -1;
	for (long i = 0; i < this->nodeList.size(); i++) {
		// If the node has the good content
		if (this->nodeList.at(i).getContent() == content) {
			return i;
		}
	}
	return index;
}

template<typename T>
IndexList & Graph<T>::getPointedNodes(const long & index) {
	// If the index is not out of bounds
	if (index < 0 || index < this->nodeList.size()) {
		return this->arcList[index];
	}
	return std::vector<long>();
}

template<typename T>
std::vector<Node<T>>& Graph<T>::getNodeList() {
	return this->nodeList;
}

template<typename T>
std::unordered_map<long, IndexList>& Graph<T>::getArcList() {
	return this->arcList;
}