/*
  22/04/2011
  This is the official task for the transverse sphericity analysis 
  Author: Antonio Ortiz, antonio.ortiz@nucleares.unam.mx
*/


#include <TChain.h>
#include <TList.h>

#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TCanvas.h>

#include "AliVEvent.h"
#include "AliVParticle.h"

#include "AliESDEvent.h"
#include "AliESDtrack.h"
#include "AliESDtrackCuts.h"

#include "AliMCEvent.h"
#include "AliMCParticle.h"
#include <AliStack.h>
#include <AliHeader.h>
#include <AliGenEventHeader.h>
#include "AliAnalysisManager.h"

#include "AliESDInputHandler.h"


#include "AliAnaTaskLinearStpp.h"

ClassImp(AliAnaTaskLinearStpp)

//________________________________________________________________________
AliAnaTaskLinearStpp::AliAnaTaskLinearStpp(const char *name)
  : AliAnalysisTaskSE(name),
  fCuts(0),
  fUseMC(kFALSE),
  fMyESDEvent(0),
  fOutputList(0),
  fHistPt(0),
  fn(0),
  fHistPtMC(0),
  fnMC(0),
  fnMultBin(0),
  fnMultBing(0),

  fstLNchA(0),
  fptNchA(0),
  fHistPtA(0),
  fRespMNchA(0),
  fstLNchAMC(0),
  fptNchAMC(0),
  fHistPtAMC(0),


  fstLNchS(0),
  fptNchS(0),
  fHistPtS(0),
  fRespMNchS(0),
  fstLNchSMC(0),
  fptNchSMC(0),
  fHistPtSMC(0),

  fstLNchH(0),
  fptNchH(0),
  fHistPtH(0),
  fRespMNchH(0),
  fstLNchHMC(0),
  fptNchHMC(0),
  fHistPtHMC(0),

  fptNchJ(0),
  fHistPtJ(0),
  fRespMNchJ(0),
  fptNchJMC(0),
  fHistPtJMC(0),

  fptNchI(0),
  fHistPtI(0),
  fRespMNchI(0),
  fptNchIMC(0),
  fHistPtIMC(0),


  fNchESD(0),
  fNchMC(0),
  fNchg(0)


{
  // Constructor

  for(Int_t i=0;i<80;++i){
 
    fstLesdA[i]=0;
    fstLesdS[i]=0;
    fstLesdH[i]=0;

    fRespMStLA[i]=0;
    fRespMStLS[i]=0;
    fRespMStLH[i]=0;

  }
  for(Int_t i=0;i<6;++i){
    //only raw or mc rec
    fstL[i]=0;
    fptJeBinNch[i]=0;
    fptAvBinNch[i]=0;
    fptIsBinNch[i]=0;

    //in a given rec event we look at the information at generation level
    fTstL[i]=0;
    fTptJeBinNch[i]=0;
    fTptAvBinNch[i]=0;
    fTptIsBinNch[i]=0;
    fRespMStLNch[i]=0;

    //pure generator
    fstLMC[i]=0;
    fptJeBinNchMC[i]=0;
    fptAvBinNchMC[i]=0;
    fptIsBinNchMC[i]=0;
  }





  DefineOutput(1, TList::Class());
}

