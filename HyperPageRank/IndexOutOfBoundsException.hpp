#pragma once
/*
* File:   IndexOutOfBoundsException.h
* Author: CATTO Guillaume, DUPLOYEZ Clément, FOSSATI Thomas
*
* Created on March 10, 2014, 2:39 PM
*/

class IndexOutOfBoundsException {
private:
	int index;
public:
	IndexOutOfBoundsException(int);
	void print();
	~IndexOutOfBoundsException() throw();
};