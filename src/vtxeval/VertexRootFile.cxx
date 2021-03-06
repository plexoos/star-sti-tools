#include <float.h>

#include "TClonesArray.h"

#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuPrimaryVertex.h"
#include "StMuDSTMaker/COMMON/StMuMcVertex.h"

#include "vtxeval/VertexRootFile.h"
#include "vtxeval/StarEventHistContainer.h"
#include "vtxeval/StarVertexHistContainer.h"
#include "vtxeval/StarVertexHftHistContainer.h"
#include "vtxeval/DecayVertexHists.h"


VertexRootFile::VertexRootFile(tvx::ProgramOptions& prgOpts, Option_t *option, const char *ftitle, Int_t compress) :
   tvx::RootFile(prgOpts, option, ftitle, compress)
{
   Add( new StarEventHistContainer("event", this) );
   Add( new StarVertexHistContainer("vertex", this) );
   Add( new DecayVertexHists("vertex_decay_kaon", this) );
   Add( new DecayVertexHists("vertex_decay_lambda", this) );
   Add( new StarVertexHftHistContainer("vertex_hft", this) );
   Add( new StarVertexHistContainer("vertex_maxrank", this) );

   // When done creating hist containers make parent TDirectoryFile the current one
   cd();
}


void VertexRootFile::FillHists(const StMuDst &event)
{
   hc<StarEventHistContainer>("event")->FillHists(event);

   // Currently consider only one primary vertex with idTruth = 1
   int idTruth = 1;
   TClonesArray *muMcVertices = event.mcArray(0);
   StMuMcVertex *mcVertex = static_cast<StMuMcVertex*>(muMcVertices->UncheckedAt(idTruth - 1));

   TClonesArray *primaryVertices = event.primaryVertices();
   int numPrimaryVertices = primaryVertices->GetEntriesFast();
   StMuPrimaryVertex *recoVertex = nullptr;
   StMuPrimaryVertex *recoVertexMaxRank = nullptr;
   double maxRank = -DBL_MAX;

   // Loop over primary verticies in the event
   for (int iVertex = 0; iVertex < numPrimaryVertices; iVertex++)
   {
      StMuPrimaryVertex *testRecoVertex = (StMuPrimaryVertex *) primaryVertices->UncheckedAt(iVertex);

      // Theoretically, there can be other reco vertices with the same truth id
      // but for now we consider the first in the list matching the id of the
      // simulated vertex
      if (testRecoVertex && testRecoVertex->idTruth() == mcVertex->Id()) {
         recoVertex = testRecoVertex;
      }

      if (testRecoVertex->ranking() > maxRank) {
         maxRank = testRecoVertex->ranking();
         recoVertexMaxRank = testRecoVertex;
      }
   }

   hc<StarEventHistContainer>("event")->FillEfficyHists(event, *mcVertex, recoVertex, recoVertexMaxRank);
}


void VertexRootFile::FillHists(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex)
{
   hc<StarVertexHistContainer>("vertex")->FillHists(vertex, mcVertex);
}


void VertexRootFile::FillHists(const StMuPrimaryVertex &vertex, const std::vector<TDecayVertex>& decayVertices)
{
   for (const auto& decayVtx : decayVertices)
   {
      switch(decayVtx.parent)
      {
      case DecayParticle_t::Lambda:
      case DecayParticle_t::AntiLambda:
         hc<DecayVertexHists>("vertex_decay_lambda")->FillHists(vertex, decayVtx);
         break;
      case DecayParticle_t::Kaon:
         hc<DecayVertexHists>("vertex_decay_kaon")->FillHists(vertex, decayVtx);
         break;
      }
   }
}


void VertexRootFile::FillHistsHftTracks(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex)
{
   hc<StarVertexHistContainer>("vertex_hft")->FillHists(vertex, mcVertex);
}


void VertexRootFile::FillHistsMaxRank(const StMuPrimaryVertex &vertex, const StMuMcVertex* mcVertex)
{
   hc<StarVertexHistContainer>("vertex_maxrank")->FillHists(vertex, mcVertex);
}
