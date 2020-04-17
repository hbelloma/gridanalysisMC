#ifndef ALIANATASKLINEARSTPP_H
#define ALIANATASKLINEARSTPP_H

// example of an analysis task creating a p_t spectrum
// Authors: Panos Cristakoglou, Jan Fiete Grosse-Oetringhaus, Christian Klein-Boesing

class TList;
class TH1F;
class TH2F;
class TProfile;
class AliESDtrackCuts;

#include "AliAnalysisTaskSE.h"

class AliAnaTaskLinearStpp : public AliAnalysisTaskSE {
 public:
  AliAnaTaskLinearStpp(const char *name="<default name>");
  virtual ~AliAnaTaskLinearStpp() { /*if (fOutputList) delete fOutputList;*/}
  
  virtual void   UserCreateOutputObjects();
  virtual void   UserExec(Option_t* option);
  virtual void   Terminate(Option_t *);
  virtual void   SetCuts(AliESDtrackCuts* cuts=0)    {fCuts = cuts;}


  void LoopESD();
  void LoopESDMC();
  
  void UseMC(Bool_t useMC=kTRUE) { fUseMC = useMC;}
  
 private:
  AliESDtrackCuts* fCuts;                   // List of cuts for ESDs
  Bool_t       fUseMC;
  AliESDEvent *fMyESDEvent; // esd event
  
  TList	      *fOutputList; // output list
  TH1F        *fHistPt;	//Pt spectrum ESD
  TH1F        *fHistPtMC;	//Pt spectrum MC
  TH1F        *fn;   
  TH1F        *fnMC;  
  TH1F        *fnMultBin;   
  TH1F        *fnMultBing; 


  TH1F *fstLesdA[80];
  TProfile *fstLNchA;
  TProfile *fptNchA;
  TH1F     *fHistPtA;
  TH2F  *fRespMNchA;
  TH2F  *fRespMStLA[80];
  TProfile *fstLNchAMC;
  TProfile *fptNchAMC;
  TH1F     *fHistPtAMC;


  TH1F *fstLesdS[80];
  TProfile *fstLNchS;
  TProfile *fptNchS;
  TH1F     *fHistPtS;
  TH2F  *fRespMNchS;
  TH2F  *fRespMStLS[80];
  TProfile *fstLNchSMC;
  TProfile *fptNchSMC;
  TH1F     *fHistPtSMC;

  TH1F *fstLesdH[80];
  TProfile *fstLNchH;
  TProfile *fptNchH;
  TH1F     *fHistPtH;
  TH2F  *fRespMNchH;
  TH2F  *fRespMStLH[80];
  TProfile *fstLNchHMC;
  TProfile *fptNchHMC;
  TH1F     *fHistPtHMC;

  TProfile *fptNchJ;
  TH1F     *fHistPtJ;
  TH2F  *fRespMNchJ;
  TProfile *fptNchJMC;
  TH1F     *fHistPtJMC;


  TProfile *fptNchI;
  TH1F     *fHistPtI;
  TH2F  *fRespMNchI;
  TProfile *fptNchIMC;
  TH1F     *fHistPtIMC;


  TH1F  *fstL[6];
  TH1F  *fptJeBinNch[6];
  TH1F  *fptAvBinNch[6];
  TH1F  *fptIsBinNch[6];

  TH1F  *fTstL[6];
  TH1F  *fTptJeBinNch[6];
  TH1F  *fTptAvBinNch[6];
  TH1F  *fTptIsBinNch[6];
  TH2F  *fRespMStLNch[6];

  TH1F  *fstLMC[6];
  TH1F  *fptJeBinNchMC[6];
  TH1F  *fptAvBinNchMC[6];
  TH1F  *fptIsBinNchMC[6];

  TH1F  *fNchESD;
  TH1F  *fNchMC;
  TH1F  *fNchg;

  AliAnaTaskLinearStpp(const AliAnaTaskLinearStpp&); // not implemented
  AliAnaTaskLinearStpp& operator=(const AliAnaTaskLinearStpp&); // not implemented
  
  ClassDef(AliAnaTaskLinearStpp, 1); // example of analysis
};

#endif
