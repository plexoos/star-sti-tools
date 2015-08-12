#include "StiRootIO/TStiKalmanTrackNode.h"

#include <boost/regex.hpp>
#include <algorithm>

#include "StarClassLibrary/StThreeVector.hh"
#include "Sti/StiPlacement.h"
#include "StiRootIO/TStiKalmanTrack.h"


ClassImp(TStiKalmanTrackNode)


TStiKalmanTrackNode::TStiKalmanTrackNode() : TObject(),
   fStiTrackNode(nullptr),
   fTrack(nullptr), fValid(false), fIsInsideVolume(-1),
   fPosition(),
   fPositionLocal(),
   fProjPositionLocal(), fTrackP(), fEnergyLosses(-1), fNodeRadius(0), fNodeCenterRefAngle(0), fNodeMaterialDensity(0),
   fNodeTrackLength(0),
   fNodeRelRadLength(0), fVolumeName(""), fAcceptedStiHit(nullptr), fClosestStiHit(nullptr),
   fCandidateStiHits(),
   fProjError()
{
}


TStiKalmanTrackNode::TStiKalmanTrackNode(const StiKalmanTrackNode &stiKTN, TStiKalmanTrack* const parent) : TObject(),
   fStiTrackNode(&stiKTN),
   fTrack(parent), fValid(stiKTN.isValid()), fIsInsideVolume(stiKTN.inside(1+2+4)),
   fPosition(),
   fPositionLocal(),
   fProjPositionLocal(), fTrackP(), fEnergyLosses(-1), fNodeRadius(0), fNodeCenterRefAngle(0), fNodeMaterialDensity(0),
   fNodeTrackLength(stiKTN.getTrackLength()),
   fNodeRelRadLength(0), fVolumeName(""), fAcceptedStiHit(nullptr), fClosestStiHit(nullptr),
   fCandidateStiHits(),
   fProjError()
{
   // Access node parameters
   fPosition.SetXYZ(stiKTN.x_g(), stiKTN.y_g(), stiKTN.z_g());
   fPositionLocal.SetXYZ(stiKTN.x(), stiKTN.y(), stiKTN.z());

   StThreeVector<double> p3 = stiKTN.getGlobalMomentum();
   fTrackP.SetXYZ( p3.x(), p3.y(),  p3.z() );

   fEnergyLosses = stiKTN.getEnergyLosses() * 1e6; // Get losses in volume material and convert GeV to keV
   fNodeRelRadLength = stiKTN.getRelRadLength();

   const StiDetector* stiKTNDet = stiKTN.getDetector();

   if (stiKTNDet)
   {
      fVolumeName = stiKTNDet->getName();
      StiPlacement* stiPlacement = stiKTNDet->getPlacement();
      assert(stiPlacement);
      fNodeRadius = stiPlacement->getLayerRadius();
      fNodeCenterRefAngle = stiPlacement->getCenterRefAngle();

      StiMaterial* stiMaterial = stiKTNDet->getMaterial();
      assert(stiMaterial);

      fNodeMaterialDensity = stiMaterial->getDensity();
   }

   if (stiKTN.getHit())
   {
      auto resultPair = fTrack->AddToParentEvent( TStiHit(*stiKTN.getHit()) );
      // Save the pointer to the hit in the parent event
      fAcceptedStiHit = &(*resultPair.first);
   }

   // Note (as of now) this info is available for nodes with r < kRMinTpc [= 55 cm]
   // see node->saveInfo() in StiKalmanTrackFinder.cxx
   const StiNodeInf* prefitKTNParams = stiKTN.getInfo();

   if (prefitKTNParams) {
      fProjError.SetXYZ(sqrt(prefitKTNParams->mPE._cXX), sqrt(prefitKTNParams->mPE._cYY), sqrt(prefitKTNParams->mPE._cZZ));
      fProjPositionLocal.SetXYZ(prefitKTNParams->mPP.x(), prefitKTNParams->mPP.y(), prefitKTNParams->mPP.z());
   }
}


TVector3 TStiKalmanTrackNode::CalcPullToHit(const TStiHit& hit) const
{
   double pullX = (hit.GetPositionLocal().X() - fProjPositionLocal.X()) / fProjError.X();
   double pullY = (hit.GetPositionLocal().Y() - fProjPositionLocal.Y()) / fProjError.Y();
   double pullZ = (hit.GetPositionLocal().Z() - fProjPositionLocal.Z()) / fProjError.Z();

   return TVector3(pullX, pullY, pullZ);
}


/**
 * Calculates and returns uncorrelated three components which can be used in the
 * pull distribution.
 */
