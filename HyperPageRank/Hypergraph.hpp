#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include "Node.hpp"
#include "Matrix.hpp"

typedef std::vector<long> IndexList;

template<typename T>
class Hypergraph {
public:
	// Constructor
	Hypergraph();
	// Destructor
	~Hypergraph();
	// Creates and adds a node to the graph from its content, returns its position in the list
	long addNode(T & content);
	// Creates an hyperarc between two given nodes
	long addHyperArc(T & originContent, T & destinationContent);
	// Add a node to the origin or destination set of an hyperarc, from its index
	long addNodeToHyperArc(const long & arcIndex, T & nodeContent, bool origin);
	// Searches a node according to its content, returns its index if found, else -1
	long searchNode(T & content);
	// Returns the index list of the nodes pointed by the hyperarc, given its index in the matrix
	IndexList getPointedNodes(const long & index);
	// Returns the index list of the origin nodes of the hyperarc, given its index in the matrix
	IndexList getPointingNodes(const long & index);
	// Returns a reference to the node list
	std::vector<Node<T>> & getNodeList();
	// Returns a reference to the hyperarc matrix
	Matrix<int> & getHyperarcMatrix();
private:
	// List of the nodes
	std::vector<Node<T>> nodeList;
	// Matrix representing the hyperarcs
	Matrix<int> hyperarcMatrix;
};

template<typename T>
Hypergraph<T>::Hypergraph() : hyperarcMatrix(0,0,0) {
	this->nodeList.clear();
}

template<typename T>
Hypergraph<T>::~Hypergraph() {
}

template<typename T>
long Hypergraph<T>::addNode(T & content) {
	// Adds the node to the node list
	this->nodeList.emplace_back(content);
	// Adds the node to the hyperarc matrix <=> adds a row
	this->hyperarcMatrix.addRow(this->hyperarcMatrix.getNbRows());
	// Return the index of the node ie its index in the node list and its row in the arc matrix
	return this->nodeList.size() - 1;
}

template<typename T>
long Hypergraph<T>::addHyperArc(T & originContent, T & destinationContent) {
	// Search the nodes in the list
	long originIndex = this->searchNode(originContent);
	long destinationIndex = this->searchNode(destinationContent);
	// Creates the hyperarc, adds it to the list
	if (originIndex != -1 && destinationIndex != -1) {
		long hyperarcIndex = this->hyperarcMatrix.getNbColumns();
		this->hyperarcMatrix.addColumn(hyperarcIndex);
		// Sets the values in the matrix : 1 <=> node is pointed ; -1 <=> node is the origin of an hyperarc
		this->hyperarcMatrix.setValue(originIndex, hyperarcIndex, -1); 
		this->hyperarcMatrix.setValue(destinationIndex, hyperarcIndex, 1);
		return hyperarcIndex;
	}
	return -1;
}

template<typename T>
long Hypergraph<T>::addNodeToHyperArc(const long & arcIndex, T & nodeContent, bool origin) {
	int value = 1;
	if (origin) { value = -1; }
	// Search the node in the list
	long nodeIndex = this->searchNode(nodeContent);
	// If the node exist, and an hyperarc is already present in the matrix at the given index
	if (nodeIndex != -1 && arcIndex < this->hyperarcMatrix.getNbColumns()) {
		this->hyperarcMatrix.setValue(nodeIndex, arcIndex, value);
		return arcIndex;
	}
	return -1;
}

template<typename T>
long Hypergraph<T>::searchNode(T & content) {
	long index = -1;
	for (long i = 0; i < this->nodeList.size(); i++) {
		// If the node has the good content
		if (this->nodeList.at(i).getContent() == content) {return i;}
	}
	return index;
}

template<typename T>
IndexList Hypergraph<T>::getPointedNodes(const long & index) {
	IndexList list;
	list.clear();
	for (long i = 0; i < this->hyperarcMatrix.getNbRows(); i++) {
		// If M(i,j) = 1
		if (this->hyperarcMatrix.getValue(i, index) == 1) { list.push_back(i); }
	}
	return list;
}

template<typename T>
IndexList Hypergraph<T>::getPointingNodes(const long & index) {
	IndexList list;
	list.clear();
	for (long i = 0; i < this->hyperarcMatrix.getNbRows(); i++) {
		// If M(i,j) = -1
		if (this->hyperarcMatrix.getValue(i, index) == -1) { list.push_back(i); }
	}
	return list;
}

template<typename T>
std::vector<Node<T>>& Hypergraph<T>::getNodeList() {
	return this->nodeList;
}

template<typename T>
Matrix<int>& Hypergraph<T>::getHyperarcMatrix() {
	return this->hyperarcMatrix;
}