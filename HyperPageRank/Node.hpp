#pragma once

#include <iostream>

template<typename T>
class Node {
public:
	// Constructor
	Node(const T & contenu);
	// Returns the content of the node
	T getContent();
private:
	// The node's content
	T content;
};

template<typename T>
Node<T>::Node(const T & content) : content(content) {
}

template<typename T>
T Node<T>::getContent() {
	return this->content;
}

template<typename T>
std::ostream & operator<<(std::ostream & flux, Node<T> & node) {
	flux << node.getContent();
	return flux;
}

template<typename T>
bool operator == (Node<T> & leftNode, Node<T> & rightNode) {
	return leftNode.getContent() == rightNode.getContent();
}

template<typename T>
bool operator < (Node<T> & leftNode, Node<T> & rightNode) {
	return leftNode.getContent() < rightNode.getContent();
}

template<typename T>
bool operator > (Node<T> & leftNode, Node<T> & rightNode) {
	return leftNode.getContent() > rightNode.getContent();
}