//________________________________________________________________________
void AliAnaTaskLinearStpp::UserCreateOutputObjects()
{
  // Create histograms
  // Called once
  fHistPt = new TH1F("fHistPt", "P_{T} distribution REC", 15, 0.1, 3.1);
  
  fn = new TH1F("fn", "fn", 20, -1, 19);
  fnMultBin = new TH1F("fnMultBin", "fnMultBin", 8, -1, 7);

  for(Int_t i=0;i<6;++i){

    fstL[i]=new TH1F(Form("fstL_%d",i),Form("fstL_%d",i),100,0,1);
    fptJeBinNch[i]=new TH1F(Form("fptJeBinNch_%d",i),Form("fptJeBinNch_%d",i),1500,0,150);
    fptAvBinNch[i]=new TH1F(Form("fptAvBinNch_%d",i),Form("fptAvBinNch_%d",i),1500,0,150);
    fptIsBinNch[i]=new TH1F(Form("fptIsBinNch_%d",i),Form("fptIsBinNch_%d",i),1500,0,150);

  }

  fNchESD=new TH1F("fNchESD","fNchESD",80,3,83);

  for(Int_t i=0;i<80;++i){

    fstLesdA[i]=new TH1F(Form("fstLesdA_%d",i),Form("fstLesdA_%d",i),100,0,1);

  }

  fstLNchA = new TProfile("fstLNchA", "fstLNchA", 100,0,100,0,10);
  fptNchA = new TProfile("fptNchA", "fptNchA", 100,0,100,0,10);
  fHistPtA = new TH1F("fHistPtA", "fHistPtA", 200, 0.0, 100);


  for(Int_t i=0;i<80;++i){

    fstLesdS[i]=new TH1F(Form("fstLesdS_%d",i),Form("fstLesdS_%d",i),100,0,1);

  }

  fstLNchS = new TProfile("fstLNchS", "fstLNchS", 100,0,100,0,10);
  fptNchS = new TProfile("fptNchS", "fptNchS", 100,0,100,0,10);
  fHistPtS = new TH1F("fHistPtS", "fHistPtS", 200, 0.0, 100);


  for(Int_t i=0;i<80;++i){

    fstLesdH[i]=new TH1F(Form("fstLesdH_%d",i),Form("fstLesdH_%d",i),100,0,1);

  }

  fstLNchH = new TProfile("fstLNchH", "fstLNchH", 100,0,100,0,10);

  fptNchH = new TProfile("fptNchH", "fptNchH", 100,0,100,0,10);
  fHistPtH = new TH1F("fHistPtH", "fHistPtH", 200, 0.0, 100);

  fptNchJ = new TProfile("fptNchJ", "fptNchJ", 100,0,100,0,10);
  fHistPtJ = new TH1F("fHistPtJ", "fHistPtJ", 200, 0.0, 100);

  fptNchI = new TProfile("fptNchI", "fptNchI", 100,0,100,0,10);
  fHistPtI = new TH1F("fHistPtI", "fHistPtI", 200, 0.0, 100);


  if (fUseMC) {
    fHistPtMC = new TH1F("fHistPtMC", "P_{T} distribution MC", 15, 0.1, 3.1);    
    fNchMC=new TH1F("fNchMC","fNchMC",80,3,83);
    fNchg =new TH1F("fNchg","fNchg",80,3,83);


    for(Int_t i=0;i<80;++i){

      fRespMStLA[i]=new TH2F(Form("fRespMStLA_%d",i),Form("fRespMStLA_%d",i),100,0,1,100,0,1);

    }
    fRespMNchA=new TH2F("fRespMNchA","fRespMNchA",100,0,100,100,0,100);
    
    
    for(Int_t i=0;i<80;++i){

      fRespMStLS[i]=new TH2F(Form("fRespMStLS_%d",i),Form("fRespMStLS_%d",i),100,0,1,100,0,1);

    }
    fRespMNchS=new TH2F("fRespMNchS","fRespMNchS",100,0,100,100,0,100);
    
    
    for(Int_t i=0;i<80;++i){
  
      fRespMStLH[i]=new TH2F(Form("fRespMStLH_%d",i),Form("fRespMStLH_%d",i),100,0,1,100,0,1);

    }
    fRespMNchH=new TH2F("fRespMNchH","fRespMNchH",100,0,100,100,0,100);
    
    fRespMNchJ=new TH2F("fRespMNchJ","fRespMNchJ",100,0,100,100,0,100);
    
    fRespMNchI=new TH2F("fRespMNchI","fRespMNchI",100,0,100,100,0,100);


    fnMC = new TH1F("fnMC", "fnMC", 20, -1, 19);
    fnMultBing = new TH1F("fnMultBing", "fnMultBing", 8, -1, 7);


    for(Int_t i=0;i<6;++i){
  
      fTstL[i]=new TH1F(Form("fTstL_%d",i),Form("fTstL_%d",i),100,0,1);
      fTptJeBinNch[i]=new TH1F(Form("fTptJeBinNch_%d",i),Form("fTptJeBinNch_%d",i),1500,0,150);
      fTptAvBinNch[i]=new TH1F(Form("fTptAvBinNch_%d",i),Form("fTptAvBinNch_%d",i),1500,0,150);
      fTptIsBinNch[i]=new TH1F(Form("fTptIsBinNch_%d",i),Form("fTptIsBinNch_%d",i),1500,0,150);
      fRespMStLNch[i]=new TH2F(Form("fRespMStLNch_%d",i),Form("fRespMStLNch_%d",i),100,0,1,100,0,1);

      fstLMC[i]=new TH1F(Form("fstLMC_%d",i),Form("fstLMC_%d",i),100,0,1);
      fptJeBinNchMC[i]=new TH1F(Form("fptJeBinNchMC_%d",i),Form("fptJeBinNchMC_%d",i),1500,0,150);
      fptAvBinNchMC[i]=new TH1F(Form("fptAvBinNchMC_%d",i),Form("fptAvBinNchMC_%d",i),1500,0,150);
      fptIsBinNchMC[i]=new TH1F(Form("fptIsBinNchMC_%d",i),Form("fptIsBinNchMC_%d",i),1500,0,150);
 
    }

    fstLNchAMC = new TProfile("fstLNchAMC", "fstLNchAMC", 100,0,100,0,10);

    fptNchAMC = new TProfile("fptNchAMC", "fptNchAMC", 100,0,100,0,10);
    fHistPtAMC = new TH1F("fHistPtAMC", "fHistPtAMC", 200, 0.0, 100);

    fstLNchSMC = new TProfile("fstLNchSMC", "fstLNchSMC", 100,0,100,0,10);
    fptNchSMC = new TProfile("fptNchSMC", "fptNchSMC", 100,0,100,0,10);
    fHistPtSMC = new TH1F("fHistPtSMC", "fHistPtSMC", 200, 0.0, 100);

    fstLNchHMC = new TProfile("fstLNchHMC", "fstLNchHMC", 100,0,100,0,10);
    fptNchHMC = new TProfile("fptNchHMC", "fptNchHMC", 100,0,100,0,10);
    fHistPtHMC = new TH1F("fHistPtHMC", "fHistPtHMC", 200, 0.0, 100);

    fptNchJMC = new TProfile("fptNchJMC", "fptNchJMC", 100,0,100,0,10);
    fHistPtJMC = new TH1F("fHistPtJMC", "fHistPtJMC", 200, 0.0, 100);
    
    fptNchIMC = new TProfile("fptNchIMC", "fptNchIMC", 100,0,100,0,10);
    fHistPtIMC = new TH1F("fHistPtIMC", "fHistPtIMC", 200, 0.0, 100);

  }

  // Create histograms
  // Called once

  fOutputList = new TList();
  fOutputList->SetOwner();


  fOutputList->Add(fHistPt);
  fOutputList->Add(fn);
  fOutputList->Add(fnMultBin);

  for(Int_t i=0;i<6;++i){

    fOutputList->Add(fstL[i]);
    fOutputList->Add(fptJeBinNch[i]);
    fOutputList->Add(fptAvBinNch[i]);
    fOutputList->Add(fptIsBinNch[i]);

  }

  fOutputList->Add(fNchESD);

  for(Int_t i=0;i<80;++i){

    fOutputList->Add(fstLesdA[i]);

  }

  fOutputList->Add(fstLNchA);
  fOutputList->Add(fptNchA);
  fOutputList->Add(fHistPtA);

  for(Int_t i=0;i<80;++i){

    fOutputList->Add(fstLesdS[i]);
  }

  fOutputList->Add(fstLNchS);
  fOutputList->Add(fptNchS);
  fOutputList->Add(fHistPtS);

  for(Int_t i=0;i<80;++i){

    fOutputList->Add(fstLesdH[i]);

  }
 
  fOutputList->Add(fstLNchH);
  fOutputList->Add(fptNchH);
  fOutputList->Add(fHistPtH);

  fOutputList->Add(fptNchJ);
  fOutputList->Add(fHistPtJ);

  fOutputList->Add(fptNchI);
  fOutputList->Add(fHistPtI);


  if (fUseMC) {

    fOutputList->Add(fHistPtMC);     
    fOutputList->Add(fNchMC);
    fOutputList->Add(fNchg);



    for(Int_t i=0;i<80;++i){

      fOutputList->Add(fRespMStLA[i]);

    }
    fOutputList->Add(fRespMNchA);
    
    
    for(Int_t i=0;i<80;++i){

      fOutputList->Add(fRespMStLS[i]);

    }
    fOutputList->Add(fRespMNchS);
    
    
    for(Int_t i=0;i<80;++i){

      fOutputList->Add(fRespMStLH[i]);

    }
    fOutputList->Add(fRespMNchH);
    fOutputList->Add(fRespMNchJ);
    fOutputList->Add(fRespMNchI);

    fOutputList->Add(fnMC);
    fOutputList->Add(fnMultBing);


    for(Int_t i=0;i<6;++i){

      fOutputList->Add(fTstL[i]);
      fOutputList->Add(fTptJeBinNch[i]);
      fOutputList->Add(fTptAvBinNch[i]);
      fOutputList->Add(fTptIsBinNch[i]);
      fOutputList->Add(fRespMStLNch[i]);

      fOutputList->Add(fstLMC[i]);
      fOutputList->Add(fptJeBinNchMC[i]);
      fOutputList->Add(fptAvBinNchMC[i]);
      fOutputList->Add(fptIsBinNchMC[i]);
 
    }

    fOutputList->Add(fstLNchAMC);
    fOutputList->Add(fptNchAMC);
    fOutputList->Add(fHistPtAMC);

    fOutputList->Add(fstLNchSMC);
    fOutputList->Add(fptNchSMC);
    fOutputList->Add(fHistPtSMC);

    fOutputList->Add(fstLNchHMC);
    fOutputList->Add(fptNchHMC);
    fOutputList->Add(fHistPtHMC);

    fOutputList->Add(fptNchJMC);
    fOutputList->Add(fHistPtJMC);
    
    fOutputList->Add(fptNchIMC);
    fOutputList->Add(fHistPtIMC);


  }


 PostData(1, fOutputList);



  
}

