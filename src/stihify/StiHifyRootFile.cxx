#include <boost/filesystem.hpp>
#include <regex>

#include "stihify/StiHifyRootFile.h"

#include "TH1.h"

#include "stihify/StiHifyHistContainer.h"
#include "stihify/StiHifyRatiosHistContainer.h"
#include "stihify/StiHifyPrgOptions.h"


StiHifyRootFile::StiHifyRootFile(StiHifyPrgOptions& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   tvx::RootFile(prgOpts, option, ftitle, compress)
{
   treeMaker = new StiHifyAnalysisTreeMaker(prgOpts);

   Add( new StiHifyHistContainer(prgOpts, "sti_hit_any_node", this) );
   Add( new StiHifyHistContainer(prgOpts, "sti_hit_accepted", this) );
   Add( new StiHifyHistContainer(prgOpts, "sti_hit_rejected", this) );
}


void StiHifyRootFile::FillHists(const StiHifyEvent &event)
{
   treeMaker->FillTree(event, StiNodeHitStatus::Any);
   treeMaker->FillTree(event, StiNodeHitStatus::Accepted);
   treeMaker->FillTree(event, StiNodeHitStatus::Rejected);

   hc<StiHifyHistContainer>("sti_hit_any_node")->FillHists(event, StiNodeHitStatus::Any);
   hc<StiHifyHistContainer>("sti_hit_accepted")->FillHists(event, StiNodeHitStatus::Accepted);
   hc<StiHifyHistContainer>("sti_hit_rejected")->FillHists(event, StiNodeHitStatus::Rejected);
}


/**
 * Create ratios for all histograms whose name starts with hActiveLayerCounts.
 */
void StiHifyRootFile::Finalize()
{
   tvx::RootFile::Finalize();

   StiHifyRatiosHistContainer *ratios = new StiHifyRatiosHistContainer("sti_hit_ratio", this);
   Add(ratios);


   const tvx::HistMap& hists = hc("sti_hit_any_node")->GetHists();

   for (const auto& hist_iter : hists)
   {
      std::string hist_name = hist_iter.first;

      // We select only histograms with names matching the following regex
      bool matched = std::regex_match(hist_name, std::regex("^hActiveLayerCounts.*$"));

      if (!matched) continue;

      const tvx::HistContainer& hitsNumer( *hc("sti_hit_accepted") );
      const tvx::HistContainer& hitsDenom( *hc("sti_hit_any_node") );

      const TH1& hitsAcc = hitsNumer[hist_name];
      const TH1& hitsAll = hitsDenom[hist_name];

      ratios->CreateRatioHist(&hitsAcc, &hitsAll);
   }

}
