#ifndef TGiant3_H 
#define TGiant3_H 
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

/* $Id: TGiant3.h,v 1.1 2005/02/07 21:08:51 fisyak Exp $ */

//////////////////////////////////////////////// 
//  C++ interface to Geant3 basic routines    // 
//////////////////////////////////////////////// 
 
#include <StarMC.h> 
#include "TRandom.h"  
//______________________________________________________________
//
//       Geant3 prototypes for commons
//
//______________________________________________________________
//

//----------QUEST 
//      COMMON/QUEST/IQUEST(100) 
typedef struct { 
  Int_t    iquest[100]; 
} Quest_t; 
 
//----------GCBANK
//      COMMON/GCBANK/NZEBRA,GVERSN,ZVERSN,IXSTOR,IXDIV,IXCONS,FENDQ(16)
//     +             ,LMAIN,LR1,WS(KWBANK)
typedef struct {
  Int_t nzebra;
  Float_t gversn;
  Float_t zversn;
  Int_t ixstor;
  Int_t ixdiv;
  Int_t ixcons;
  Float_t fendq[16];
  Int_t lmain;
  Int_t lr1;
} Gcbank_t;

//----------GCLINK 
//      COMMON/GCLINK/JDIGI ,JDRAW ,JHEAD ,JHITS ,JKINE ,JMATE ,JPART 
//     +      ,JROTM ,JRUNG ,JSET  ,JSTAK ,JGSTAT,JTMED ,JTRACK,JVERTX 
//     +      ,JVOLUM,JXYZ  ,JGPAR ,JGPAR2,JSKLT 
typedef struct { 
  Int_t    jdigi; 
  Int_t    jdraw; 
  Int_t    jhead; 
  Int_t    jhits; 
  Int_t    jkine; 
  Int_t    jmate; 
  Int_t    jpart; 
  Int_t    jrotm; 
  Int_t    jrung; 
  Int_t    jset; 
  Int_t    jstak; 
  Int_t    jgstat; 
  Int_t    jtmed; 
  Int_t    jtrack; 
  Int_t    jvertx; 
  Int_t    jvolum; 
  Int_t    jxyz; 
  Int_t    jgpar; 
  Int_t    jgpar2; 
  Int_t    jsklt; 
} Gclink_t; 
 
 
//----------GCFLAG 
//      COMMON/GCFLAG/IDEBUG,IDEMIN,IDEMAX,ITEST,IDRUN,IDEVT,IEORUN 
//     +        ,IEOTRI,IEVENT,ISWIT(10),IFINIT(20),NEVENT,NRNDM(2) 
typedef struct { 
  Int_t    idebug; 
  Int_t    idemin; 
  Int_t    idemax; 
  Int_t    itest; 
  Int_t    idrun; 
  Int_t    idevt; 
  Int_t    ieorun; 
  Int_t    ieotri; 
  Int_t    ievent; 
  Int_t    iswit[10]; 
  Int_t    ifinit[20]; 
  Int_t    nevent; 
  Int_t    nrndm[2]; 
} Gcflag_t; 
 
//----------GCKINE 
//      COMMON/GCKINE/IKINE,PKINE(10),ITRA,ISTAK,IVERT,IPART,ITRTYP 
//     +      ,NAPART(5),AMASS,CHARGE,TLIFE,VERT(3),PVERT(4),IPAOLD 
typedef struct { 
  Int_t    ikine; 
  Float_t  pkine[10]; 
  Int_t    itra; 
  Int_t    istak; 
  Int_t    ivert; 
  Int_t    ipart; 
  Int_t    itrtyp; 
  Int_t    napart[5]; 
  Float_t  amass; 
  Float_t  charge; 
  Float_t  tlife; 
  Float_t  vert[3]; 
  Float_t  pvert[4]; 
  Int_t    ipaold; 
} Gckine_t; 
 
//----------GCKING 
//      COMMON/GCKING/KCASE,NGKINE,GKIN(5,MXGKIN), 
//     +                           TOFD(MXGKIN),IFLGK(MXGKIN) 
#define MXGKIN 100 
typedef struct  { 
  Int_t    kcase; 
  Int_t    ngkine; 
  Float_t  gkin[MXGKIN][5]; 
  Int_t    tofd[MXGKIN]; 
  Int_t    iflgk[MXGKIN]; 
} Gcking_t; 

//----------GCKIN2
//      COMMON/GCKIN2/NGPHOT,XPHOT(11,MXPHOT)
#define MXPHOT 800
typedef struct {
  Int_t ngphot;
  Float_t xphot[MXPHOT][11];
} Gckin2_t;

//----------GCKIN3 
//      COMMON/GCKIN3/GPOS(3,MXGKIN)
typedef struct {
  Float_t gpos[MXGKIN][3];
} Gckin3_t;

//----------GCMATE 
//      COMMON/GCMATE/NMAT,NAMATE(5),A,Z,DENS,RADL,ABSL 
typedef struct { 
  Int_t    nmat; 
  Int_t    namate[5]; 
  Float_t  a; 
  Float_t  z; 
  Float_t  dens; 
  Float_t  radl; 
  Float_t  absl; 
} Gcmate_t; 
 
