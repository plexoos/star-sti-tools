#include <cmath>

#include "TVector3.h"

#include "StiHify/StiHifyHistContainer.h"
#include "StiRootIO/TStiHitProxy.h"


StiHifyHistContainer::StiHifyHistContainer(const StiHifyPrgOptions& prgOpts, const char* name, TDirectory* motherDir, Option_t* option) :
   StiHistContainer(name, motherDir, option),
   fPrgOptions(prgOpts),
   hDistClosest2AcceptedHit(nullptr),
   hPullClosestHit1D(nullptr),
   hPullClosestHit2D(nullptr),
   hPullCandidateHits2D(nullptr),
   hChi2CandidateHits(nullptr),
   hCountCandidateHits(nullptr),
   hActiveLayerCounts(nullptr),
   hActiveLayerCounts_HitCandidate(nullptr),
   hActiveLayerCounts_TrkPrj(nullptr)
{
   BookHists();
}


void StiHifyHistContainer::BookHists()
{
   this->cd();

   mHs["hDistClosest2AcceptedHit"] = hDistClosest2AcceptedHit
      = new TH1I("hDistClosest2AcceptedHit", " ; Closest to Accepted Hits: Distance R, cm; Num. of Track Nodes; ", 100, 0, 0.5);
   hDistClosest2AcceptedHit->SetOption("XY hist");

   mHs["hPullClosestHit1D"] = hPullClosestHit1D
      = new TH1I("hPullClosestHit1D", " ; Track Proj. to Closest Hit Pull Dist.: Distance R, #sigma-units; Num. of Track Nodes; ", 100, 0, 10);

   mHs["hPullClosestHit2D"] = hPullClosestHit2D
      = new TH2I("hPullClosestHit2D", " ; Track Proj. to Closest Hit Pull Dist.: Local Z, #sigma-units; Local Y, #sigma-units; Num. of Track Nodes", 50, -10, 10, 50, -10, 10);
   hPullClosestHit2D->SetOption("colz");

   mHs["hPullCandidateHits2D"] = hPullCandidateHits2D
      = new TH2I("hPullCandidateHits2D", " ; Track Proj. to Candidate Hit Pull Dist.: Local Z, #sigma-units; Local Y, #sigma-units; Num. of Track Nodes", 50, -6, 6, 50, -6, 6);
   hPullCandidateHits2D->SetOption("colz");

   mHs["hChi2CandidateHits"] = hChi2CandidateHits
      = new TH1I("hChi2CandidateHits", " ; Track Proj. to Candidate Hit: #chi^{2}; Num. of Track Nodes", 50, 0, 50);

   mHs["hCountCandidateHits"] = hCountCandidateHits
      = new TH1I("hCountCandidateHits", " ; Num. of Candidate Hits; Num. of Track Nodes", 20, 0, 20);

   mHs["hActiveLayerCounts"] = hActiveLayerCounts
      = new TH2F("hActiveLayerCounts", " ; Track Local Z, cm; Local Y, cm; Num. of Track Nodes", 25, -25, 25, 10, -2, 8);
   hActiveLayerCounts->SetOption("colz");

   mHs["hActiveLayerCounts_HitCandidate"] = hActiveLayerCounts_HitCandidate
      = new TH2F("hActiveLayerCounts_HitCandidate", " ; Track Local Z, cm; Local Y, cm; Num. of Track Nodes", 25, -25, 25, 10, -2, 8);
   hActiveLayerCounts_HitCandidate->SetOption("colz");

   mHs["hActiveLayerCounts_TrkPrj"] = hActiveLayerCounts_TrkPrj
      = new TH2F("hActiveLayerCounts_TrkPrj", " ; Track Proj. Local Z, cm; Local Y, cm; Num. of Track Nodes", 25, -25, 25, 10, -2, 8);
   hActiveLayerCounts_TrkPrj->SetOption("colz");
}


