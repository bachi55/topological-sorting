#include "meter.h"

template <>
Matrix <double> normalizeMeasurements (const Matrix <timeDuration> & measurements) {
  Matrix <double> normMeasurements (measurements.rows(), measurements.cols(), 0);
  
  for (uint i = 0; i < measurements.rows(); i++) for (uint j = 0; j < measurements.cols(); j++) {
    normMeasurements(i,j) = double (std::chrono::duration_cast <nanoseconds> (measurements(i,j)).count());
    normMeasurements(i,j) /= 1000.0;
  }
  
  return normMeasurements;
}

template <>
Matrix <double> normalizeMeasurements (const Matrix <uint64_t> & measurements) {
  Matrix <double> normMeasurements (measurements.rows(), measurements.cols(), 0);
  
  for (uint i = 0; i < measurements.rows(); i++) for (uint j = 0; j < measurements.cols(); j++) 
    normMeasurements(i,j) = double (measurements(i,j));
  
  return normMeasurements;
}

timePoint myClock (void) { 
  return std::chrono::system_clock::now(); 
}

Register cpuid (uint eax) {
  if (!hasCPUID ())
    throw std::runtime_error ("Error: No CPUID instruction avaivable on the current CPU.");
  
  uint ebx, ecx, edx;
  
  __asm__ __volatile__ 
	  (
	    "mov %0, %%eax\n"	// move the first input operant to 'eax'
	    "cpuid"		// run 'cpuid', results will be in e{a,b,c,d}x
	    : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) // output operants
	    : "a"(eax)					 // input operants
// 	    : "%rax", "%rbx", "%rcx", "%rdx"		 // TODO: take a closer look to CLOBBERLISTS!
	  );
 
  return Register (eax, ebx, ecx, edx);
}

Register cpuid (uint eax, uint ebx, uint ecx, uint edx) {
  if (!hasCPUID ())
    throw std::runtime_error ("Error: No CPUID instruction avaivable on the current CPU.");
  
  __asm__ __volatile__ 
          (
            "mov %0, %%eax\n"                            // move the first input operant to 'eax'
            "cpuid"                                      // run 'cpuid', results will be in e{a,b,c,d}x
            : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) // output operants
            : "a"(eax), "c"(ecx)                         // input operants
          );
  
  return Register (eax, ebx, ecx, edx);
}

uint64_t rdtscp (void) {
  if (!hasRDTSCP ())
    throw std::runtime_error ("Error: No RDTSCP instruction avaivable on the current CPU");
  
  uint lo, hi;
  
  __asm__ __volatile__
	  (
	    "rdtscp"
	    : "=a" (lo), "=d" (hi)	// output operants, 'rdtscp' writes the results into the 'eax' and 'edx' register
// 	    :: "%rax", "%rcx", "%rdx"
	  );
	  
  return ((uint64_t)hi << 32) | lo;
}

uint64_t rdtsc (void) {
  if (!hasRDTSC ())
    throw std::runtime_error ("Error: No RDTSC instruction avaivable on the current CPU");
  
  uint lo, hi;
  
  __asm__ __volatile__
	  (
	    "rdtsc"
	    : "=a" (lo), "=d" (hi)	// output operants, 'rdtscp' writes the results into the 'eax' and 'edx' register
	  );
	  
  return ((uint64_t)hi << 32) | lo;
}

uint64_t myCycles ( void ) {
  cpuid (0);
  return rdtsc();
}

