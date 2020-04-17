void runGrid()
{
// Load common libraries
   gSystem->AddIncludePath("-I${ALICE_ROOT}/include/ -I${ALICE_ROOT}/PWG0/ -I${ALICE_ROOT}/PWG0/");
    gSystem->Load("libVMC");
    gSystem->Load("libTree");
    gSystem->Load("libProof");
    gSystem->Load("libSTEERBase");
    gSystem->Load("libESD");
    gSystem->Load("libAOD");
    gSystem->Load("libANALYSIS");
    gSystem->Load("libANALYSISalice");
    


// Use AliRoot includes to compile our task
 

// Create and configure the alien handler plugin
  gROOT->LoadMacro("CreateAlienHandler.C");
  AliAnalysisGrid *alienHandler = CreateAlienHandler();  
  if (!alienHandler) return; 


// Create the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("testAnalysis");

// Connect plugin to the analysis manager
  mgr->SetGridHandler(alienHandler);



  Bool_t useMC=kTRUE;
  Bool_t ismcsimulation=kTRUE;

  AliESDInputHandler* esdH = new AliESDInputHandler();
  esdH->SetInactiveBranches("Calo FMD");
  esdH -> SetReadFriends(0);
  mgr->SetInputEventHandler(esdH);
  if(useMC==kTRUE){
    AliMCEventHandler *mc = new AliMCEventHandler();
    mgr->SetMCtruthEventHandler(mc); 
  }
 


  cInput = mgr->GetCommonInputContainer();
  


  gROOT->LoadMacro("$ALICE_ROOT/ANALYSIS/macros/AddTaskPhysicsSelection.C");
  AliPhysicsSelectionTask* physSelTask = AddTaskPhysicsSelection(ismcsimulation);
  

  AliESDtrackCuts* esdTrackCuts = AliESDtrackCuts::GetStandardITSTPCTrackCuts2010(kTRUE,1);
  esdTrackCuts->SetMinNCrossedRowsTPC(120);
  esdTrackCuts->SetMinRatioCrossedRowsOverFindableClustersTPC(0.8);
  esdTrackCuts->SetMaxChi2PerClusterITS(36);
  esdTrackCuts->SetMaxFractionSharedTPCClusters(0.4);
  esdTrackCuts->SetMaxChi2TPCConstrainedGlobal(36);
 


  gROOT->LoadMacro("AliAnaTaskLinearStpp.cxx++g"); 
  task = new AliAnaTaskLinearStpp("taskesapp");
  task->SetCuts(esdTrackCuts);
  // uncomment this to use MC
  task->UseMC(useMC);

  

  cInput = mgr->GetCommonInputContainer();

  mgr->ConnectInput(task, 0, cInput);
  
  // Attach output
  cOutput = mgr->CreateContainer("cOutput", TList::Class(), AliAnalysisManager::kOutputContainer,"StLQLHC10e20_MCPythia.root");
  mgr->ConnectOutput(task, 1, cOutput);



// No need to create a chain - this is handled by the plugin
//  gROOT->LoadMacro("$ALICE_ROOT/PWG0/CreateESDChain.C");
//  TChain* chain = CreateESDChain("ESD82XX_30K.txt", 10);



// Enable debug printouts
  mgr->SetDebugLevel(2);

  if (!mgr->InitAnalysis())
    return;

  mgr->PrintStatus();
// Start analysis in grid.
  mgr->StartAnalysis("grid");
}; 
