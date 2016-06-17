#ifndef StiScanPrgOptions_h
#define StiScanPrgOptions_h

#include <set>
#include <string>

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include "TChain.h"

#include "StiRootIO/PrgOptionProcessor.h"


namespace {
   namespace po = boost::program_options;
}


/**
 * Processes and controls user options provided in the command line.
 */
class StiScanPrgOptions : public PrgOptionProcessor
{
public:

   StiScanPrgOptions();
   StiScanPrgOptions(int argc, char **argv, const std::string& stiTreeName="t", const std::string& geantStepTreeName="stepping");
   ~StiScanPrgOptions();

   bool  DoGeantStepTree() const { return fDoGeantStepTree; }
   bool  DoAutoHistRange() const { return fDoAutoHistRange; }
   TChain* GetGeantStepChain() { return fGeantStepChain; }
   double GetHistZMin() const { return fHistZMin; }
   double GetHistZMax() const { return fHistZMax; }
   double GetHistRMin() const { return fHistRMin; }
   double GetHistRMax() const { return fHistRMax; }
   void SetHistZRange(double minZ, double maxZ) { fHistZMin = minZ; fHistZMax = maxZ; }
   void SetHistRMax(double maxR) { fHistRMax = maxR; }

protected:

   virtual void VerifyOptions();
   virtual void AddToInputChains(std::string stiTreeRootFileName);

   bool    fDoGeantStepTree; ///< A flag to process geant tree if set
   bool    fDoAutoHistRange; ///< A flag to find optimal range for histogram axises automatically
   TChain *fGeantStepChain;
   double  fHistZMin;
   double  fHistZMax;
   double  fHistRMin;
   double  fHistRMax;
};

#endif
