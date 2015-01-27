#include "matrix.h"

//TODO: Try to use type_traits (header) : is_arithmetic (function) to print out.
template <>
void Matrix <ulong> ::printMatrix ()
{
  
  for (auto it = _data.begin(); it != _data.end(); ++it) {
    // print newline after a row has been printed
    if (((it - _data.begin()) % _ncol == 0) && (it != _data.begin())) 
      std::printf ("\n");
    
    std::printf ("%lu ", (*it));
  }
  
  std::printf ("\n");
  
}

template <>
void Matrix <float> ::printMatrix ()
{
  
  for (auto it = _data.begin(); it != _data.end(); ++it) {
    // print newline after a row has been printed
    if (((it - _data.begin()) % _ncol == 0) && (it != _data.begin())) 
      std::printf ("\n");
    
    std::printf ("%f ", (*it));
  }
  
  std::printf ("\n");
  
}

template <>
void Matrix <timeDuration> ::printMatrix ()
{
  
  for (auto it = _data.begin(); it != _data.end(); ++it) {
    // print newline after a row has been printed
    if (((it - _data.begin()) % _ncol == 0) && (it != _data.begin())) 
      std::printf ("\n");
    
    std::printf ("%lu ", std::chrono::duration_cast <nanoseconds> ((*it)).count());
  }
  
  std::printf ("\n");
  
}