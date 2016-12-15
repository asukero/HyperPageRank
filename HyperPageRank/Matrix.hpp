#pragma once
/*
* File:   Matrix.h
* Authors: CATTO Guillaume, DUPLOYEZ Clément, FOSSATI Thomas
*
* Created on March 10, 2014, 2:38 PM
*/


#include <vector> // Vectors
#include <iostream>
#include <memory> // Smart pointers
#include "IndexOutOfBoundsException.hpp" // fichier à créer et chemin à changer selon l'organisation de votre projet
#include <iomanip> // For displaying tabulations correctly


template<class T> class Matrix {

public:

	// -------------- interdit de modifier cette partie -------------- //
	Matrix(int nbRows, int nbColumns, T emptyValue);

	T getEmptyValue();

	int getNbRows();
	void addRow(int rowIndex) throw(IndexOutOfBoundsException);
	void removeRow(int rowIndex) throw(IndexOutOfBoundsException);

	int getNbColumns();
	void addColumn(int colIndex) throw(IndexOutOfBoundsException);
	void removeColumn(int colIndex) throw(IndexOutOfBoundsException);

	T getValue(int rowIndex, int colIndex) throw(IndexOutOfBoundsException);
	void setValue(int rowIndex, int colIndex, T value) throw(IndexOutOfBoundsException);

	~Matrix();
	// --------------------------------------------------------------- //
	void display();

private:
	std::vector<std::vector<std::unique_ptr<T>>> data; // Array containing arrays
	int nbCols; // Number of columns
	T empty; // Default value when cell is null
};


template <class T> Matrix<T>::Matrix(int nbRows, int nbColumns, T emptyValue) // Creates a vector of length nbRows
{
	if (nbRows < 0) {
		throw(IndexOutOfBoundsException(nbRows));
	}

	if (nbColumns < 0) {
		throw(IndexOutOfBoundsException(nbColumns));
	}

	this->data.resize(nbRows);

	this->nbCols = nbColumns;
	this->empty = emptyValue;

	for (auto& row : this->data)
	{
		row.resize(nbColumns); // For each row, we resize the vector to nbColumns. Each cell contains a unique_ptr with default value nullptr.
	}

}


template <class T> T Matrix<T>::getEmptyValue()
{
	return this->empty;
}


template <class T> int Matrix<T>::getNbRows()
{
	return this->data.size(); //The vector data contains all rows
}


template <class T> void Matrix<T>::addRow(int rowIndex) throw(IndexOutOfBoundsException)
{
	if (rowIndex < 0 || rowIndex > getNbRows()) throw(IndexOutOfBoundsException(rowIndex));

	std::vector<std::unique_ptr<T>> row(getNbColumns());
	this->data.insert(this->data.begin() + rowIndex, std::move(row));
}


template <class T> void Matrix<T>::removeRow(int rowIndex) throw(IndexOutOfBoundsException)
{
	if (rowIndex < 0 || rowIndex >= getNbRows()) throw(IndexOutOfBoundsException(rowIndex));

	this->data.erase(this->data.begin() + rowIndex);
}


template <class T> int Matrix<T>::getNbColumns()
{
	return this->nbCols;
}


template <class T> void Matrix<T>::addColumn(int colIndex) throw(IndexOutOfBoundsException)
{
	if (colIndex < 0 || colIndex > getNbColumns()) throw(IndexOutOfBoundsException(colIndex));

	int nbRows = getNbRows();
	for (int i = 0; i < nbRows; i++)
	{
		this->data[i].insert(this->data[i].begin() + colIndex, nullptr);
	}

	this->nbCols++;
}


template <class T> void Matrix<T>::removeColumn(int colIndex) throw(IndexOutOfBoundsException)
{
	if (colIndex < 0 || colIndex >= getNbColumns()) throw(IndexOutOfBoundsException(colIndex));

	int nbRows = getNbRows();
	for (int i = 0; i < nbRows; i++)
	{
		this->data[i].erase(this->data[i].begin() + colIndex);
	}

	this->nbCols--;
}


template <class T> T Matrix<T>::getValue(int rowIndex, int colIndex) throw(IndexOutOfBoundsException)
{
	if (rowIndex < 0 || rowIndex >= getNbRows()) throw(IndexOutOfBoundsException(rowIndex));
	if (colIndex < 0 || colIndex >= getNbColumns()) throw(IndexOutOfBoundsException(colIndex));

	T * ptr = this->data[rowIndex][colIndex].get(); // Returns a pointer to the cell (to a unique_ptr). There's no allocation, thus no need for unique_ptr
	if (ptr == nullptr) return getEmptyValue();
	return *ptr;
}


template <class T> void Matrix<T>::setValue(int rowIndex, int colIndex, T value) throw(IndexOutOfBoundsException)
{
	if (rowIndex < 0 || rowIndex >= getNbRows()) {
		throw(IndexOutOfBoundsException(rowIndex));
	}

	if (colIndex < 0 || colIndex >= getNbColumns()) {
		throw(IndexOutOfBoundsException(colIndex));
	}

	if (value != getEmptyValue())
	{
		this->data[rowIndex][colIndex].reset(new T(value));
	}
	else
	{
		this->data[rowIndex][colIndex].reset(nullptr);
	}
}


template <class T> Matrix<T>::~Matrix()
{
	// Nothing to do, since unique_ptrs and vectors deallocate themselves
}

template <class T> void Matrix<T>::display()
{
	int rows = getNbRows();
	int columns = getNbColumns();
	for (int i = 0; i < rows; i++) {
		std::cout << i << ":";

		for (int j = 0; j < columns; j++) {
			std::cout << std::setw(8);
			std::cout << "[" << j << "]" << getValue(i, j);
		}
		std::cout << std::endl;
	}
}