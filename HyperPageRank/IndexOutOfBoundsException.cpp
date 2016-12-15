/*
* File:   IndexOutOfBoundsException.cpp
* Author: CATTO Guillaume, DUPLOYEZ Clément, FOSSATI Thomas
*
* Created on
*/

#include <iostream>

#include "IndexOutOfBoundsException.hpp"

IndexOutOfBoundsException::IndexOutOfBoundsException(int i) {
	//std::cout << "Building the Index Out Of Bounds Exception at this: " << this << std::endl;
	this->index = i;
}

void IndexOutOfBoundsException::print() {
	std::cerr << "/!\\ -------- Index out of bounds exception! Index given is: " << this->index << " -------- /!\\" << std::endl;
}

IndexOutOfBoundsException::~IndexOutOfBoundsException() throw() {
	//std::cout << "Destroying the Index Out Of Bounds Exception at this: " << this << std::endl;
}