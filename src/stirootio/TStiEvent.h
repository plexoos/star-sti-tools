#ifndef TStiEvent_h
#define TStiEvent_h

#include <vector>
#include <set>
#include <utility>

#include "TObject.h"

#include "St_base/Stypes.h"
#include "StEvent/StEnumerations.h"
#include "Sti/StiTrackContainer.h"
#include "Sti/StiHitContainer.h"
#include "stirootio/TStiKalmanTrack.h"
#include "stirootio/TStiHit.h"

class StiVolumeFilter;
class StiKalmanTrack;
class StiKalmanTrackNode;


class TStiEvent : public TObject
{
public:

   TStiEvent();
   explicit TStiEvent(const StiVolumeFilter& stiVolumeFilter);

   bool AcceptTrack(const StiKalmanTrack& track) const;
   bool AcceptTrackNode(const StiKalmanTrackNode& node) const;
   virtual EReturnCodes  Fill(const StiTrackContainer &stiTrackContainer);
   virtual EReturnCodes  Fill(StiHitContainer &stiHitContainer);
   virtual EReturnCodes  PostFill();
   std::pair<std::set<TStiHit>::iterator, bool>   InsertStiHit(const TStiHit &stiHit) { return fTStiHits.insert(stiHit); }
   const std::set<TStiHit>& GetStiHits() const { return fTStiHits; }
   virtual void  Clear(Option_t *opt = "");
   virtual void  Print(Option_t *opt = "") const;
   const std::vector<TStiKalmanTrack>& GetTStiKalmanTracks() const { return fTStiKalmanTracks; }

protected:

   /// A singleton allowing to impose various requirements on tracks and/or
   /// track nodes. Usually, tracks crossing only specific detector volumes are
   /// processed and saved
   const StiVolumeFilter&  fStiVolumeFilter;

   /// A collection of all Sti tracks of interest in this event
   std::vector<TStiKalmanTrack>  fTStiKalmanTracks;

   /// A collection of all Sti hits of interest in this event
   std::set<TStiHit>  fTStiHits;

   ClassDef(TStiEvent, 1)
};

#endif
