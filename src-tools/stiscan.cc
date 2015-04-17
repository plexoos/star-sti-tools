#include <fstream>
#include <string>
#include <unordered_map>

#include "TChain.h"
#include "TError.h"
#include "TGeoNavigator.h"
#include "TGeoManager.h"
#include "TRandom.h"
#include "TROOT.h"

#include "GeaRootIO/TGeaEvent.h"
#include "StiScan/StiScanEvent.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiScan/StiScanRootFile.h"

typedef std::unordered_map<size_t, std::string> Hash2StringMap;


// These globals are not used but required since we link against TGiant3.o
//int    Margc=0;
//char** Margv=NULL;

TGeoManager *gGeoManager = 0;


void loop_hftree(StiScanPrgOptions &poProc);
void make_geometry(std::string geoTag="y2014a");
void create_volume_hash_map(TGeoNavigator &geoNav, Hash2StringMap &hash2PathMap);


int main(int argc, char **argv)
{
   const std::string hftTreeName = "t";
   const std::string geantStepTreeName = "stepping";

   StiScanPrgOptions poProc(argc, argv, hftTreeName, geantStepTreeName);
   poProc.ProcessOptions();

   // Initialize gGeoManager with geometry from a ROOT file
   make_geometry("y2014a");

   gGeoManager->cd("HALL_1/CAVE_1");
   TGeoNavigator* geoNav = gGeoManager->GetCurrentNavigator();
   assert(geoNav);

   Hash2StringMap hash2PathMap;
   create_volume_hash_map(*geoNav, hash2PathMap);

   TGeaEvent::fgHash2PathMap = &hash2PathMap;

   loop_hftree(poProc);

   return EXIT_SUCCESS;
}


void loop_hftree(StiScanPrgOptions &poProc)
{
   TChain *hftChain       = poProc.GetHftChain();
   TChain *geantStepChain = poProc.GetGeantStepChain();

   // Create a new output file
   std::string outFileName = poProc.GetHftreeFile();

   std::string postfix("stiscan.root");
   std::size_t postfix_pos = outFileName.find(postfix);

   if (postfix_pos != std::string::npos)
   {
      outFileName.replace( postfix_pos, postfix.length(), "stiscan.hist.root");
   } else {
      outFileName += "_stiscan.root";
   }

   StiScanRootFile outRootFile(poProc, outFileName.c_str(), "recreate");

   int nTreeEvents = hftChain->GetEntries();
   int nProcEvents = 0;

   Info("loop_hftree", "Found tree/chain with N entries: %d", nTreeEvents);

   StiScanEvent *stiScanEvent = new StiScanEvent();
   hftChain->SetBranchAddress("e.", &stiScanEvent);
   hftChain->SetBranchStatus("e.*", false);
   hftChain->SetBranchStatus("e.TStiEvent.fTStiKalmanTracks*", true);

   // Prepare resources for geant event
   TGeaEvent *geantEvent = new TGeaEvent();

   if (poProc.DoGeantStepTree())
      geantStepChain->SetBranchAddress("TGeaEvent", &geantEvent);

   TRandom myRandom;

   Info("loop_hftree", "Loop over tree/chain...");

   for (int iEvent = 1; iEvent <= nTreeEvents; iEvent++, nProcEvents++)
   {
      if ( nTreeEvents >= 10 && iEvent % int(nTreeEvents*0.1) == 0 )
         Info("loop_hftree", "Analyzing event %d", iEvent);

      if (myRandom.Rndm() > poProc.GetSparsity()) continue;

      hftChain->GetEntry(iEvent-1);

      outRootFile.FillHists(*stiScanEvent, &poProc.GetVolumeList());

      if (poProc.DoGeantStepTree()) {
         geantStepChain->GetEntry(iEvent-1);
         outRootFile.FillHists(*geantEvent, &poProc.GetVolumeList());
      }
   }

   delete stiScanEvent;
   delete geantEvent;

   outRootFile.FillDerivedHists();

   if (poProc.SaveGraphics()) {
      gROOT->Macro(poProc.GetStyleMacro().c_str());
      outRootFile.SaveAllAs(poProc.GetOutPrefix());
   }

   outRootFile.Write();
   outRootFile.Close();
}


void make_geometry(std::string geoTag)
{
   std::string myGeomTag("y2014a");

   TGeoManager::Import( Form("%s.root", myGeomTag.c_str()) );

   if (!gGeoManager) {
      Error("make_geometry", "No gGeoManager found");
      exit(EXIT_FAILURE);
   }
}


void create_volume_hash_map(TGeoNavigator &geoNav, Hash2StringMap &hash2PathMap)
{
   TGeoNode* currNode = geoNav.GetCurrentNode();

   if (!currNode) {
      Warning("create_volume_hash_map", "Invalid TGeoNode provided as input. Skipping...");
      return;
   }

   // Keep track of current level with respect to the original node
   static int level = 0;

   std::string currentPath( geoNav.GetPath() );

   int nDaughters = currNode->GetVolume()->GetNdaughters();

   if (nDaughters) {

      TGeoNode* motherNode = currNode;
      for (int iDaughter = 0; iDaughter < nDaughters; iDaughter++)
      {
         TGeoNode *daughter = motherNode->GetVolume()->GetNode(iDaughter);

         geoNav.CdDown(daughter);
         level++;

         create_volume_hash_map(geoNav, hash2PathMap);
      }

   } else { // We are here if this node is a leaf, i.e. no daughters

      // Add this volume to the hash map
      std::string hashedPath(currentPath);

      // Remove a "TpcRefSys_1/" substring as it not relevant for geometry trees used in simulation
      size_t first_pos = hashedPath.find("TpcRefSys_1/");
      if (first_pos != std::string::npos) {
         hashedPath.replace(first_pos, std::string("TpcRefSys_1/").length(), "");
      }

      std::hash<std::string> hash_fn;
      std::size_t hash_value = hash_fn(hashedPath);

      std::pair<size_t, std::string> hash2Path(hash_value, hashedPath);

      hash2PathMap.insert(hash2Path);

      geoNav.cd(currentPath.c_str());
   }

   if (level > 0) {
      geoNav.CdUp();
      level--;
   }
}