//________________________________________________________________________
void AliAnaTaskLinearStpp::UserExec(Option_t *)
{
  // Main loop
  // Called for each event

  AliVEvent *event = InputEvent();
  if (!event) {
     Error("UserExec", "Could not retrieve event");
     return;
  }
  
  fMyESDEvent = dynamic_cast<AliESDEvent*> (InputEvent());
  if (fMyESDEvent) {
    LoopESD();
    if (fUseMC)
      LoopESDMC();
  }



  // Post output data.
  PostData(1, fOutputList);

}      

//________________________________________________________________________
void AliAnaTaskLinearStpp::Terminate(Option_t*)
{
  // Draw result to the screen
  // Called once at the end of the query

  fOutputList = dynamic_cast<TList*> (GetOutputData(1));
  if (!fOutputList) {
    Error("Terminate","fOutputList not available");
    return;
  }

  fHistPt = (TH1F*) fOutputList->FindObject("fHistPt");
  if (!fHistPt) {
    Error("Terminate","fHistPt not available");
    return;
  }
  
  Int_t pixelX = 350;
  if (fUseMC) pixelX = 700;
  
  TCanvas *c1 = new TCanvas(GetName(),"Pt",10,10,pixelX,350);
  if (fUseMC) c1->Divide(2,1);
  
  c1->cd(1)->SetLogy();
  if (fHistPt)
    fHistPt->DrawCopy("E");
  
  if (fUseMC) {
    fHistPtMC = (TH1F*) fOutputList->FindObject("fHistPtMC");
    if (!fHistPtMC) {
      Error("Terminate","fHistPtMC not available");
      return;
    }
    c1->cd(2)->SetLogy();
    fHistPtMC->DrawCopy("E");
  }
}