TVector3 TStiKalmanTrackNode::CalcPullClosestHit() const
{
   if (!fClosestStiHit) return TVector3(DBL_MAX, DBL_MAX, DBL_MAX);

   double pullX = (fClosestStiHit->GetPositionLocal().X() - fProjPositionLocal.X()) / fProjError.X();
   double pullY = (fClosestStiHit->GetPositionLocal().Y() - fProjPositionLocal.Y()) / fProjError.Y();
   double pullZ = (fClosestStiHit->GetPositionLocal().Z() - fProjPositionLocal.Z()) / fProjError.Z();

   return TVector3(pullX, pullY, pullZ);
}


/**
 * Calculate chi2 for a user provided hit using the original Sti methods.
 */
double TStiKalmanTrackNode::CalcChi2(const TStiHit& hit) const
{
   if (!fStiTrackNode || !hit.GetStiHit())
      return -1;

   return fStiTrackNode->evaluateChi2Info(hit.GetStiHit());
}


bool TStiKalmanTrackNode::MatchedVolName(const std::string & pattern) const
{
   if (fVolumeName.empty()) return true;

   boost::regex r(pattern);
   bool matched = boost::regex_match(fVolumeName, r);

   //if (matched)
   //   Info("MatchedVolName", "Volume [%s] matched pattern [%s]", fVolumeName.c_str(), pattern.c_str());

   return matched;
}


bool TStiKalmanTrackNode::MatchedVolName(const std::set<std::string> & patterns) const
{
   if (fVolumeName.empty() || patterns.empty())
      return true;

   std::set<std::string>::const_iterator iPattern = patterns.begin();

   for( ; iPattern != patterns.end(); ++iPattern )
   {
      //Info("MatchedVolName", "Looking for pattern [%s] in fVolumeName [%s]", (*iPattern).c_str(), fVolumeName.c_str());
      if ( MatchedVolName(*iPattern) )
         return true;
   }

   return false;
}


void TStiKalmanTrackNode::Print(Option_t *opt) const
{
   fPosition.Print();
   fTrackP.Print();
   printf("fEnergyLosses:    %f\n", fEnergyLosses);
   printf("fNodeRadius:      %f\n", fNodeRadius);
   printf("fVolumeName:      %s\n", fVolumeName.c_str());
}


bool operator< (const TStiKalmanTrackNode& lhs, const TStiKalmanTrackNode& rhs)
{
   return lhs.fNodeRadius < rhs.fNodeRadius;
}


std::set<const TStiHit*> TStiKalmanTrackNode::GetCandidateHits() const
{
   std::set<const TStiHit*> candidateHits;

   std::transform(fCandidateStiHits.begin(), fCandidateStiHits.end(),
      std::inserter(candidateHits, candidateHits.begin()), TStiHitProxy::GetBareStiHit);

   return candidateHits;
}


void TStiKalmanTrackNode::FindClosestHit(const std::set<TStiHit>& stiHits)
{
   TVector3 distVec;
   double min_dist = DBL_MAX;

   for (const auto& hit : stiHits)
   {
      distVec = GetPosition() - hit.GetPosition();

      double dist = distVec.Mag();
      if (dist < min_dist) {
         min_dist = dist;
         fClosestStiHit = &hit;
      }
   }
}


/**
 * Finds all hits within a 5x(track_proj_err) vicinity of the track mean
 * projection and fills this node's fCandidateStiHits collection with pointers
 * to the found hits. Note that the function also among the candidate hist finds
 * the closest hit to the track node position. The hits are selected from the
 * user provided collection stiHits which should normaly be a collection of hits
 * in the parent event to which the track belongs.
 */
void TStiKalmanTrackNode::FindCandidateHits(const std::set<TStiHit>& stiHits)
{
   TVector3 distVec;
   double min_dist = DBL_MAX;

   for (const auto& hit : stiHits)
   {
      if (fVolumeName != hit.GetVolumeName()) continue;

      distVec = GetPositionLocal() - hit.GetPositionLocal();

      if (fabs(distVec.Y()) < 5*fProjError.Y() &&
          fabs(distVec.Z()) < 5*fProjError.Z() )
      {
         fCandidateStiHits.insert(TStiHitProxy(hit, *this));

         double dist = distVec.Mag();
         if (dist < min_dist) {
            min_dist = dist;
            fClosestStiHit = &hit;
         }
      }
   }
}


void TStiKalmanTrackNode::FindCandidateHitsByChi2(const std::set<TStiHit>& stiHits)
{
   for (const auto& hit : stiHits)
   {
      if (fVolumeName != hit.GetVolumeName()) continue;

      if (CalcChi2(hit) < 20)
      {
         fCandidateStiHits.insert(TStiHitProxy(hit, *this));
      }
   }
}