//----------GCTMED 
//      COMMON/GCTMED/NUMED,NATMED(5),ISVOL,IFIELD,FIELDM,TMAXFD,STEMAX 
//     +      ,DEEMAX,EPSIL,STMIN,CFIELD,PREC,IUPD,ISTPAR,NUMOLD 
typedef struct { 
  Int_t    numed; 
  Int_t    natmed[5]; 
  Int_t    isvol; 
  Int_t    ifield; 
  Float_t  fieldm; 
  Float_t  tmaxfd; 
  Float_t  stemax; 
  Float_t  deemax; 
  Float_t  epsil; 
  Float_t  stmin; 
  Float_t  cfield; 
  Float_t  prec; 
  Int_t    iupd; 
  Int_t    istpar; 
  Int_t    numold; 
} Gctmed_t; 
 
//----------GCTRAK 
#define MAXMEC 30 
//      PARAMETER (MAXMEC=30) 
//      COMMON/GCTRAK/VECT(7),GETOT,GEKIN,VOUT(7),NMEC,LMEC(MAXMEC) 
//     + ,NAMEC(MAXMEC),NSTEP ,MAXNST,DESTEP,DESTEL,SAFETY,SLENG 
//     + ,STEP  ,SNEXT ,SFIELD,TOFG  ,GEKRAT,UPWGHT,IGNEXT,INWVOL 
//     + ,ISTOP ,IGAUTO,IEKBIN, ILOSL, IMULL,INGOTO,NLDOWN,NLEVIN 
//     + ,NLVSAV,ISTORY 
typedef struct { 
  Float_t  vect[7]; 
  Float_t  getot; 
  Float_t  gekin; 
  Int_t    vout[7]; 
  Int_t    nmec; 
  Int_t    lmec[MAXMEC]; 
  Int_t    namec[MAXMEC]; 
  Int_t    nstep; 
  Int_t    maxnst; 
  Float_t  destep; 
  Float_t  destel; 
  Float_t  safety; 
  Float_t  sleng; 
  Float_t  step; 
  Float_t  snext; 
  Float_t  sfield; 
  Float_t  tofg; 
  Float_t  gekrat; 
  Float_t  upwght; 
  Int_t    ignext; 
  Int_t    inwvol; 
  Int_t    istop; 
  Int_t    igauto; 
  Int_t    iekbin; 
  Int_t    ilosl; 
  Int_t    imull; 
  Int_t    ingoto; 
  Int_t    nldown; 
  Int_t    nlevin; 
  Int_t    nlsav; 
  Int_t    istory; 
} Gctrak_t; 
 
//----------GCVOLU 
//      COMMON/GCVOLU/NLEVEL,NAMES(15),NUMBER(15), 
//     +LVOLUM(15),LINDEX(15),INFROM,NLEVMX,NLDEV(15),LINMX(15), 
//     +GTRAN(3,15),GRMAT(10,15),GONLY(15),GLX(3) 
typedef struct { 
  Int_t    nlevel; 
  Int_t    names[15]; 
  Int_t    number[15]; 
  Int_t    lvolum[15]; 
  Int_t    lindex[15]; 
  Int_t    infrom; 
  Int_t    nlevmx; 
  Int_t    nldev[15]; 
  Int_t    linmx[15]; 
  Float_t  gtran[15][3]; 
  Float_t  grmat[15][10]; 
  Float_t  gonly[15]; 
  Float_t  glx[3]; 
} Gcvolu_t; 
 
//----------GCSETS 
//  COMMON/GCSETS/IHSET,IHDET,ISET,IDET,IDTYPE,NVNAME,NUMBV(20) 
typedef struct { 
  Int_t    ihset; 
  Int_t    ihdet; 
  Int_t    iset; 
  Int_t    idet; 
  Int_t    idtype; 
  Int_t    nvname; 
  Int_t    numbv[20]; 
} Gcsets_t; 
 
//----------GCNUM 
//   COMMON/GCNUM/NMATE ,NVOLUM,NROTM,NTMED,NTMULT,NTRACK,NPART 
//  +            ,NSTMAX,NVERTX,NHEAD,NBIT 
typedef struct { 
  Int_t    nmate; 
  Int_t    nvolum; 
  Int_t    nrotm; 
  Int_t    ntmed; 
  Int_t    ntmult; 
  Int_t    ntrack; 
  Int_t    npart; 
  Int_t    nstmax; 
  Int_t    nvertx; 
  Int_t    nhead; 
  Int_t    nbit; 
} Gcnum_t; 
 
//----------GCCUTS 
//  COMMON/GCCUTS/CUTGAM,CUTELE,CUTNEU,CUTHAD,CUTMUO,BCUTE,BCUTM 
//   +             ,DCUTE ,DCUTM ,PPCUTM,TOFMAX,GCUTS(5) 
typedef struct { 
  Float_t cutgam; 
  Float_t cutele; 
  Float_t cutneu; 
  Float_t cuthad; 
  Float_t cutmuo; 
  Float_t bcute; 
  Float_t bcutm; 
  Float_t dcute; 
  Float_t dcutm; 
  Float_t ppcutm; 
  Float_t tofmax; 
  Float_t gcuts[5]; 
} Gccuts_t; 