//________________________________________________________________________
void AliAnaTaskLinearStpp::LoopESD()
{

  /////////////////////////////////////////////////////////////////////////////////
  Double_t etacutoffa=0.8;
  Double_t ptcutoffa=0.5;
  Bool_t neutral=kFALSE;
  Int_t nana=1;
  ////////////////////////////////////////////////////////////////////////////////
  

  if (fUseMC==kFALSE){ 

    fn->Fill(0);
    
    Int_t ntracks = fMyESDEvent->GetNumberOfTracks();
    Printf("ESD tracks: %d", ntracks);
    if(ntracks==0)return;
    
    fn->Fill(1);


    Bool_t isSelected = (((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & AliVEvent::kMB);
    if(isSelected==kFALSE)return;
    
    fn->Fill(2);

    //accept event only, if vertex is good and is within fVertexZcut region
    const AliESDVertex*	vtxESD   = fMyESDEvent->GetPrimaryVertex();
    if(vtxESD->GetNContributors()<1)return;
    Float_t fVz= vtxESD->GetZ();
    if(TMath::Abs(fVz)>10) return;
    
    fn->Fill(3);


    Bool_t pileup=fMyESDEvent->IsPileupFromSPD(3, 0.8);// This function checks if there was a pile up reconstructed with SPD
    if(pileup==kTRUE){Printf("pile up, skip");return;}
    
    fn->Fill(4);
    
    TObjArray* acceptedtracks = new TObjArray();	
    Int_t nesdtracks=0;
    Double_t maxipttracks=0;
    Double_t sumapt=0;
    
    //linearized
    Double_t s00l=0;
    Double_t s01l=0;
    Double_t s10l=0;
    Double_t s11l=0;


    // Track loop
    for (Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
      AliESDtrack *track = (AliESDtrack *)fMyESDEvent->GetTrack(iTracks);
      if (!track) {
	Error("UserExec", "Could not receive track %d", iTracks);
	continue;
      }

 

      if (!fCuts->AcceptTrack(track))continue;
      if (TMath::Abs(track->Eta())>etacutoffa)continue;
      if(track->Pt()<ptcutoffa)continue;
      if(track->Pt()>maxipttracks) maxipttracks=track->Pt();

      Printf("p_T=%f",track->Pt());

      sumapt+=track->Pt();
      
      s00l +=  (track->Px() * track->Px())/track->Pt();
      s01l +=  (track->Px() * track->Py())/track->Pt();
      s10l +=  (track->Py() * track->Px())/track->Pt();
      s11l +=  (track->Py() * track->Py())/track->Pt();


      nesdtracks++;
      acceptedtracks->Add(track);
      fHistPt->Fill(track->Pt());
      
    } //track loop 
    
    Printf("nesdtracks=%d",nesdtracks);

    if(nesdtracks<nana)return;
    Double_t meanPt=sumapt/nesdtracks;

    fn->Fill(5);

    //linearized sphericity
    Float_t sphericityL=-2;
    
    Double_t S00L=s00l/sumapt;
    Double_t S01L=s01l/sumapt;
    //Double_t S10L=s10l/sumapt;
    Double_t S11L=s11l/sumapt;
    
    Float_t lambda1L=((S00L+S11L)+TMath::Sqrt((S00L+S11L)*(S00L+S11L)-4*(S00L*S11L-S01L*S01L)))/2;
    Float_t lambda2L=((S00L+S11L)-TMath::Sqrt((S00L+S11L)*(S00L+S11L)-4*(S00L*S11L-S01L*S01L)))/2;
    if(lambda2L+lambda1L==0)
      sphericityL=0;
    if(lambda1L+lambda2L!=0) 
      sphericityL=2*TMath::Min( lambda1L,lambda2L )/( lambda1L+lambda2L );
    
    if(sphericityL<0)return;


    //ALL
    
    fn->Fill(6);
    
    fstLNchA->Fill(nesdtracks,sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

    fptNchA->Fill(nesdtracks,meanPt);

    for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
      AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
      if (!track) continue;
      fHistPtA->Fill(track->Pt());
    }  

    for(Int_t i=0;i<80;++i){
      if(nesdtracks==i+2){
	fstLesdA[i]->Fill(sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
      }else continue;     
    }

    //SOFT
    if(maxipttracks<2){
      
      fn->Fill(7);

      fstLNchS->Fill(nesdtracks,sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
      fptNchS->Fill(nesdtracks,meanPt);

      for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	if (!track) continue;
	fHistPtS->Fill(track->Pt());
      }  
      
      
      for(Int_t i=0;i<80;++i){
	if(nesdtracks==i+2){
	  fstLesdS[i]->Fill(sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	}else continue;     
      }
      
      
    }//end SOFT



    //HARD
    if(maxipttracks>=2){
      
      fn->Fill(8);

      fstLNchH->Fill(nesdtracks,sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
      fptNchH->Fill(nesdtracks,meanPt);

      for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	if (!track) continue;
	fHistPtH->Fill(track->Pt());
      }  
      
      
      for(Int_t i=0;i<80;++i){
	if(nesdtracks==i+2){
	  fstLesdH[i]->Fill(sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	}else continue;     
      }
      
      
    }//end HARD

    //JETTY
    if(sphericityL<0.2){
      
      fn->Fill(9);
      fptNchJ->Fill(nesdtracks,meanPt);
      for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	if (!track) continue;
	fHistPtJ->Fill(track->Pt());
      }  
            
    }//end JETTY



    //ISOTROPIC
    if(sphericityL>0.8){
      
      fn->Fill(10);
      fptNchI->Fill(nesdtracks,meanPt);

      for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	if (!track) continue;
	fHistPtI->Fill(track->Pt());
      }  
            
    }//end ISOTROPIC


    /*
      SELECTION ON MULTIPLICITY
    */




    if(nesdtracks>=3){
      
      Int_t multBin=0;
      if((nesdtracks<10)&&(nesdtracks>=3))multBin=1;
      if((nesdtracks<20)&&(nesdtracks>=10))multBin=2;
      if((nesdtracks<30)&&(nesdtracks>=20))multBin=3;
      if((nesdtracks<40)&&(nesdtracks>=30))multBin=4;
      if(nesdtracks>=40)multBin=5;


      fNchESD->Fill(nesdtracks);
      fstL[0]->Fill(sphericityL);
      fstL[multBin]->Fill(sphericityL);
      fnMultBin->Fill(0);
      fnMultBin->Fill(multBin);

      if(sphericityL<0.2){	
	for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	  AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	  if (!track) continue;
	  fptJeBinNch[0]->Fill(track->Pt());
	  fptJeBinNch[multBin]->Fill(track->Pt());
	}  
      }
      
      if((sphericityL<0.7)&&(sphericityL>0.5)){	
	for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	  AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	  if (!track) continue;
	  fptAvBinNch[0]->Fill(track->Pt());
	  fptAvBinNch[multBin]->Fill(track->Pt());
	}  
      }
      
      if(sphericityL>0.8){	
	for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	  AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	  if (!track) continue;
	  fptIsBinNch[0]->Fill(track->Pt());
	  fptIsBinNch[multBin]->Fill(track->Pt());
	}  
      }
      
 
    }//END Nch>=3
    
    
    delete acceptedtracks;

  }//end MC=kFALSE

  /*
    USING BOTH: REC AND GEN MC
  */

  else{

    AliMCEvent *mcEvent = (AliMCEvent*) MCEvent();
    if (!mcEvent) {
      Error("UserExec", "Could not retrieve MC event");
      return;
    }
    AliStack* stack = mcEvent->Stack();
    if (!stack)
      {
	AliDebug(AliLog::kError, "Stack not available");
	return;
      }
    
    AliHeader* header = mcEvent->Header();
    if (!header)
      {
	AliDebug(AliLog::kError, "Header not available");
	return;
      }
    Int_t nPrim  = stack->GetNprimary();

    fn->Fill(0);
    
    Int_t ntracks = fMyESDEvent->GetNumberOfTracks();
    Printf("ESD tracks: %d", ntracks);
    if(ntracks==0)return;
    
    fn->Fill(1);
    

    Bool_t isSelected = (((AliInputEventHandler*)(AliAnalysisManager::GetAnalysisManager()->GetInputEventHandler()))->IsEventSelected() & AliVEvent::kMB);
    if(isSelected==kFALSE)return;
    
    fn->Fill(2);
    
    //accept event only, if vertex is good and is within fVertexZcut region
    const AliESDVertex*	vtxESD   = fMyESDEvent->GetPrimaryVertex();
    if(vtxESD->GetNContributors()<1)return;
    Float_t fVz= vtxESD->GetZ();
    if(TMath::Abs(fVz)>10) return;
    
    fn->Fill(3);

    
    Bool_t pileup=fMyESDEvent->IsPileupFromSPD(3, 0.8);// This function checks if there was a pile up reconstructed with SPD
    if(pileup==kTRUE){Printf("pile up, skip");return;}
    
    fn->Fill(4);
    


    
    TObjArray* acceptedtracks = new TObjArray();	
    Int_t nesdtracks=0;
    Double_t maxipttracks=0;
    Double_t sumapt=0;
    
    //linearized
    Double_t s00l=0;
    Double_t s01l=0;
    Double_t s10l=0;
    Double_t s11l=0;



    // Track loop
    for (Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
      AliESDtrack *track = (AliESDtrack *)fMyESDEvent->GetTrack(iTracks);
      if (!track) {
	Error("UserExec", "Could not receive track %d", iTracks);
	continue;
      }
      if (!fCuts->AcceptTrack(track))continue;
      if (TMath::Abs(track->Eta())>etacutoffa)continue;
      if(track->Pt()<ptcutoffa)continue;
      if(track->Pt()>maxipttracks) maxipttracks=track->Pt();
      sumapt+=track->Pt();
      
      s00l +=  (track->Px() * track->Px())/track->Pt();
      s01l +=  (track->Px() * track->Py())/track->Pt();
      s10l +=  (track->Py() * track->Px())/track->Pt();
      s11l +=  (track->Py() * track->Py())/track->Pt();


      nesdtracks++;
      acceptedtracks->Add(track);
      fHistPt->Fill(track->Pt());
      
    } //track loop 
    
    if(nesdtracks<nana)return;
    Double_t meanPt=sumapt/nesdtracks;
    
    fn->Fill(5);
    
    //linearized sphericity
    Float_t sphericityL=-2;
    
    Double_t S00L=s00l/sumapt;
    Double_t S01L=s01l/sumapt;
    // Double_t S10L=s10l/sumapt;
    Double_t S11L=s11l/sumapt;
    
    Float_t lambda1L=((S00L+S11L)+TMath::Sqrt((S00L+S11L)*(S00L+S11L)-4*(S00L*S11L-S01L*S01L)))/2;
    Float_t lambda2L=((S00L+S11L)-TMath::Sqrt((S00L+S11L)*(S00L+S11L)-4*(S00L*S11L-S01L*S01L)))/2;
    if(lambda2L+lambda1L==0)
      sphericityL=0;
    if(lambda1L+lambda2L!=0) 
      sphericityL=2*TMath::Min( lambda1L,lambda2L )/( lambda1L+lambda2L );
    
    if(sphericityL<0)return;



    //MC gen
    TObjArray* acceptedtracksg = new TObjArray();	
    Int_t nmctracks=0;
   // Double_t maxiptmctracks=0;
    Double_t sumaptg=0;
        
    Double_t s00gl=0;
    Double_t s01gl=0;
    Double_t s10gl=0;
    Double_t s11gl=0;



    //kine particles loop
    for (Int_t iMCTracks = 0; iMCTracks < nPrim; ++iMCTracks) {
      TParticle* trackmc = stack->Particle(iMCTracks);
      if (!trackmc) continue;
      // Check if particle is charged, and primary
      
      Double_t etamc =trackmc ->Eta();
      Double_t ptmc=trackmc->Pt();
      Int_t pdgCode  = TMath::Abs(trackmc->GetPdgCode());
      if (TMath::Abs(etamc) > etacutoffa) continue;
      if(ptmc < ptcutoffa) continue;
      Bool_t isprimary = stack->IsPhysicalPrimary(iMCTracks); 
      if(isprimary==0) continue;
      TParticlePDG* pdgPart =trackmc ->GetPDG();
      if(neutral==kTRUE){
	if (pdgCode == 22 || pdgCode == 12 || pdgCode == 14 || pdgCode == 16)continue;	
      } else {
	if (pdgPart->Charge() == 0) continue;
      }
      nmctracks++;
      acceptedtracksg->Add(trackmc);
      sumaptg+=trackmc->Pt();

      s00gl += (trackmc->Px() * trackmc->Px())/trackmc->Pt();
      s01gl += (trackmc->Px() * trackmc->Py())/trackmc->Pt();
      s10gl += (trackmc->Py() * trackmc->Px())/trackmc->Pt();
      s11gl += (trackmc->Py() * trackmc->Py())/trackmc->Pt();


    }  
    
    if(nmctracks<1)return;
    
    //linearized sphericity
    Double_t S00gl=s00gl/sumaptg;
    Double_t S01gl=s01gl/sumaptg;
    // Double_t S10gl=s10gl/sumaptg;
    Double_t S11gl=s11gl/sumaptg;


    Float_t sphericitygL=-2;
    
    Float_t lambda1gL=((S00gl+S11gl)+TMath::Sqrt((S00gl+S11gl)*(S00gl+S11gl)-4*(S00gl*S11gl-S01gl*S01gl)))/2;
    Float_t lambda2gL=((S00gl+S11gl)-TMath::Sqrt((S00gl+S11gl)*(S00gl+S11gl)-4*(S00gl*S11gl-S01gl*S01gl)))/2;
    if((lambda2gL==0)&&(lambda1gL==0))
      sphericitygL=0;
    if(lambda1gL+lambda2gL!=0) 
      sphericitygL=2*TMath::Min( lambda1gL,lambda2gL )/( lambda1gL+lambda2gL );
    
    if(sphericitygL<0)return;

    //ALL
    
    fn->Fill(6);
    
    fstLNchA->Fill(nesdtracks,sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&

    if((nesdtracks>=3)&&(nmctracks>=3)){
      fRespMNchA->Fill(nmctracks,nesdtracks);
      fNchESD->Fill(nesdtracks);
      fNchMC ->Fill(nmctracks);
    }


    fptNchA->Fill(nesdtracks,meanPt);

    for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
      AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
      if (!track) continue;
      fHistPtA->Fill(track->Pt());
    }  
    
    
    for(Int_t i=0;i<80;++i){
      if(nesdtracks==i+2){
	fstLesdA[i]->Fill(sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	fRespMStLA[i]->Fill(sphericitygL,sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
      }else continue;     
    }
    
    //SOFT
    if(maxipttracks<2){
      
      fn->Fill(7);
      fstLNchS->Fill(nesdtracks,sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&
      fRespMNchS->Fill(nmctracks,nesdtracks);
      fptNchS->Fill(nesdtracks,meanPt);
	
      for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	if (!track) continue;
	fHistPtS->Fill(track->Pt());
      }  
      
      
      for(Int_t i=0;i<80;++i){
	if(nesdtracks==i+2){
	  fstLesdS[i]->Fill(sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&
	  fRespMStLS[i]->Fill(sphericitygL,sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&
	}else continue;     
      }
      
      
    }//end SOFT

    
    
    //HARD
    if(maxipttracks>=2){
      
      fn->Fill(8);

      fstLNchH->Fill(nesdtracks,sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
      fRespMNchH->Fill(nmctracks,nesdtracks);
      fptNchH->Fill(nesdtracks,meanPt);

      for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	if (!track) continue;
	fHistPtH->Fill(track->Pt());
      }  
      
      
      for(Int_t i=0;i<80;++i){
	if(nesdtracks==i+2){
	  fstLesdH[i]->Fill(sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
	  fRespMStLH[i]->Fill(sphericitygL,sphericityL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&6
	}else continue;     
      }
      
      
    }//end HARD
    
    //JETTY
    if(sphericityL<0.2){
      
      fn->Fill(9);
      fRespMNchJ->Fill(nmctracks,nesdtracks);
      fptNchJ->Fill(nesdtracks,meanPt);

      for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	if (!track) continue;
	fHistPtJ->Fill(track->Pt());
      }  
      
    }//end JETTY

    
    //ISOTROPIC
    if(sphericityL>0.8){
      
      fn->Fill(10);
      fRespMNchI->Fill(nmctracks,nesdtracks);
      fptNchI->Fill(nesdtracks,meanPt);

      for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	if (!track) continue;
	fHistPtI->Fill(track->Pt());
      }  
            
    }//end ISOTROPIC
    

    if(nesdtracks>=3){
      
      Int_t multBin=0;
      if((nesdtracks<10)&&(nesdtracks>=3))multBin=1;
      if((nesdtracks<20)&&(nesdtracks>=10))multBin=2;
      if((nesdtracks<30)&&(nesdtracks>=20))multBin=3;
      if((nesdtracks<40)&&(nesdtracks>=30))multBin=4;
      if(nesdtracks>=40)multBin=5;


 
      fstL[0]->Fill(sphericityL);
      fstL[multBin]->Fill(sphericityL);

      fTstL[0]->Fill(sphericitygL);
      fTstL[multBin]->Fill(sphericitygL);

      fnMultBin->Fill(0);
      fnMultBin->Fill(multBin);
      fRespMStLNch[0]->Fill(sphericitygL,sphericityL);
      fRespMStLNch[multBin]->Fill(sphericitygL,sphericityL);


      //JETTY
      if(sphericityL<0.2){
	//Loop over ESDs	
	for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	  AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	  if (!track) continue;
	  fptJeBinNch[0]->Fill(track->Pt());
	  fptJeBinNch[multBin]->Fill(track->Pt());
	}  
	
	//The true pT spectrum for this reconstructed event	
	for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
	  TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
	  if (!track) continue;
	  fTptJeBinNch[0]->Fill(track->Pt());
	  fTptJeBinNch[multBin]->Fill(track->Pt());
	}  
	
      }
      
      //AVERAGE EVENTS
      if((sphericityL<0.7)&&(sphericityL>0.5)){	
	for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	  AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	  if (!track) continue;
	  fptAvBinNch[0]->Fill(track->Pt());
	  fptAvBinNch[multBin]->Fill(track->Pt());
	}  
	//The true pT spectrum for this reconstructed event	
	for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
	  TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
	  if (!track) continue;
	  fTptAvBinNch[0]->Fill(track->Pt());
	  fTptAvBinNch[multBin]->Fill(track->Pt());
	}  
	
      }
      
      //ISOTROPIC EVENTS
      if(sphericityL>0.8){	
	for (Int_t iTracks = 0; iTracks < acceptedtracks->GetEntries(); ++iTracks) { 
	  AliESDtrack* track = dynamic_cast<AliESDtrack*> (acceptedtracks->At(iTracks));
	  if (!track) continue;
	  fptIsBinNch[0]->Fill(track->Pt());
	  fptIsBinNch[multBin]->Fill(track->Pt());
	}  
	//The true pT spectrum for this reconstructed event	
	for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
	  TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
	  if (!track) continue;
	  fTptIsBinNch[0]->Fill(track->Pt());
	  fTptIsBinNch[multBin]->Fill(track->Pt());
	}  
	
      }
      
      
    }//END Nch>=3
    
    
    delete acceptedtracks;
    delete acceptedtracksg;
    
    
    
  }//end MC=kTRUE
  
  
  
}   

