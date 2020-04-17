AliAnalysisGrid* CreateAlienHandler() {
// Check if user has a valid token, otherwise make one. This has limitations.
// One can always follow the standard procedure of calling alien-token-init then
   //source /tmp/gclient_env_$UID in the current shell.
   //if (!AliAnalysisGrid::CreateToken()) return NULL;
   AliAnalysisAlien *plugin = new AliAnalysisAlien();
   //plugin->SetOverwriteMode();
// Set the run mode (can be "full", "test", "offline", "submit" or "terminate")
   plugin->SetRunMode("test");
// Set versions of used packages
   plugin->SetAPIVersion("V1.1x");
   //plugin->SetROOTVersion("v5-26-00b-6");
   //plugin->SetAliROOTVersion("v4-19-21-AN");
   plugin->SetROOTVersion("v5-34-05");
   plugin->SetAliROOTVersion("v5-04-72-AN");

// Declare input data to be processed.
// Method 1: Create automatically XML collections using alien 'find' command.
// Define production directory LFN
   //plugin->SetGridDataDir("/alice/sim/LHC10b3");
   // On real reconstructed data:
   //plugin->SetGridDataDir("/alice/data/2010/LHC10e");
   plugin->SetGridDataDir("/alice/sim/LHC10e20");
// Set data search pattern
   //plugin->SetDataPattern("*/100/*ESDs.root");
   plugin->SetDataPattern("*AliESDs.root");
// Data pattern for reconstructed data
   //plugin->SetDataPattern("*ESDs/pass2/*ESDs.root");
   //plugin->SetDataPattern("*ESDs/pass2/10000130356045.200/*ESDs.root");

   //plugin->SetRunPrefix("000");   // real data
// ...then add run numbers to be considered
  


//part1
   plugin->AddRunNumber(130356);
   
   //plugin->AddRunNumber(130847);
   /*plugin->AddRunNumber(130844);
   plugin->AddRunNumber(130842);
   plugin->AddRunNumber(130840);
   plugin->AddRunNumber(130834);
   plugin->AddRunNumber(130799);
   plugin->AddRunNumber(130798);
   plugin->AddRunNumber(130795);
   plugin->AddRunNumber(130793);
   plugin->AddRunNumber(130704);
   plugin->AddRunNumber(130696);
   plugin->AddRunNumber(130628);
   plugin->AddRunNumber(130623);
   plugin->AddRunNumber(130621);
   */
   /*
   //part2
   plugin->AddRunNumber(130620);
   plugin->AddRunNumber(130609);
   plugin->AddRunNumber(130608);
   plugin->AddRunNumber(130601);
   plugin->AddRunNumber(130526);
   plugin->AddRunNumber(130524);
   plugin->AddRunNumber(130520);
   plugin->AddRunNumber(130519);
   plugin->AddRunNumber(130517);
   plugin->AddRunNumber(130481);
   plugin->AddRunNumber(130480);
   plugin->AddRunNumber(130479);
   plugin->AddRunNumber(130375);
   plugin->AddRunNumber(130360);
   plugin->AddRunNumber(130358);
   //part3
   plugin->AddRunNumber(130356);
   plugin->AddRunNumber(130354);
   plugin->AddRunNumber(130343);
   plugin->AddRunNumber(130342);
   plugin->AddRunNumber(130179);
   plugin->AddRunNumber(130178);
   plugin->AddRunNumber(130172);
   plugin->AddRunNumber(130158);
   plugin->AddRunNumber(130157);
   plugin->AddRunNumber(130151);
   plugin->AddRunNumber(130149);
   plugin->AddRunNumber(129983);
   plugin->AddRunNumber(129966);
   plugin->AddRunNumber(129962);
   plugin->AddRunNumber(129961);
   //part4
   plugin->AddRunNumber(129960);
   plugin->AddRunNumber(129959);
   plugin->AddRunNumber(129744);
   plugin->AddRunNumber(129742);
   plugin->AddRunNumber(129738);
   plugin->AddRunNumber(129736);
   plugin->AddRunNumber(129735);
   plugin->AddRunNumber(129729);
   plugin->AddRunNumber(129726);
   plugin->AddRunNumber(129725);
   plugin->AddRunNumber(129723);
   plugin->AddRunNumber(129667);
   plugin->AddRunNumber(129666);
   plugin->AddRunNumber(129659);
   plugin->AddRunNumber(129654);
   //part5
   plugin->AddRunNumber(129641);
   plugin->AddRunNumber(129639);
   plugin->AddRunNumber(129599);
   plugin->AddRunNumber(129536);
   plugin->AddRunNumber(128913);
   plugin->AddRunNumber(128778);
   plugin->AddRunNumber(128263);
   plugin->AddRunNumber(127940);
   plugin->AddRunNumber(127930);
   plugin->AddRunNumber(127819);
   plugin->AddRunNumber(127814);
   plugin->AddRunNumber(127730);
   plugin->AddRunNumber(127729);
   plugin->AddRunNumber(127724);

   plugin->AddRunNumber(127719);
   plugin->AddRunNumber(127718);
   plugin->AddRunNumber(127714);
   plugin->AddRunNumber(127712);
   */





//   plugin->AddRunNumber(104065);  // real data
//   plugin->SetOutputSingleFolder("output");
//   plugin->SetOutputToRunNo();
// Method 2: Declare existing data files (raw collections, xml collections, root file)
// If no path mentioned data is supposed to be in the work directory (see SetGridWorkingDir())
// XML collections added via this method can be combined with the first method if
// the content is compatible (using or not tags)
//   plugin->AddDataFile("tag.xml");
//   plugin->AddDataFile("/alice/data/2008/LHC08c/000057657/raw/Run57657.Merged.RAW.tag.root");
// Define alien work directory where all files will be copied. Relative to alien $HOME.
   //plugin->SetGridWorkingDir("LHC10epass2MBLStpart1");
   plugin->SetGridWorkingDir("LHC10e20_MCpythia"); 
// Declare alien output directory. Relative to working directory.
   plugin->SetGridOutputDir("output"); // In this case will be $HOME/work/output
// Declare the analysis source files names separated by blancs. To be compiled runtime
// using ACLiC on the worker nodes.
   plugin->SetAnalysisSource("AliAnaTaskLinearStpp.cxx");
   //plugin->SetAnalysisSource("AliAnalysisTaskPt.cxx");
// Declare all libraries (other than the default ones for the framework. These will be
// loaded by the generated analysis macro. Add all extra files (task .cxx/.h) here.
   plugin->SetAdditionalLibs("AliAnaTaskLinearStpp.h AliAnaTaskLinearStpp.cxx");
   //plugin->SetAdditionalLibs("AliAnalysisTaskPt.h AliAnalysisTaskPt.cxx");
// Declare the output file names separated by blancs.
// (can be like: file.root or file.root@ALICE::Niham::File)
//   plugin->SetOutputFiles("Pt.ESD.1.root");
   plugin->SetDefaultOutputs();
//   plugin->SetDefaultOutputs(kFALSE);
   //plugin->SetOutputFiles("StLQLHC10eRAW.root");
// Optionally define the files to be archived.
//   plugin->SetOutputArchive("log_archive.zip:stdout,stderr@ALICE::NIHAM::File root_archive.zip:*.root@ALICE::NIHAM::File");
//   plugin->SetOutputArchive("log_archive.zip:stdout,stderr");
// Optionally set a name for the generated analysis macro (default MyAnalysis.C)
   plugin->SetAnalysisMacro("TaskESApp.C");
// Optionally set maximum number of input files/subjob (default 100, put 0 to ignore)
   plugin->SetSplitMaxInputFileNumber(1000);
// Optionally modify the executable name (default analysis.sh)
   //plugin->SetExecutable("TaskESApp.sh");
// Optionally set number of failed jobs that will trigger killing waiting sub-jobs.
//   plugin->SetMaxInitFailed(5);
// Optionally resubmit threshold.
   plugin->SetMasterResubmitThreshold(90); //
// Optionally set time to live (default 30000 sec)
   plugin->SetTTL(30000);
// Optionally set input format (default xml-single)
   plugin->SetInputFormat("xml-single");
// Optionally modify the name of the generated JDL (default analysis.jdl)
   plugin->SetJDLName("TaskESApp.jdl");
// Optionally modify job price (default 1)
   plugin->SetPrice(1);      
// Optionally modify split mode (default 'se')    
   plugin->SetSplitMode("se");
   return plugin;


}
