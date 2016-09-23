#include <cmath>
#include <boost/filesystem.hpp>

#include "StiScanRootFile.h"

#include "TRandom.h"

#include "StiScan/StiScanHistContainer.h"
#include "StiScan/StiScanHistsByVolume.h"
#include "StiScan/StiScanPrgOptions.h"
#include "StiScan/StiScanRatiosHistContainer.h"


StiScanRootFile::StiScanRootFile(StiScanPrgOptions& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   tvx::RootFile(prgOpts, option, ftitle, compress),
   fPrgOptions(prgOpts)
{
   // Find ranges (\todo if requested by the user)
   if (fPrgOptions.DoAutoHistRange()) {
      Info("StiScanRootFile", "Find auto range. Loop over tree/chain...");
      FindAutoRange();
   }

   Add( new StiScanHistsByVolume(fPrgOptions, "sti_vol", this) );
   Add( new StiScanHistContainer(fPrgOptions, "sti_trk", this) );
   Add( new StiScanHistContainer(fPrgOptions, "gea", this) ); // Will create integral projections from 2D histograms
}


void StiScanRootFile::FindAutoRange() const
{
   const std::set<std::string> *volumeList = &fPrgOptions.GetVolumeList();

   TChain *stiChain = fPrgOptions.GetStiTChain();

   StiScanEvent *stiEvent = new StiScanEvent();
   stiChain->SetBranchAddress("e.", &stiEvent);
   stiChain->SetBranchStatus("e.TStiEvent.*", false);
   stiChain->SetBranchStatus("e.TStiEvent.fTStiKalmanTracks*", true);

   double nodeRMax = 0;
   double nodeZMin = DBL_MAX;
   double nodeZMax = -DBL_MAX;

   TRandom myRandom;

   int nTreeEvents = stiChain->GetEntries();

   Info("FindAutoRange", "Found tree/chain with N entries: %d", nTreeEvents);

   for (int iEvent = 1; iEvent <= nTreeEvents; iEvent++)
   {
      if ( nTreeEvents >= 10 && iEvent % int(nTreeEvents*0.1) == 0 )
         Info("FindAutoRange", "Analyzing event %d", iEvent);

      if (myRandom.Rndm() > fPrgOptions.GetSparsity()) continue;

      stiChain->GetEntry(iEvent-1);

      for (const auto& kalmTrack : stiEvent->GetTStiKalmanTracks())
      {
         for (const auto& kalmNode : kalmTrack.GetNodes())
         {
            if (volumeList && volumeList->size() && !kalmNode.MatchedVolName(*volumeList) )
               continue;

            if (kalmNode.GetNodeMaterialDensity() <= 0)
               continue;

            double node_z = kalmNode.GetPosition().Z();

            if (node_z < nodeZMin) nodeZMin = node_z;
            if (node_z > nodeZMax) nodeZMax = node_z;

            // Find maximum radius
            double node_r = (float) kalmNode.GetPosition().Perp();

            if (node_r > nodeRMax) nodeRMax = node_r;
         }
      }
   }

   // In case no entries were found in the tree set some reasonable values
   if (nodeRMax == 0 && nodeZMin == DBL_MAX && nodeZMax == -DBL_MAX) {
      nodeRMax = 1;
      nodeZMin = 0;
      nodeZMax = 1;
   }

   Info("FindAutoRange", "Updated nodeZMin, nodeZMax, nodeRMax: %g, %g, %g", nodeZMin, nodeZMax, nodeRMax);

   fPrgOptions.SetHistZRange(nodeZMin, nodeZMax);
   fPrgOptions.SetHistRMax(nodeRMax);

   delete stiEvent;
}


void StiScanRootFile::FillHists(const StiScanEvent &stiEvent, const std::set<std::string> *volumeList)
{
   StiScanHistContainer* container;

   container = static_cast<StiScanHistContainer*> (fDirs["sti_vol"]);
   container->FillHists(stiEvent, volumeList);

   container = static_cast<StiScanHistContainer*> (fDirs["sti_trk"]);
   container->FillHists(stiEvent, volumeList);
}


void StiScanRootFile::FillHists(const TGeaEvent &geaEvent, const std::set<std::string> *volumeList)
{
   StiScanHistContainer* container = static_cast<StiScanHistContainer*> (fDirs["gea"]);
   container->FillHists(geaEvent, volumeList);
}


void StiScanRootFile::FillDerivedHists()
{
   tvx::RootFile::FillDerivedHists();

   StiScanRatiosHistContainer *ratios;
   fDirs["sti_gea_ratio"] = ratios = new StiScanRatiosHistContainer("sti_gea_ratio", this);

   const tvx::HistContainer* gea = fDirs["gea"];
   const tvx::HistContainer* sti_trk = fDirs["sti_trk"];

   const TH1* gea_eloss = gea->FindHist("hELossVsPhiVsRVsZ_pyx");
   const TH1* sti_eloss = sti_trk->FindHist("hELossVsPhiVsRVsZ_pyx");
   ratios->CreateRatioHist(sti_eloss, gea_eloss);

   gea_eloss = gea->FindHist("hELossVsPhiVsRVsZ_pyx_px");
   sti_eloss = sti_trk->FindHist("hELossVsPhiVsRVsZ_pyx_px");
   ratios->CreateRatioHist(sti_eloss, gea_eloss);

   gea_eloss = gea->FindHist("hELossVsXVsYVsZ_pyx");
   sti_eloss = sti_trk->FindHist("hELossVsXVsYVsZ_pyx");
   ratios->CreateRatioHist(sti_eloss, gea_eloss);
}