//________________________________________________________________________
void AliAnaTaskLinearStpp::LoopESDMC()
{
  // Main loop
  // Called for each event
  /////////////////////////////////////////////////////////////////////////////////
  Double_t etacutoffa=0.8;
  Double_t ptcutoffa=0.5;
  Bool_t neutral=kFALSE;
  //Int_t nana=1;
  ////////////////////////////////////////////////////////////////////////////////
  
  
  AliMCEvent *mcEvent = (AliMCEvent*) MCEvent();
  if (!mcEvent) {
    Error("UserExec", "Could not retrieve MC event");
    return;
  }
  AliStack* stack = mcEvent->Stack();
  if (!stack)
    {
      AliDebug(AliLog::kError, "Stack not available");
      return;
    }
  
  AliHeader* header = mcEvent->Header();
  if (!header)
    {
      AliDebug(AliLog::kError, "Header not available");
      return;
    }
  Int_t nPrim  = stack->GetNprimary();
  
  fnMC->Fill(0);
  
  //PONER CORTE EN VERTICE
  TObjArray* acceptedtracksg = new TObjArray();	
  Int_t nmctracks=0;
  Double_t maxiptmctracks=0;
  Double_t sumaptg=0;  
    
  Double_t s00gl=0;//&&&&&&&&&&&&&&&&&&&&&
  Double_t s01gl=0;//&&&&&&&&&&&&&&&&&&&&&
  Double_t s10gl=0;//&&&&&&&&&&&&&&&&&&&&&
  Double_t s11gl=0;//&&&&&&&&&&&&&&&&&&&&&



  //kine particles loop
  for (Int_t iMCTracks = 0; iMCTracks < nPrim; ++iMCTracks) {
    TParticle* trackmc = stack->Particle(iMCTracks);
    if (!trackmc) continue;
    // Check if particle is charged, and primary
    
    Double_t etamc =trackmc ->Eta();
    Double_t ptmc=trackmc->Pt();
    Int_t pdgCode  = TMath::Abs(trackmc->GetPdgCode());
    if (TMath::Abs(etamc) > etacutoffa) continue;
    if(ptmc < ptcutoffa) continue;
    Bool_t isprimary = stack->IsPhysicalPrimary(iMCTracks); 
    if(isprimary==0) continue;
    TParticlePDG* pdgPart =trackmc ->GetPDG();
    if(neutral==kTRUE){
      if (pdgCode == 22 || pdgCode == 12 || pdgCode == 14 || pdgCode == 16)continue;	
    } else {
      if (pdgPart->Charge() == 0) continue;
    }

    acceptedtracksg->Add(trackmc);
    if(trackmc->Pt()>maxiptmctracks) maxiptmctracks=trackmc->Pt();
    nmctracks++;
    sumaptg+=trackmc->Pt();
    fHistPtMC->Fill(trackmc->Pt());

    s00gl += (trackmc->Px() * trackmc->Px())/trackmc->Pt();//&&&&&&&&&&&&&&&&&&&&&
    s01gl += (trackmc->Px() * trackmc->Py())/trackmc->Pt();//&&&&&&&&&&&&&&&&&&&&&
    s10gl += (trackmc->Py() * trackmc->Px())/trackmc->Pt();//&&&&&&&&&&&&&&&&&&&&&
    s11gl += (trackmc->Py() * trackmc->Py())/trackmc->Pt();//&&&&&&&&&&&&&&&&&&&&&

  }  
    
  if(nmctracks<1)return;
  Double_t meanPtg=sumaptg/nmctracks;  

    
  //linearized
  Double_t S00gl=s00gl/sumaptg;//&&&&&&&&&&&&&&&&&&&&&
  Double_t S01gl=s01gl/sumaptg;//&&&&&&&&&&&&&&&&&&&&&
  //Double_t S10gl=s10gl/sumaptg;//&&&&&&&&&&&&&&&&&&&&&
  Double_t S11gl=s11gl/sumaptg;//&&&&&&&&&&&&&&&&&&&&&

  Float_t sphericitygL=-2;//&&&&&&&&&&&&&&&&&&&&&
  
  Float_t lambda1gl=((S00gl+S11gl)+TMath::Sqrt((S00gl+S11gl)*(S00gl+S11gl)-4*(S00gl*S11gl-S01gl*S01gl)))/2;//&&&&&&&&&&&&&&&&&&&&&
  Float_t lambda2gl=((S00gl+S11gl)-TMath::Sqrt((S00gl+S11gl)*(S00gl+S11gl)-4*(S00gl*S11gl-S01gl*S01gl)))/2;//&&&&&&&&&&&&&&&&&&&&&
  if((lambda2gl==0)&&(lambda1gl==0))//&&&&&&&&&&&&&&&&&&&&&
    sphericitygL=0;//&&&&&&&&&&&&&&&&&&&&&
  if(lambda1gl+lambda2gl!=0) //&&&&&&&&&&&&&&&&&&&&&//&&&&&&&&&&&&&&&&&&&&&
    sphericitygL=2*TMath::Min( lambda1gl,lambda2gl )/( lambda1gl+lambda2gl );//&&&&&&&&&&&&&&&&&&&&&
  
  if(sphericitygL<0)return;//&&&&&&&&&&&&&&&&&&&&&




    
  //ALL
    
  fnMC->Fill(6);
    
  fstLNchAMC->Fill(nmctracks,sphericitygL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
  fptNchAMC->Fill(nmctracks,meanPtg);

  for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
    TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
    if (!track) continue;
    fHistPtAMC->Fill(track->Pt());
  }  
  
  
  //SOFT
  if(maxiptmctracks<2){
    
    fnMC->Fill(7);
    
    fstLNchSMC->Fill(nmctracks,sphericitygL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&6
    fptNchSMC->Fill(nmctracks,meanPtg);

    for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
      TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
      if (!track) continue;
      fHistPtSMC->Fill(track->Pt());
    }  
    
  }//end SOFT
  
  
  
  //HARD
  if(maxiptmctracks>=2){
    
    fnMC->Fill(8);
    
    fstLNchHMC->Fill(nmctracks,sphericitygL);//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&6
    fptNchHMC->Fill(nmctracks,meanPtg);

    for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
      TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
      if (!track) continue;
      fHistPtHMC->Fill(track->Pt());
    }  
    
  }//end HARD
  
  //JETTY
  if(sphericitygL<0.2){
    
    fnMC->Fill(9);
    fptNchJMC->Fill(nmctracks,meanPtg);

    for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
      TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
      if (!track) continue;
      fHistPtJMC->Fill(track->Pt());
    }  
    
  }//end JETTY
  
  
  //ISOTROPIC
  if(sphericitygL>0.8){
    
    fnMC->Fill(10);
    fptNchIMC->Fill(nmctracks,meanPtg);

    for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
      TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
      if (!track) continue;
      fHistPtIMC->Fill(track->Pt());
    }  
    
  }//end ISOTROPIC
  


    if(nmctracks>=3){
      
      Int_t multBin=0;
      if((nmctracks<10)&&(nmctracks>=3))multBin=1;
      if((nmctracks<20)&&(nmctracks>=10))multBin=2;
      if((nmctracks<30)&&(nmctracks>=20))multBin=3;
      if((nmctracks<40)&&(nmctracks>=30))multBin=4;
      if(nmctracks>=40)multBin=5;

      fstLMC[0]->Fill(sphericitygL);
      fstLMC[multBin]->Fill(sphericitygL);
      fnMultBing->Fill(0);
      fnMultBing->Fill(multBin);

      if(sphericitygL<0.2){	
	for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
	  TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
	  if (!track) continue;
	  fptJeBinNchMC[0]->Fill(track->Pt());
	  fptJeBinNchMC[multBin]->Fill(track->Pt());
	}  
      }
      
      if((sphericitygL<0.7)&&(sphericitygL>0.5)){	
	for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
	  TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
	  if (!track) continue;
	  fptAvBinNchMC[0]->Fill(track->Pt());
	  fptAvBinNchMC[multBin]->Fill(track->Pt());
	}  
      }
      
      if(sphericitygL>0.8){	
	for (Int_t iTracks = 0; iTracks < acceptedtracksg->GetEntries(); ++iTracks) { 
	  TParticle* track = dynamic_cast<TParticle*> (acceptedtracksg->At(iTracks));
	  if (!track) continue;
	  fptIsBinNchMC[0]->Fill(track->Pt());
	  fptIsBinNchMC[multBin]->Fill(track->Pt());
	}  
      }
      
 
    }//END Nch>=3


 
  delete acceptedtracksg;

 


}

