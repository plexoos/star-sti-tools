#ifndef StiScanHistContainer_h
#define StiScanHistContainer_h

#include <map>
#include <set>
#include <string>

#include "TDirectoryFile.h"
#include "TProfile2D.h"
#include "TProfile3D.h"

#include "StiScan/StiScanEvent.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiRootIO/Profile2D.h"
#include "StiRootIO/Profile3D.h"
#include "StiRootIO/TStiKalmanTrack.h"
#include "GeaRootIO/TGeaEvent.h"

typedef std::map<std::string, TH1*>                    HistMap;
typedef std::map<std::string, TH1*>::iterator          HistMapIter;
typedef std::map<std::string, TH1*>::const_iterator    HistMapConstIter;


class StiScanHistContainer : public TDirectoryFile
{
public:

   StiScanHistContainer(StiScanPrgOptions& prgOpts);
   StiScanHistContainer(StiScanPrgOptions& prgOpts, const char* name, TDirectory* motherDir = 0, bool doProjection=true, Option_t* option = "");
   ~StiScanHistContainer();

   void FillHists(const StiScanEvent &eventT, const std::set<std::string> *volumeList=0);
   void FillHists(const TGeaEvent &eventG, const std::set<std::string> *volumeList=0);
   void FillDerivedHists();
   void SaveAllAs(std::string prefix="./");
   void SetZRange(double minZ, double maxZ) { mNodeZMin = minZ; mNodeZMax = maxZ; }
   double GetZMin() { return mNodeZMin; }
   double GetZMax() { return mNodeZMax; }
   void SetRRange(float minR, float maxR) { mNodeRMin = minR; mNodeRMax = maxR; }
   float GetRMin() const { return mNodeRMin; }
   float GetRMax() const { return mNodeRMax; }

protected:

   void BookHists();
   virtual void FillHists(const TStiKalmanTrack &kalmTrack, const std::set<std::string> *volumeList=0);
   void FillHists(const TGeaTrack &trackG, const std::set<std::string> *volumeList=0);

   StiScanPrgOptions& fPrgOptions; ///< Command line arguments and options requested by the user
   HistMap mHs;
   double  mNodeZMin;
   double  mNodeZMax;
   float   mNodeRMin;
   float   mNodeRMax;
   /** If true will create integral projections of 2D profiles instead of creating 1D profiles with bin averages. */
   bool    mDoProjection;

   Profile3D* hNStepsVsPhiVsRVsZ;
   Profile3D* hELossVsPhiVsRVsZ;
   Profile3D* hELossVsXVsYVsZ;
   Profile3D* hDensityVsPhiVsRVsZ;
   Profile3D* hRelRadLengthVsPhiVsRVsZ;

private:

   void InitRange();
};

#endif