//----------GCMULO
//      COMMON/GCMULO/SINMUL(101),COSMUL(101),SQRMUL(101),OMCMOL,CHCMOL
//     +  ,EKMIN,EKMAX,NEKBIN,NEK1,EKINV,GEKA,GEKB,EKBIN(200),ELOW(200)
typedef struct {
  Float_t sinmul[101];
  Float_t cosmul[101];
  Float_t sqrmul[101];
  Float_t omcmol;
  Float_t chcmol;
  Float_t ekmin;
  Float_t ekmax;
  Int_t   nekbin;
  Int_t   nek1;
  Float_t ekinv;
  Float_t geka;
  Float_t gekb;
  Float_t ekbin[200];
  Float_t elow[200];
} Gcmulo_t;

//----------GCPHYS
//      COMMON/GCPHYS/IPAIR,SPAIR,SLPAIR,ZINTPA,STEPPA
//     +             ,ICOMP,SCOMP,SLCOMP,ZINTCO,STEPCO
//     +             ,IPHOT,SPHOT,SLPHOT,ZINTPH,STEPPH
//     +             ,IPFIS,SPFIS,SLPFIS,ZINTPF,STEPPF
//     +             ,IDRAY,SDRAY,SLDRAY,ZINTDR,STEPDR
//     +             ,IANNI,SANNI,SLANNI,ZINTAN,STEPAN
//     +             ,IBREM,SBREM,SLBREM,ZINTBR,STEPBR
//     +             ,IHADR,SHADR,SLHADR,ZINTHA,STEPHA
//     +             ,IMUNU,SMUNU,SLMUNU,ZINTMU,STEPMU
//     +             ,IDCAY,SDCAY,SLIFE ,SUMLIF,DPHYS1
//     +             ,ILOSS,SLOSS,SOLOSS,STLOSS,DPHYS2
//     +             ,IMULS,SMULS,SOMULS,STMULS,DPHYS3
//     +             ,IRAYL,SRAYL,SLRAYL,ZINTRA,STEPRA
//      COMMON/GCPHLT/ILABS,SLABS,SLLABS,ZINTLA,STEPLA
//     +             ,ISYNC
//     +             ,ISTRA
typedef struct { 
  Int_t    ipair;
  Float_t  spair;
  Float_t  slpair;
  Float_t  zintpa;
  Float_t  steppa;
  Int_t    icomp;
  Float_t  scomp;
  Float_t  slcomp;
  Float_t  zintco;
  Float_t  stepco;
  Int_t    iphot;
  Float_t  sphot;
  Float_t  slphot;
  Float_t  zintph;
  Float_t  stepph;
  Int_t    ipfis;
  Float_t  spfis;
  Float_t  slpfis;
  Float_t  zintpf;
  Float_t  steppf;
  Int_t    idray;
  Float_t  sdray;
  Float_t  sldray;
  Float_t  zintdr;
  Float_t  stepdr;
  Int_t    ianni;
  Float_t  sanni;
  Float_t  slanni;
  Float_t  zintan;
  Float_t  stepan;
  Int_t    ibrem;
  Float_t  sbrem;
  Float_t  slbrem;
  Float_t  zintbr;
  Float_t  stepbr;
  Int_t    ihadr;
  Float_t  shadr;
  Float_t  slhadr;
  Float_t  zintha;
  Float_t  stepha;
  Int_t    imunu;
  Float_t  smunu;
  Float_t  slmunu;
  Float_t  zintmu;
  Float_t  stepmu;
  Int_t    idcay;
  Float_t  sdcay;
  Float_t  slife;
  Float_t  sumlif;
  Float_t  dphys1;
  Int_t    iloss;
  Float_t  sloss;
  Float_t  soloss;
  Float_t  stloss;
  Float_t  dphys2;
  Int_t    imuls;
  Float_t  smuls;
  Float_t  somuls;
  Float_t  stmuls;
  Float_t  dphys3;
  Int_t    irayl;
  Float_t  srayl;
  Float_t  slrayl;
  Float_t  zintra;
  Float_t  stepra;
} Gcphys_t; 
 
//----------GCOPTI 
//      COMMON/GCOPTI/IOPTIM
typedef struct { 
  Int_t   ioptim;
} Gcopti_t; 
 
//----------GCTLIT 
//      COMMON/GCTLIT/THRIND,PMIN,DP,DNDL,JMIN,ITCKOV,IMCKOV,NPCKOV
typedef struct { 
  Float_t   thrind;
  Float_t   pmin;
  Float_t   dp;
  Float_t   dndl;
  Int_t     jmin;
  Int_t     itckov;
  Int_t     imckov;
  Int_t     npckov;
} Gctlit_t; 
 
//----------GCVDMA 
//      COMMON/GCVDMA/NVMANY,MANYLE(20),MANYNA(20,15),
//     +MANYNU(20,15),NFMANY,MYCOUN,IMYSE,RAYTRA,VECCOS(3)
typedef struct { 
  Int_t     vdma[624];
  Float_t   raytra;
  Float_t   veccos[3];
} Gcvdma_t; 
 
