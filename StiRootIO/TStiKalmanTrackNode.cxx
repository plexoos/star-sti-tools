#include "StiRootIO/TStiKalmanTrackNode.h"

#include <boost/regex.hpp>

#include "StarClassLibrary/StThreeVector.hh"
#include "Sti/StiPlacement.h"
#include "StiRootIO/TStiKalmanTrack.h"


ClassImp(TStiKalmanTrackNode)


TStiKalmanTrackNode::TStiKalmanTrackNode() : TObject(),
   fTrack(nullptr), fValid(false), fIsInsideVolume(-1),
   fPosition(), fPositionLocal(), fTrackP(), fEnergyLosses(-1), fNodeRadius(0), fNodeCenterRefAngle(0), fNodeMaterialDensity(0),
   fNodeTrackLength(0),
   fNodeRelRadLength(0), fVolumeName(""), fStiHit(nullptr), fClosestStiHit(nullptr),
   fAdjacentStiHits(),
   fTrackProjErr()
{
}


TStiKalmanTrackNode::TStiKalmanTrackNode(TStiKalmanTrack* const track, const StiKalmanTrackNode &stiKTN) : TObject(),
   fTrack(track), fValid(stiKTN.isValid()), fIsInsideVolume(stiKTN.inside(1+2+4)),
   fPosition(), fPositionLocal(), fTrackP(), fEnergyLosses(-1), fNodeRadius(0), fNodeCenterRefAngle(0), fNodeMaterialDensity(0),
   fNodeTrackLength(stiKTN.getTrackLength()),
   fNodeRelRadLength(0), fVolumeName(""), fStiHit(nullptr), fClosestStiHit(nullptr),
   fAdjacentStiHits(),
   fTrackProjErr()
{
   // Access node parameters
   fPosition.SetXYZ(stiKTN.x_g(), stiKTN.y_g(), stiKTN.z_g());

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
      fStiHit = &(*resultPair.first);
   }

   // Note (as of now) this info is available for nodes with r < kRMinTpc [= 55 cm]
   // see node->saveInfo() in StiKalmanTrackFinder.cxx
   const StiNodeInf* prefitKTNParams = stiKTN.getInfo();

   if (prefitKTNParams) {
      fTrackProjErr.SetXYZ(sqrt(prefitKTNParams->mPE._cXX), sqrt(prefitKTNParams->mPE._cYY), sqrt(prefitKTNParams->mPE._cZZ));
      fPositionLocal.SetXYZ(prefitKTNParams->mPP.x(), prefitKTNParams->mPP.y(), prefitKTNParams->mPP.z());
   }
}


TVector3 TStiKalmanTrackNode::CalcPullToHit(const TStiHit& hit) const
{
   double pullX = (hit.GetPositionLocal().X() - fPositionLocal.X()) / fTrackProjErr.X();
   double pullY = (hit.GetPositionLocal().Y() - fPositionLocal.Y()) / fTrackProjErr.Y();
   double pullZ = (hit.GetPositionLocal().Z() - fPositionLocal.Z()) / fTrackProjErr.Z();

   return TVector3(pullX, pullY, pullZ);
}


/**
 * Calculates and returns uncorrelated three components which can be used in the
 * pull distribution.
 */
TVector3 TStiKalmanTrackNode::CalcPullClosestHit() const
{
   if (!fClosestStiHit) return TVector3(DBL_MAX, DBL_MAX, DBL_MAX);

   double pullX = (fClosestStiHit->GetPositionLocal().X() - fPositionLocal.X()) / fTrackProjErr.X();
   double pullY = (fClosestStiHit->GetPositionLocal().Y() - fPositionLocal.Y()) / fTrackProjErr.Y();
   double pullZ = (fClosestStiHit->GetPositionLocal().Z() - fPositionLocal.Z()) / fTrackProjErr.Z();

   return TVector3(pullX, pullY, pullZ);
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


void TStiKalmanTrackNode::AssignClosestHit(const std::set<TStiHit>& stiHits)
{
   TVector3 distVec;
   double min_dist = DBL_MAX;

   for (auto iHit = stiHits.begin(); iHit != stiHits.end(); ++iHit)
   {
      distVec = GetPosition() - iHit->GetPosition();

      double dist = distVec.Mag();
      if (dist < min_dist) {
         min_dist = dist;
         fClosestStiHit = &*iHit;
      }
   }
}


/**
 * Finds all hits within a 5x(track_proj_err) vicinity of the track mean
 * projection and fills this node's fAdjacentStiHits collection with pointers to
 * the found hits. The hits are selected from the user provided collection
 * stiHits which is normaly a collection of hits in the parent event to which
 * the track belongs.
 */
void TStiKalmanTrackNode::FindAdjacentHits(const std::set<TStiHit>& stiHits)
{
   TVector3 distVec;

   for (auto iHit = stiHits.begin(); iHit != stiHits.end(); ++iHit)
   {
      distVec = GetPosition() - iHit->GetPosition();

      if (distVec.Mag() < 5*fTrackProjErr.Mag())
      {
         fAdjacentStiHits.insert(&*iHit);
      }
   }
}
