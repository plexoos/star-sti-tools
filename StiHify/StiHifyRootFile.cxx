#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include "StiHifyRootFile.h"

#include "TH1.h"

#include "StiHify/StiHifyHistContainer.h"
#include "StiHify/StiHifyRatiosHistContainer.h"
#include "StiHify/StiHifyPrgOptions.h"


StiHifyRootFile::StiHifyRootFile(StiHifyPrgOptions& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   StiRootFile(prgOpts, option, ftitle, compress)
{
   BookHists(prgOpts);
}


void StiHifyRootFile::BookHists(const StiHifyPrgOptions& prgOpts)
{
   treeMaker = new StiHifyAnalysisTreeMaker(prgOpts);
   mDirs["sti_hit_any_node"] = new StiHifyHistContainer(prgOpts, "sti_hit_any_node", this);
   mDirs["sti_hit_accepted"] = new StiHifyHistContainer(prgOpts, "sti_hit_accepted", this);
   mDirs["sti_hit_rejected"] = new StiHifyHistContainer(prgOpts, "sti_hit_rejected", this);
}


void StiHifyRootFile::FillHists(const StiHifyEvent &event)
{
   treeMaker->FillTree(event, StiNodeHitStatus::Any);
   treeMaker->FillTree(event, StiNodeHitStatus::Accepted);
   treeMaker->FillTree(event, StiNodeHitStatus::Rejected);

   StiHifyHistContainer* container;

   container = static_cast<StiHifyHistContainer*> (mDirs["sti_hit_any_node"]);
   container->FillHists(event, StiNodeHitStatus::Any);

   container = static_cast<StiHifyHistContainer*> (mDirs["sti_hit_accepted"]);
   container->FillHists(event, StiNodeHitStatus::Accepted);

   container = static_cast<StiHifyHistContainer*> (mDirs["sti_hit_rejected"]);
   container->FillHists(event, StiNodeHitStatus::Rejected);
}


/**
 * Create ratios for all histograms whose name starts with hActiveLayerCounts.
 */
void StiHifyRootFile::FillDerivedHists()
{
   StiRootFile::FillDerivedHists();

   StiHifyRatiosHistContainer *ratios;
   mDirs["sti_hit_ratio"] = ratios = new StiHifyRatiosHistContainer("sti_hit_ratio", this);


   const tvx::HistMap& hists = mDirs["sti_hit_any_node"]->GetHists();

   for (const auto& hist_iter : hists)
   {
      std::string hist_name = hist_iter.first;

      // We select only histograms with names matching the following regex
      bool matched = boost::regex_match(hist_name, boost::regex("^hActiveLayerCounts.*$"));

      if (!matched) continue;

      const tvx::StiHistContainer& hitsNumer(*mDirs["sti_hit_accepted"]);
      const tvx::StiHistContainer& hitsDenom(*mDirs["sti_hit_any_node"]);

      const TH1* hitsAcc = hitsNumer.FindHist(hist_name);
      const TH1* hitsAll = hitsDenom.FindHist(hist_name);

      ratios->CreateRatioHist(hitsAcc, hitsAll);
   }

}