//----------GCTPOL 
#define MAXME1 30 
//      COMMON/GCTPOL/POLAR(3), NAMEC1(MAXME1) 
typedef struct { 
  Float_t polar[3]; 
  Int_t   namec1[MAXME1]; 
} Gctpol_t; 

/************************************************************************
 *                                                                      *
 *      Commons for GEANE                                               *
 *                                                                      *
 ************************************************************************/

//------------ERTRIO
//    INTEGER          MXPRED
//    PARAMETER (MXPRED = 10)
//    DOUBLE PRECISION ERDTRP
//    REAL             ERRIN, ERROUT, ERTRSP, ERXIN, ERXOUT, ERPIN,
//   +                 ERPOUT
//    INTEGER          NEPRED, INLIST, ILPRED, IEPRED
//    COMMON /ERTRIO/  ERDTRP(5,5,MXPRED), ERRIN(15), ERROUT(15,MXPRED),
//   +                 ERTRSP(5,5,MXPRED), ERXIN( 3), ERXOUT( 3,MXPRED),
//   +                 ERPIN(3), ERPOUT(3,MXPRED), NEPRED,INLIST,ILPRED,
//   +                 IEPRED(MXPRED)
//

#define MXPRED 10
typedef struct {
  Double_t erdtrp[MXPRED*5*5];
  Float_t  errin[5];
  Float_t  errout[MXPRED*15];
  Float_t  ertrsp[MXPRED*5*5];
  Float_t  erxin[3];
  Float_t  erxout[MXPRED*3];
  Float_t  erpin[3];
  Float_t  erpout[MXPRED*3];
  Int_t    nepred;
  Int_t    inlist;
  Int_t    ilpred;
  Int_t    iepred;
} Ertrio_t;

//-----------EROTPS
//    CHARACTER*8     CHOPTI
//    LOGICAL         LEEXAC, LELENG, LEONLY, LEPLAN, LEPOIN, LEVOLU
//    REAL            ERPLI, ERPLO, ERLENG
//    INTEGER         NAMEER, NUMVER, IOVLER
//    COMMON /EROPTS/ ERPLI(3,2), ERPLO(3,4,MXPRED), ERLENG(MXPRED),
//   +                NAMEER(MXPRED), NUMVER(MXPRED), IOVLER(MXPRED),
//   +                LEEXAC, LELENG, LEONLY, LEPLAN, LEPOIN, LEVOLU
//    COMMON /EROPTC/CHOPTI

typedef struct {
  Float_t   erpli[3*2];
  Float_t   erplo[MXPRED*3*4];
  Float_t   erleng[MXPRED];
  Int_t     nameer[MXPRED];
  Int_t     numver[MXPRED];
  Int_t     iovler[MXPRED];
  Bool_t    leexac;
  Bool_t    leleng;
  Bool_t    leonly;
  Bool_t    leplan;
  Bool_t    lepoin;
  Bool_t    levolu;
  Bool_t    levmix;
} Eropts_t;

typedef struct {
  char chopti[8];
} Eroptc_t;

//-------ERWORK
//    DOUBLE PRECISION EI, EF, ASDSC
//    COMMON /ERWORK/ EI(15), EF(15), ASDSC(5,5),
//   +                   XI(3), PPI(3), HI(9),
//   +                   XF(3), PF(3),  HF(9),
//   +                   CHTR, DEDX2, BACKTR, CUTEK, TLGCM2, TLRAD

typedef struct {
  Double_t  ei[15];
  Double_t  ef[15];
  Double_t  asdsc[5*5];
  Float_t   xi[3];
  Float_t   ppi[3];
  Float_t   hi[9];
  Float_t   xf[3];
  Float_t   pf[3];
  Float_t   hf[9];
  Float_t   chtr;
  Float_t   dedx2;
  Float_t   backtr;
  Float_t   cutek;
  Float_t   tlgcm2;
  Float_t   tlrad;
} Erwork_t;

/************************************************************************
 *                                                                      *
 *      Commons for GEANE                                               *
 *                                                                      *
 ************************************************************************/
class TVolume;

class TGiant3 : public StarMC { 

protected:
  Int_t fNextVol;      //! Iterator for GeomIter

//--------------Declarations for ZEBRA--------------------- 
  Int_t *fZiq, *fZlq;  //!
  Float_t *fZq;        //!
		          
  Quest_t  *fQuest;    //!
  Gcbank_t *fGcbank;   //!
  Gclink_t *fGclink;   //!
  Gccuts_t *fGccuts;   //!
  Gcmulo_t *fGcmulo;   //!
  Gcmate_t *fGcmate;   //!
  Gctpol_t *fGctpol;   //!
  Gcnum_t  *fGcnum;    //!
  Gcsets_t *fGcsets;   //!
  Gcopti_t *fGcopti;   //!
  Gctlit_t *fGctlit;   //!
  Gcvdma_t *fGcvdma;   //!
  Gcvolu_t *fGcvolu;   //!
  Gckine_t *fGckine;   //!
  Gcflag_t *fGcflag;   //!
  Gctmed_t *fGctmed;   //!
  Gcphys_t *fGcphys;   //!
  Gcking_t *fGcking;   //!
  Gckin2_t *fGckin2;   //!
  Gckin3_t *fGckin3;   //!
  Gctrak_t *fGctrak;   //!   
		          
		          
  // commons for GEANE 
  Ertrio_t *fErtrio;   //!
  Eropts_t *fEropts;   //!
  Eroptc_t *fEroptc;   //!
  Erwork_t *fErwork;   //!

