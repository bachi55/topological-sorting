#include "matrix.h"

template <class T>
Matrix <T> ::Matrix ()
  : Matrix (0, 0, std::vector <T> ()) {}
  
template <class T>
Matrix <T> ::Matrix (uint nrow, uint ncol, const T & initElement) 
  : _nrow (nrow)
  , _ncol (ncol)
  , _data (std::vector <T> (_nrow * _ncol, initElement)) 
  , _init (false) {}

template <class T>
Matrix <T> ::Matrix (uint nrow, uint ncol, std::vector<T>&& data) 
  : _nrow (nrow)
  , _ncol (ncol)
  , _data (data) {
    
  if ((_nrow * _ncol) != data.size())
    throw std::invalid_argument ("Error: matrix construction failed, nrow * ncol should be equal data.size");
    
  _init = true;
  
}

template <class T>
Matrix <T> ::Matrix (uint nrow, uint ncol, const std::vector<T>& data) 
  : Matrix (nrow, ncol, data) {}

template <class T>
void Matrix <T> ::printMatrix () {
  
  std::printf ("Sorry! There is no \'printMatrix\' implementation for type T right now.\n");
  
}

template <class T>
T& Matrix <T> ::operator() (uint row, uint col) {
  
  if ((row > (_nrow - 1)) || (col > (_ncol - 1)))
    throw std::invalid_argument ("Error: matrix index out of bounce.");
  
  return _data[row * _ncol + col];
  
}

template <class T>
const T & Matrix <T> ::operator() (uint row, uint col) const {
  
  if ((row > (_nrow - 1)) || (col > (_ncol - 1)))
    throw std::invalid_argument ("Error: matrix index out of bounce.");
  
  return _data[row * _ncol + col];
  
}


template <class T>
Matrix <T> Matrix <T> ::operator* (const Matrix <T> & rhs) {
  
  if ((*this)._ncol != rhs._nrow)
     throw std::invalid_argument ("Error: matrix dimension does not fit.");
  
  Matrix <T> res ((*this)._nrow, rhs._ncol, std::vector <T> ((*this)._nrow * rhs._ncol, 0));
  
  for (uint i = 0; i < (*this)._nrow; i++) 
    for (uint j = 0; j < rhs._ncol; j++)
      for (uint k = 0; k < rhs._nrow; k++) 
	res (i, j) += (*this) (i, k) * rhs (k, j);
      
  return res;
  
}

template <class T>
std::vector <T> Matrix <T> ::getRow (uint row) const {
  
  if (row > (_nrow - 1))
    throw std::invalid_argument ("Error: matrix index out of bounce.");
  
  return 
    std::vector <T> (_data.begin() + ((_ncol) * row), _data.begin() + ((_ncol) * row + _ncol));
  
}

template <class T>
void Matrix <T> ::setRow (uint row, const std::vector <T> & newData) {
  
  if (row > (_nrow - 1))
    throw std::invalid_argument ("Error: matrix index out of bounce.");
  
  if ( newData.size() != (_ncol))
    throw std::invalid_argument ("Error: New row does not have the correct size.");
  
  auto rowData 	    = _data.begin() + ((_ncol) * row);
  
  std::copy (newData.begin(), newData.end(), rowData);
  
}

template <class T>
void Matrix <T> ::setRow (uint row, std::vector <T> && newData) {
  
  if (row > (_nrow - 1))
    throw std::invalid_argument ("Error: matrix index out of bounce.");
  
  if ( newData.size() != (_ncol))
    throw std::invalid_argument ("Error: New row does not have the correct size.");
  
  auto rowData 	    = _data.begin() + ((_ncol) * row);
  
  std::move (newData.begin(), newData.end(), rowData);
  
}


template <typename T>
Matrix <T> createIdentityMatrix (uint nrow) {
  
  Matrix <T> E = Matrix <T> (nrow, nrow, T(0));
  
  for (uint i = 0; i < nrow; i++) E (i,i) = T(1);
  
  return E;
  
}

// Specializing a member function template need to be done in an .cpp file, since otherwise the
// template would need be anymore a template.
template <>
void Matrix <ulong> ::printMatrix ();

template <>
void Matrix <float> ::printMatrix ();

template <>
void Matrix <timeDuration> ::printMatrix ();


