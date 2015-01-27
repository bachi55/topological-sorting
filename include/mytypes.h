#ifndef MYTYPES_H
#define MYTYPES_H

#include <chrono>

typedef std::chrono::system_clock::time_point			timePoint ;
typedef std::chrono::microseconds 				microseconds;
typedef std::chrono::nanoseconds 				nanoseconds;
typedef std::chrono::duration <long, std::ratio<1,1000000000>> 	timeDuration;

// type to store the return values of 'cpuid'
struct Register {
  Register (uint eax, uint ebx, uint ecx, uint edx) 
    : _eax (eax)
    , _ebx (ebx)
    , _ecx (ecx)
    , _edx (edx) {};
    
  uint _eax;
  uint _ebx;
  uint _ecx;
  uint _edx;
};

#endif