  //Put here all volume names

  char (*fVolNames)[5];           //! Names of geant volumes as C++ chars

  enum {kMaxParticles = 100};

  Int_t fNPDGCodes;    //!

  Int_t fPDGCode[kMaxParticles]; //!

public: 
  TGiant3(); 
  TGiant3(const char *title, Int_t nwgeant=5000000, Int_t nwpaw=0, Int_t iwtype=1);
  virtual ~TGiant3();
  virtual void LoadAddress(); 
 
///////////////////////////////////////////////////////////////////////
//                                                                   //
//                                                                   //
//     Here are the service routines from the geometry               //
//     which could be implemented also in other geometries           //
//                                                                   //
//                                                                   //
///////////////////////////////////////////////////////////////////////

  void  GeomIter();
  Int_t CurrentMaterial(Float_t &a, Float_t &z, Float_t &dens, Float_t &radl, Float_t &absl) const;
  Int_t NextVolUp(Text_t *name, Int_t &copy);
  Int_t CurrentVolID(Int_t &copy) const;
  Int_t CurrentVolOffID(Int_t off, Int_t &copy) const;
  const char* CurrentVolName() const;
  const char *CurrentVolOffName(Int_t off) const;
  Int_t VolId(const Text_t *name) const;
  Int_t IdFromPDG(Int_t pdg) const;
  Int_t PDGFromId(Int_t pdg) const;
  void  DefineParticles();
  const char* VolName(Int_t id) const;
  Float_t Xsec(char*, Float_t, Int_t, Int_t);
  void  TrackPosition(TLorentzVector &xyz) const;
  void  TrackMomentum(TLorentzVector &xyz) const;  
  Int_t NofVolumes() const;
  Float_t TrackTime() const;  
  Float_t TrackCharge() const;
  Float_t TrackMass() const;
  Float_t TrackStep() const;
  Float_t TrackLength() const;
  Int_t   TrackPid() const;
  Bool_t IsTrackInside() const;
  Bool_t IsTrackEntering() const;
  Bool_t IsTrackExiting() const;
  Bool_t IsTrackOut() const;
  Bool_t IsTrackDisappeared() const;
  Bool_t IsTrackStop() const;
  Bool_t IsTrackStarve() const;
  Int_t  NSecondaries() const;
  Int_t  CurrentEvent() const;
  const char*  ProdProcess() const;
  void   GetSecondary(Int_t, Int_t&, TLorentzVector&, TLorentzVector&);
  void   StopTrack();
  void   StopEvent();
  Float_t MaxStep() const;
  void  SetMaxStep(Float_t maxstep);
  void  SetMaxNStep(Int_t maxnstp);
  Int_t GetMaxNStep() const;
  void SetCut(const char* cutName, Float_t cutValue);
  void SetProcess(const char* flagName, Int_t flagValue);
  //  void GetParticle(const Int_t pdg, char *name, Float_t &mass) const;
  virtual Int_t GetMedium() const;
  virtual Float_t Edep() const;
  virtual Float_t Etot() const;

  virtual void    Rndm(Float_t* r, const Int_t n) const;

  virtual void    Material(Int_t&, const char*, Float_t, Float_t, Float_t, Float_t,
			    Float_t, Float_t* buf=0, Int_t nwbuf=0);
  virtual void    Mixture(Int_t&, const char*, Float_t*, Float_t*, Float_t, Int_t, Float_t*);
  virtual void    Medium(Int_t&, const char*, Int_t, Int_t, Int_t, Float_t, Float_t, 
		   Float_t, Float_t, Float_t, Float_t, Float_t* ubuf=0, Int_t nbuf=0);
  virtual void    Matrix(Int_t&, Float_t, Float_t, Float_t, Float_t, Float_t, Float_t);

/////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                         //
//                                                                                         //
//     Here are the interface functions with GEANT3.21                                     //
//                                                                                         //
//                                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////

  // access functions to commons
 
