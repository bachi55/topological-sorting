#ifndef METER_H
#define METER_H

#include "matrix.h"
#include "mytypes.h"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>

// TODO: How to pass arguments to _f, which could be void, without pulting the interface?
template <class Measure, class Unit>
class Meter {
  std::function <Measure (void)> _f;
  
  Measure _start, _end; 
  
public:
  // constructor
  Meter (std::function <Measure (void)> f);
  
  // start measurement
  void start ();
  // stop measurement
  void stop ();
  // gives the elapsed measurement
  Unit peak ();
  // resets the Meter
  void reset ();

};

// ---------------------------------------------------------------------
// TIME
// ---------------------------------------------------------------------

// Function to return the current time of the system_clock in microseconds.
timePoint myClock (void);

// ---------------------------------------------------------------------
// CYCLES
// --------------------------------------------------------------------- 
  
// This function calls the 'cpuid' instructions (in assembler) and returns the result.
// 
// A side effect of the 'cpuid' instruction is the serialization of the cpu.
Register cpuid (uint eax);

// This function calls the 'cpuid' instructions (in assembler) and returns it's result.
//
Register cpuid (uint eax, uint ebx, uint ecx, uint edx);

// This function checks, whether the current CPU supports the 'cpuid' instruction
//
// TODO: implement this function
inline bool hasCPUID (void) { return true; };

// This functions reads the cpu instruction time stamp and serializes the cpu 
//
// NOTE: only instructions before are prevent to move behind 'rdtscp'
uint64_t rdtscp (void);

// This function checks, whether the current CPU supports the 'rdtscp' instruction
// 
// TODO: implement this function
inline bool hasRDTSCP (void) { return true; };

// This functions reads the cpu instruction time stamp
uint64_t rdtsc (void);

// This function checks, whether the current CPU supports the 'rdtsc' instruction
// 
// TODO: implement this function
inline bool hasRDTSC (void) { return true; };


// This function return the cpu instruction time stamp.
//
// Before the time stamp is read, the cpu will be serialized using the 'cpuid' command.
uint64_t myCycles (void);

// This function can write out measurements
//
// NOTE: parameterIdentifier identifies a certain set of parameters
//
// It can be used in the follwing way: 
// 1) run your algorithm each parameter set (lets say of size N) M-times
// 2) store your measurements into a matrix (N times M)
// 3) call this function with an appropriate name
// TODO: Realize with string streams!
template <typename T>
void writeMeasurements (const std::string & oFilename
		      , const std::string & header
		      , const std::vector <uint> & parameterIdentifier 
		      , const Matrix <T> measurements);

// This function can benchmark a given algorithm using a given measure
template <typename Unit, typename Measure, typename T, typename... Args>
std::vector <Unit> benchmark (std::function <Measure (void)> measuringFunction
			    , uint nRuns
			    , std::function <T (Args...)> algorithm
			    , Args... algorithmParameter);

// This function can create a string out of a measurement unit
template <typename Unit>
Matrix <double> normalizeMeasurements (const Matrix <Unit> & measurements) {
  
  std::printf ("Sorry! There is no \'normalizeMeasurements\' implementation for type T right now.\n");
  
};

// This function is dummy function to test the variadic templates
template <typename T, typename... Args>
T execute (const std::function  <T (Args...)> f, Args... parameter) {
  
  return f (parameter...);
}

// NOTE: Never, Never, Never ... forget this again!!!!!
#include "meter.tpp"

#endif