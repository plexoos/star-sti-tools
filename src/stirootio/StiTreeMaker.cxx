#include "stirootio/StiTreeMaker.h"

#include "TBranch.h"
#include "TSystem.h"

#include "StBFChain/StBFChain.h"
#include "StEvent/StEnumerations.h"
#include "StEvent/StEnumerations.h"


StiTreeMaker::StiTreeMaker(const std::string &name, const std::string &suffix) : StMaker(name.c_str()),
   fFile(nullptr),
   fSuffix(suffix),
   fTree(new TTree("t", "TTree with HFT hits and tracks")),
   fEvent(nullptr)
{
}


StiTreeMaker::~StiTreeMaker()
{
   if (fFile) fFile->Close();

   delete fFile; fFile = nullptr;
   delete fTree; fTree = nullptr;
   delete fEvent; fEvent = nullptr;
}


/**
 * Creates an output ROOT file and a ROOT tree with a user defined structure. It
 * is the user's responsibility to attach a branch to the tree by overriding
 * SetEventTree(). The name of the output file is formed from the input file
 * name by adding a suffix string.
 */
Int_t StiTreeMaker::Init()
{
   StBFChain *bfChain = (StBFChain *) StMaker::GetChain();

   if (!bfChain) return kStFatal;

   // Authorize Trees up to 2 Terabytes (if the system can do it)
   TTree::SetMaxTreeSize(1000 * Long64_t(2000000000));

   TString fileName( gSystem->BaseName(bfChain->GetFileOut().Data()) );

   // Form a new name for the output file
   fileName += "." + fSuffix + ".root";

   fFile = new TFile(fileName, "RECREATE", "TTree with HFT hits and tracks");
   fFile->SetCompressionLevel(1); // Set file compression level

   SetEventTree();

   fTree->SetDirectory(fFile);

   return kStOK;
}


Int_t StiTreeMaker::Finish()
{
   fFile = fTree->GetCurrentFile(); //just in case we switched to a new file
   fFile->Write();

   if (GetDebug() >= 1)
      fTree->Print();

   return kStOK;
}


/** Clear the container for the next event. */
void StiTreeMaker::Clear(Option_t *opt)
{
   fEvent->Clear();
}