  virtual Quest_t*  Quest()  const {return fQuest;}
  virtual Gcbank_t* Gcbank() const {return fGcbank;}
  virtual Gclink_t* Gclink() const {return fGclink;}
  virtual Gccuts_t* Gccuts() const {return fGccuts;}
  virtual Gcmulo_t* Gcmulo() const {return fGcmulo;}
  virtual Gcmate_t* Gcmate() const {return fGcmate;}
  virtual Gctpol_t* Gctpol() const {return fGctpol;}
  virtual Gcnum_t*  Gcnum()  const {return fGcnum;}
  virtual Gcsets_t* Gcsets() const {return fGcsets;}
  virtual Gcopti_t* Gcopti() const {return fGcopti;}
  virtual Gctlit_t* Gctlit() const {return fGctlit;}
  virtual Gcvdma_t* Gcvdma() const {return fGcvdma;}
  virtual Gcvolu_t* Gcvolu() const {return fGcvolu;}
  virtual Gckine_t* Gckine() const {return fGckine;}
  virtual Gcflag_t* Gcflag() const {return fGcflag;}
  virtual Gctmed_t* Gctmed() const {return fGctmed;}
  virtual Gcphys_t* Gcphys() const {return fGcphys;}
  virtual Gcking_t* Gcking() const {return fGcking;}
  virtual Gckin2_t* Gckin2() const {return fGckin2;}
  virtual Gckin3_t* Gckin3() const {return fGckin3;}
  virtual Gctrak_t* Gctrak() const {return fGctrak;}
  virtual Int_t*    Iq()     const {return fZiq;}
  virtual Int_t*    Lq()     const {return fZlq;}
  virtual Float_t*  Q()      const {return fZq;}


  // Access to GEANE commons

  virtual Ertrio_t* Ertrio() const {return fErtrio;}
  virtual Eropts_t* Eropts() const {return fEropts;}
  virtual Eroptc_t* Eroptc() const {return fEroptc;}
  virtual Erwork_t* Erwork() const {return fErwork;}



      // functions from GBASE 
   virtual  void  Gpcxyz(); 
   virtual  void  Ggclos(); 
   virtual  void  SetColors() {}
   virtual  void  Gfile(const char *filename, const char *option="I"); 
   virtual  void  Glast(); 
   virtual  void  Gprint(const char *name); 
   virtual  void  Grun(); 
   virtual  void  Gtrig(); 
   virtual  void  Gtrigc(); 
   virtual  void  Gtrigi(); 
   virtual  void  Gwork(Int_t nwork); 
   virtual  void  Gzinit(); 
 
      // functions from GCONS 
   virtual  void  Gfmate(Int_t imat, char *name, Float_t &a, Float_t &z, Float_t &dens, 
                         Float_t &radl, Float_t &absl, Float_t* ubuf, Int_t& nbuf); 
   virtual  void  Gfpart(Int_t ipart, char *name, Int_t &itrtyp,  
                         Float_t &amass, Float_t &charge, Float_t &tlife); 
   virtual  void  Gftmed(Int_t numed, char *name, Int_t &nmat, Int_t &isvol,  
                         Int_t &ifield, Float_t &fieldm, Float_t &tmaxfd, 
                         Float_t &stemax, Float_t &deemax, Float_t &epsil, 
                         Float_t &stmin, Float_t *buf=0, Int_t *nbuf=0);
   virtual  void  Gftmat(Int_t imate, Int_t ipart, char *chmeca, Int_t kdim,
			 Float_t* tkin, Float_t* value, Float_t* pcut,
			 Int_t &ixst);
   virtual  Float_t Gbrelm(Float_t z, Float_t t, Float_t cut);
   virtual  Float_t Gprelm(Float_t z, Float_t t, Float_t cut);
   virtual  void  Gmate(); 
   virtual  void  Gpart(); 
   virtual  void  Gsckov(Int_t itmed, Int_t npckov, Float_t *ppckov,
			 Float_t *absco, Float_t *effic, Float_t *rindex); 
   virtual  void  Gsdk(Int_t ipart, Float_t *bratio, Int_t *mode); 
   virtual  void  Gsmate(Int_t imat, const char *name, Float_t a, Float_t z,  
                         Float_t dens, Float_t radl, Float_t absl); 
   virtual  void  Gsmixt(Int_t imat, const char *name, Float_t *a, Float_t *z,  
                         Float_t dens, Int_t nlmat, Float_t *wmat); 
   virtual  void  Gspart(Int_t ipart, const char *name, Int_t itrtyp,  
                         Float_t amass, Float_t charge, Float_t tlife); 
   virtual  void  Gstmed(Int_t numed, const char *name, Int_t nmat, Int_t isvol,  
                         Int_t ifield, Float_t fieldm, Float_t tmaxfd, 
                         Float_t stemax, Float_t deemax, Float_t epsil, 
                         Float_t stmin); 
   virtual  void  Gstpar(Int_t itmed, const char *param, Float_t parval); 
 
      // functions from GKINE 
   virtual  void  Gfkine(Int_t itra, Float_t *vert, Float_t *pvert, 
                         Int_t &ipart, Int_t &nvert); 
   virtual  void  Gfvert(Int_t nvtx, Float_t *v, Int_t &ntbeam, Int_t &nttarg, Float_t &tofg); 
   virtual  Int_t Gskine(Float_t *plab, Int_t ipart, Int_t nv, Float_t *ubuf=0, Int_t nwbuf=0); 
   virtual  Int_t Gsvert(Float_t *v, Int_t ntbeam, Int_t nttarg, Float_t *ubuf=0, Int_t nwbuf=0); 
 
      // functions from GPHYS 
   virtual  void  Gphysi(); 
 