void StiHifyHistContainer::FillHists(const StiHifyEvent &event, StiNodeHitStatus hitStatus, const std::set<std::string> *volumeList)
{
   for (auto iTStiKTrack=event.GetTStiKalmanTracks().begin(); iTStiKTrack!=event.GetTStiKalmanTracks().end(); ++iTStiKTrack)
   {
      const TStiKalmanTrack &kalmTrack = *iTStiKTrack;

      for (auto iTStiKTrackNode=kalmTrack.GetNodes().begin(); iTStiKTrackNode!=kalmTrack.GetNodes().end(); ++iTStiKTrackNode)
      {
         const TStiKalmanTrackNode &trkNode = *iTStiKTrackNode;

         switch (hitStatus)
         {
         case StiNodeHitStatus::Any:
            FillHists(trkNode, volumeList);
            break;
         case StiNodeHitStatus::Accepted:
            FillHistsHitsAccepted(trkNode, volumeList);
            break;
         case StiNodeHitStatus::Rejected:
            FillHistsHitsRejected(trkNode, volumeList);
            break;
         default:
            Error("FillHists", "Internal type of Sti hit assigned to this node is not specified. "
                  "Histograms won't be filled");
            break;
         }
      }
   }
}


void StiHifyHistContainer::FillHists(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList)
{
   if (volumeList && volumeList->size() && !trkNode.MatchedVolName(*volumeList) )
      return;

   if (trkNode.GetVolumeName().empty() || !trkNode.IsInsideVolume())
      return;

   if (trkNode.GetHit()) {
      hDistClosest2AcceptedHit->Fill( fabs(trkNode.CalcDistanceToHit() - trkNode.CalcDistanceToClosestHit()) );
   }

   hPullClosestHit1D->Fill(trkNode.CalcDistanceToClosestHit() < 0 ? -1 : (trkNode.CalcDistanceToClosestHit()/trkNode.GetTrackProjErr().Mag()) );

   TVector3 pull = trkNode.CalcPullClosestHit();
   hPullClosestHit2D->Fill(pull.Z(), pull.Y());

   const std::set<TStiHitProxy>& hitCandidates = trkNode.GetCandidateProxyHits();

   hCountCandidateHits->Fill(hitCandidates.size());

   // Consider the first candidate hit only. This histogram is used in hit
   // efficiency calculation
   bool foundClosestCandidate = false;

   for (auto iHitCandidate=hitCandidates.begin(); iHitCandidate!=hitCandidates.end(); ++iHitCandidate)
   {
      const TStiHitProxy& hitCandidate = *iHitCandidate;

      TVector3 pull = trkNode.CalcPullToHit( *hitCandidate.GetTStiHit() );
      hPullCandidateHits2D->Fill(pull.Z(), pull.Y());
      hChi2CandidateHits->Fill(hitCandidate.GetChi2());

      // Choose the first (i.e. the closest) candidate hit
      if (hitCandidate.GetDistanceToNode() >= 0 && !foundClosestCandidate) {
         hActiveLayerCounts_HitCandidate->Fill(trkNode.GetPositionLocal().Z(), trkNode.GetPositionLocal().Y());
         foundClosestCandidate = true;
      }
   }

   hActiveLayerCounts->Fill(trkNode.GetPositionLocal().Z(), trkNode.GetPositionLocal().Y());

   hActiveLayerCounts_TrkPrj->Fill(trkNode.GetProjPositionLocal().Z(), trkNode.GetProjPositionLocal().Y());


   std::string histName("hActiveLayerCounts_" + trkNode.GetVolumeName());

   TH1* hActiveLayerCounts_det = FindHist(histName);

   if (!hActiveLayerCounts_det) {
      this->cd();
      mHs[histName] = hActiveLayerCounts_det
         = new TH2F(histName.c_str(), " ; Track Local Z, cm; Local Y, cm; Num. of Track Nodes", 25, -25, 25, 10, -2, 8);
      hActiveLayerCounts_det->SetOption("colz");
   }

   hActiveLayerCounts_det->Fill( trkNode.GetPositionLocal().Z(), trkNode.GetPositionLocal().Y() );
}


void StiHifyHistContainer::FillHistsHitsAccepted(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList)
{
   if (!trkNode.GetHit())
      return;

   FillHists(trkNode, volumeList);
}


void StiHifyHistContainer::FillHistsHitsRejected(const TStiKalmanTrackNode &trkNode, const std::set<std::string> *volumeList)
{
   if (trkNode.GetHit())
      return;

   FillHists(trkNode, volumeList);
}


void StiHifyHistContainer::FillDerivedHists()
{
   this->cd();

   TH1 *myRatio = static_cast<TH1*>(hActiveLayerCounts->Clone());
   myRatio->SetOption("colz");
   myRatio->SetName("hActiveLayerCounts_HitCandidate_eff");
   myRatio->Divide(hActiveLayerCounts_HitCandidate, hActiveLayerCounts, 1, 1, "B");

   mHs[std::string(myRatio->GetName())] = myRatio;
}
