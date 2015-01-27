#include "meter.h" 

template <class Measure, class Unit>
Meter <Measure, Unit>:: Meter (std::function <Measure (void)> f) 
  : _f (f)
  , _start (Measure ())
  , _end (Measure ()) {}
  
template <class Measure, class Unit>
void Meter <Measure, Unit>:: start () {
  _start = _f();
}

template <class Measure, class Unit>
void Meter <Measure, Unit>:: stop () {
  _end = _f();
}

template <class Measure, class Unit>
Unit Meter <Measure, Unit>:: peak () {
  return _end - _start;
}

template <class Measure, class Unit>
void Meter <Measure, Unit>:: reset () {
  _start = Measure ();
  _end = Measure ();
}

template <typename T>
double calculateMean (const std::vector<T> & x) {
  if (x.size() == 0) 
    throw std::invalid_argument ("Error: Mean of an empty vector is not defined!");
  
  if (! std::is_arithmetic <T>:: value)
    throw std::invalid_argument ("Error: Cannot calculate mean of non arithmetic type.");
  
  double sum = 0.0; 
  for (auto it = x.begin(); it != x.end(); ++it) 
    sum += (*it);
  
  return sum / x.size();
}

template <typename T>
double calculateVar (const std::vector<T> & x) {
  if (x.size() == 0)
    throw std::invalid_argument ("Error: Variance of an empty vector is not defined!");
  
  if (! std::is_arithmetic <T>:: value)
    throw std::invalid_argument ("Error: Cannot calculate variance of non arithmetic type.");
  
  double var = 0.0;
  double mean = calculateMean <T> (x);
  
  for (auto it = x.begin(); it != x.end(); ++it) 
    var += ((*it) - mean) * ((*it) - mean);
  
  return var / x.size();
}

template <typename T>
void writeMeasurements (const std::string & oFilename
		      , const std::string & header
// 		      , const std::string & format
		      , const std::vector <uint> & parameterIdentifier 
		      , const Matrix <T> measurements) {
  // Test whether the output file exists
  auto oFile = std::fopen (oFilename.c_str(), "r");
  
  if (oFile) {
    std::printf ("Warning: %s already exists. No measurements has been written out.\n", oFilename.c_str());
    std::fclose (oFile);
    return; 
  }
  
  oFile = std::fopen (oFilename.c_str(), "w");
  if (! oFile) 
    throw std::runtime_error ("Error: Cannot create output file %s." + oFilename);
  
  // Write header to the output file
  std::fprintf (oFile, header.c_str());

  if (measurements.isEmpty()) {
    std::fclose (oFile);
    return;
  }
    
  Matrix <double> normMeasurements = normalizeMeasurements (measurements);
    
  if (! (parameterIdentifier.size() == measurements.rows()))
    throw std::invalid_argument ("Error: For all parameter sets are measurements needed.");
  
  // Extract statistics from the measurements & write to the file
  for (uint i = 0; i < normMeasurements.rows(); i++) {
    auto measurement (normMeasurements.getRow (i));
    
    double min = (*std::min_element (measurement.begin(), measurement.end()));
    double max = (*std::max_element (measurement.begin(), measurement.end()));
    
    double mean = calculateMean (measurement);
    double sd   = std::sqrt (calculateVar (measurement));
    
    std::fprintf (oFile, "# %2u %10.3f %10.3f %10.3f %10.3f ", parameterIdentifier[i], min, max, mean, sd);
    
    for (uint run = 0; run < measurement.size (); run++) 
      std::fprintf (oFile, "%10.3f ", measurement[run]);
    
    std::fprintf (oFile, "\n");
  }
  
  std::fclose (oFile);
  
  return;
}

template <typename Unit, typename Measure, typename T, typename... Args>
std::vector <Unit> benchmark (std::function <Measure (void)> measuringFunction
			    , uint nRuns
			    , std::function <T (Args...)> algorithm
			    , Args... algorithmParameter) {
  
  std::vector <Unit> measurements (nRuns);
  
  Meter <Measure, Unit> meter (measuringFunction);
  
  for (uint run = 0; run < nRuns; run++) {
    meter.start();
    algorithm (algorithmParameter...);
    meter.stop();
    measurements[run] = meter.peak();
  }
  
  return measurements;
}

// template spezialization
template <>
Matrix <double> normalizeMeasurements (const Matrix <timeDuration> & measurement);
template <>
Matrix <double> normalizeMeasurements (const Matrix <uint64_t> & measurement);