      // functions from GTRAK 
   virtual  void  Gdebug(); 
   virtual  void  Gekbin(); 
   virtual  void  Gfinds(); 
   virtual  void  Gsking(Int_t igk); 
   virtual  void  Gskpho(Int_t igk); 
   virtual  void  Gsstak(Int_t iflag); 
   virtual  void  Gsxyz(); 
   virtual  void  Gtrack(); 
   virtual  void  Gtreve(); 
   virtual  void  Gtreve_root(); 
   virtual  void  Grndm(Float_t *rvec, const Int_t len) const; 
   virtual  void  Grndmq(Int_t &is1, Int_t &is2, const Int_t iseq, const Text_t *chopt); 
 
      // functions from GGEOM 
   virtual  void  Gdxyz(Int_t ); 
   virtual  void  Gdcxyz(); 

      // functions from GGEOM 
   virtual  void  Gdtom(Float_t *xd, Float_t *xm, Int_t &iflag); 
   virtual  void  Glmoth(const char* iudet, Int_t iunum, Int_t &nlev, 
                         Int_t *lvols, Int_t *lindx); 
   virtual  void  Gmedia(Float_t *x, Int_t &numed); 
   virtual  void  Gtmedi(Float_t *x, Int_t &numed); 
   virtual  void  Gmtod(Float_t *xm, Float_t *xd, Int_t &iflag); 
   virtual  void  Gsdvn(const char *name, const char *mother, Int_t ndiv, Int_t iaxis); 
   virtual  void  Gsdvn2(const char *name, const char *mother, Int_t ndiv, Int_t iaxis, Float_t c0i, Int_t numed); 
   virtual  void  Gsdvs(const char *name, const char *mother, Float_t step, Int_t iaxis, Int_t numed); 
   virtual  void  Gsdvs2(const char *name, const char *mother, Float_t step, Int_t iaxis, Float_t c0, Int_t numed); 
   virtual  void  Gsdvt(const char *name, const char *mother, Float_t step, Int_t iaxis, Int_t numed, Int_t ndvmx); 
   virtual  void  Gsdvt2(const char *name, const char *mother, Float_t step, Int_t iaxis,
			 Float_t c0, Int_t numed, Int_t ndvmx); 
   virtual  void  Gsord(const char *name, Int_t iax); 
   virtual  void  Gspos(const char *name, Int_t nr, const char *mother,  
                         Float_t x, Float_t y, Float_t z, Int_t irot, const char *konly="ONLY"); 
   virtual  void  Gsposp(const char *name, Int_t nr, const char *mother,  
                         Float_t x, Float_t y, Float_t z, Int_t irot, const char *konly, Float_t *upar, Int_t np); 
   virtual  void  Gsrotm(Int_t nmat, Float_t theta1, Float_t phi1, Float_t theta2, Float_t phi2, 
                         Float_t theta3, Float_t phi3); 
   virtual  void  Gprotm(Int_t nmat=0); 
   virtual  Int_t Gsvolu(const char *name, const char *shape, Int_t nmed,  
                         Float_t *upar, Int_t np); 
   virtual  Int_t Glvolu(const Int_t Nlev, Int_t *Lnam, Int_t *Lnum);  

   virtual  void  Gsatt(const char *name, const char *att, Int_t val);
   virtual  void  Gfpara(const char *name, Int_t number, Int_t intext, Int_t& npar,
			 Int_t& natt, Float_t* par, Float_t* att);
   virtual  void  Gckpar(Int_t, Int_t, Float_t*);
   virtual  void  Gckmat(Int_t, char*);
    
      // functions from GDRAW 
   virtual  void  DefaultRange();
   virtual  void  InitHIGZ();
   virtual  void  Gdopen(Int_t view);
   virtual  void  Gdclose();
   virtual  void  Gdelete(Int_t view);
   virtual  void  Gdshow(Int_t view);
   virtual  void  Gdopt(const char *name,const char *value);
   virtual  void  Gdraw(const char *name,Float_t theta=30, Float_t phi=30, Float_t psi=0,Float_t u0=10,Float_t v0=10,Float_t ul=0.01,Float_t vl=0.01);
   virtual  void  Gdrawc(const char *name,Int_t axis=1, Float_t cut=0,Float_t u0=10,Float_t v0=10,Float_t ul=0.01,Float_t vl=0.01);
   virtual  void  Gdrawx(const char *name,Float_t cutthe, Float_t cutphi, Float_t cutval,
                         Float_t theta=30, Float_t phi=30,Float_t u0=10,Float_t v0=10,Float_t ul=0.01,Float_t vl=0.01);
   virtual  void  Gdhead(Int_t isel, const char *name, Float_t chrsiz=0.6);   
   virtual  void  Gdman(Float_t u0, Float_t v0, const char *type="MAN");
   virtual  void  Gdspec(const char *name);
   virtual  void  DrawOneSpec(const char *name);
   virtual  void  Gdtree(const char *name,Int_t levmax=15,Int_t ispec=0);
   virtual  void  GdtreeParent(const char *name,Int_t levmax=15,Int_t ispec=0);

   virtual  void  WriteEuclid(const char*, const char*, Int_t, Int_t);

   virtual  void  SetABAN(Int_t par=1);
   virtual  void  SetANNI(Int_t par=1);
   virtual  void  SetAUTO(Int_t par=1);
   virtual  void  SetBOMB(Float_t bomb=1);
   virtual  void  SetBREM(Int_t par=1);
   virtual  void  SetCKOV(Int_t par=1);
   virtual  void  SetClipBox(const char *name,Float_t xmin=-9999,Float_t xmax=0, Float_t ymin=-9999,Float_t ymax=0,Float_t zmin=-9999,Float_t zmax=0);
   virtual  void  SetCOMP(Int_t par=1);
   virtual  void  SetCUTS(Float_t cutgam,Float_t cutele,Float_t cutneu,Float_t cuthad,
                      Float_t cutmuo ,Float_t bcute ,Float_t bcutm ,Float_t dcute ,
                      Float_t dcutm ,Float_t ppcutm, Float_t tofmax);
   virtual  void  SetDCAY(Int_t par=1);
   virtual  void  SetDEBU(Int_t emin=1, Int_t emax=999, Int_t emod=1);
   virtual  void  SetDRAY(Int_t par=1);
   virtual  void  SetERAN(Float_t ekmin=1.e-5, Float_t ekmax=1.e4,
			  Int_t nekbin=90);
   virtual  void  SetHADR(Int_t par=1);
   virtual  void  SetKINE(Int_t kine, Float_t xk1=0, Float_t xk2=0, Float_t xk3=0, Float_t xk4=0,
                         Float_t xk5=0, Float_t xk6=0, Float_t xk7=0, Float_t xk8=0, Float_t xk9=0,
                         Float_t xk10=0);
   virtual  void  SetLOSS(Int_t par=2);
   virtual  void  SetMULS(Int_t par=1);
   virtual  void  SetMUNU(Int_t par=1);
   virtual  void  SetOPTI(Int_t par=2);
   virtual  void  SetPAIR(Int_t par=1);
   virtual  void  SetPFIS(Int_t par=1);
   virtual  void  SetPHOT(Int_t par=1);
   virtual  void  SetRAYL(Int_t par=1);
   virtual  void  SetSWIT(Int_t sw, Int_t val=1);
   virtual  void  SetTRIG(Int_t nevents=1);
   virtual  void  SetUserDecay(Int_t ipart);

   virtual  void  Vname(const char *name, char *vname);

   virtual  void  InitLego();

  // Routines from GEANE

   virtual void Ertrgo();
   virtual Int_t Ertrak(Float_t *x1, Float_t *p1, 
		        Float_t *x2, Float_t *p2,
			Int_t &ipa,  Option_t *chopt);
   virtual void Eufill(Int_t &n, Float_t* ein, Float_t* xlf);
   virtual void Eufilp(Int_t &n, Float_t* ein, Float_t* pli, Float_t * plf);
   virtual void Eufilv(Int_t &n, Float_t* ein, const Char_t *cnamv, Int_t *numv, Int_t *Iovl);
   virtual void     Agmain(Int_t &nwgeant,Int_t &nwpaw,Int_t &iwtype);
   virtual Float_t* Gufld(Float_t *x, Float_t *bf);  
   virtual void     Agxuser();
   virtual void     Aginit(const Char_t *command);
   virtual void     Aginit(Int_t &nwgeant, Int_t &nwpaw, Int_t &iwtype);
   virtual void     Agxinit();
   virtual void     Agpawq();
   virtual void     GetKuipPrompt() {Agpawq();}
   virtual void     Agexit();
   virtual void     Kuexel(const char* line);
   virtual void     Kuexec(const char* line);
   virtual void     Gfrotm(Int_t & Nmat, 
			   Float_t &Theta1, Float_t & Phi1,
			   Float_t &Theta2, Float_t & Phi2,
			   Float_t &Theta3, Float_t & Phi3);
#if 0
   virtual Bool_t   Agsens(const Char_t *name);
#endif
   virtual void     Gfnhit(const Char_t *cset, const Char_t *cdet, Int_t &nhits);
   virtual void     Gfrung(Int_t & Nwrung, Int_t *Irung, Int_t & Nwbuf, Float_t * Ubuf);
   virtual void     Gfhead(Int_t & Nwhead, Int_t *Ihead, Int_t & Nwbuf, Float_t * Ubuf);
   virtual void     Gfpath(Int_t &iSet, Int_t &iDet, Int_t *numBv, Int_t & nLev, Int_t *lNam, Int_t *lNum);
#if 0
   virtual ULong_t  Csaddr(const Char_t *name);
   virtual Int_t    Csjcal(
                            addrfun *fun, /* addres of external routine */
                            Int_t  &narg,   /* number   of arguments      */
                            ...);         /* other narg arguments       */
#endif
   static TGiant3  *Geant3(){ return fgGeant;}
   static TGiant3  *fgGeant; 
   virtual void FinishGeometry();
   virtual void BuildPhysics();

   ClassDef(TGiant3,0)  //C++ interface to Geant basic routines 
}; 

#endif 
