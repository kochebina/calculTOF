#include "run.hh"
#include "construction.hh"
#include "G4RunManager.hh"


MyRunAction::MyRunAction()
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	man->SetNtupleMerging(true);

	man->CreateNtuple("Source","Source");
	man->CreateNtupleDColumn("SposX");
	man->CreateNtupleDColumn("SposY");
	man->CreateNtupleDColumn("SposZ");
	man->FinishNtuple(0); //numéro du Ntuple

	man->CreateNtuple("Steps","Steps");
	man->CreateNtupleIColumn("EventID"); //I pour integer
	man->CreateNtupleDColumn("X");
	man->CreateNtupleDColumn("Y");
	man->CreateNtupleDColumn("Z");
	man->CreateNtupleDColumn("GlobalTime");
	man->CreateNtupleSColumn("PreProcessName");
	man->CreateNtupleSColumn("PostProcessName");
	man->CreateNtupleIColumn("trackID");
	man->CreateNtupleDColumn("totalEnergy");
	man->CreateNtupleIColumn("stepNumber");
	man->CreateNtupleIColumn("parentID");
	man->CreateNtupleSColumn("volName");
	man->CreateNtupleIColumn("crystalID");
	man->CreateNtupleDColumn("edep");
	man->FinishNtuple(1);

/*	man->CreateNtuple("Hits", "Hits"); //noms vérifiés comme Gate
	man->CreateNtupleIColumn("eventID"); //I pour integer
	man->CreateNtupleDColumn("posX");
	man->CreateNtupleDColumn("posY");
	man->CreateNtupleDColumn("posZ");
	//man->CreateNtupleDColumn("time");
	man->CreateNtupleDColumn("time");
	man->CreateNtupleDColumn("localTime");
	man->CreateNtupleDColumn("properTime");
	man->CreateNtupleDColumn("T0");
	man->CreateNtupleSColumn("PreProcessName");//pas dans Gate
	man->CreateNtupleSColumn("processName");
	man->CreateNtupleIColumn("trackID");
	man->CreateNtupleDColumn("totalEnergy"); //pas dans Gate
	man->CreateNtupleDColumn("kineticEnergy"); //pas dans Gate
	man->CreateNtupleIColumn("stepNumber"); //pas dans Gate
	man->CreateNtupleIColumn("hitNumber"); //pas dans Gate
	man->CreateNtupleIColumn("hitNumberTrack1"); //pas dans Gate
	man->CreateNtupleIColumn("hitNumberTrack2"); //pas dans Gate
	man->CreateNtupleIColumn("parentID");
	man->CreateNtupleSColumn("volName"); //pas dans Gate
	man->CreateNtupleSColumn("particleName");//pas dans Gate
	man->CreateNtupleIColumn("layerID");
	man->CreateNtupleIColumn("crystalID");
	man->CreateNtupleIColumn("submoduleID");
	man->CreateNtupleIColumn("moduleID");
	man->CreateNtupleIColumn("rsectorID");
	man->CreateNtupleDColumn("edep");
	man->FinishNtuple(2);
	*/

	//NB : ce NTuple est lié à la classe "Hits"...
	man->CreateNtuple("Hits", "Hits"); //noms vérifiés comme Gate
	man->CreateNtupleIColumn("hitNumber"); //pas dans Gate
	man->CreateNtupleIColumn("hitNumberTrack1"); //pas dans Gate
	man->CreateNtupleIColumn("hitNumberTrack2"); //pas dans Gate
	man->CreateNtupleIColumn("trackID");
	man->CreateNtupleIColumn("eventID"); //I pour integer
	man->CreateNtupleIColumn("parentID");
	man->CreateNtupleDColumn("posX");
	man->CreateNtupleDColumn("posY");
	man->CreateNtupleDColumn("posZ");
	man->CreateNtupleDColumn("time");
	man->CreateNtupleDColumn("edep");
	man->CreateNtupleIColumn("layerID");
	man->CreateNtupleIColumn("crystalID");
	man->CreateNtupleIColumn("submoduleID");
	man->CreateNtupleIColumn("moduleID");
	man->CreateNtupleIColumn("rsectorID");
	man->CreateNtupleSColumn("particleName");//pas dans Gate
	man->CreateNtupleDColumn("kineticEnergy"); //pas dans Gate
	man->CreateNtupleSColumn("processName");
	man->FinishNtuple(2);

	man->CreateNtuple("Singles", "Singles"); //noms vérifiés comme Gate
	man->CreateNtupleIColumn("eventID"); //I pour integer
	man->CreateNtupleDColumn("globalPosX");
	man->CreateNtupleDColumn("globalPosY");
	man->CreateNtupleDColumn("globalPosZ");
	man->CreateNtupleDColumn("time");
	man->CreateNtupleIColumn("layerID");
	man->CreateNtupleIColumn("crystalID");
	man->CreateNtupleIColumn("submoduleID");
	man->CreateNtupleIColumn("moduleID");
	man->CreateNtupleIColumn("rsectorID");
	man->CreateNtupleDColumn("edep");
	man->FinishNtuple(3);

	man->CreateNtuple("Coincidences", "Coincidences"); //noms vérifiés comme Gate
	man->CreateNtupleIColumn("eventID"); //I pour integer

	man->CreateNtupleDColumn("posX1");
	man->CreateNtupleDColumn("posY1");
	man->CreateNtupleDColumn("posZ1");

	man->CreateNtupleDColumn("posX2");
	man->CreateNtupleDColumn("posY2");
	man->CreateNtupleDColumn("posZ2");

	man->CreateNtupleDColumn("time1");
	man->CreateNtupleDColumn("time2");

	man->CreateNtupleIColumn("layerID1");
	man->CreateNtupleIColumn("crystalID1");
	man->CreateNtupleIColumn("submoduleID1");
	man->CreateNtupleIColumn("moduleID1");
	man->CreateNtupleIColumn("rsectorID1");
	man->CreateNtupleDColumn("edep1");

	man->CreateNtupleIColumn("layerID2");
	man->CreateNtupleIColumn("crystalID2");
	man->CreateNtupleIColumn("submoduleID2");
	man->CreateNtupleIColumn("moduleID2");
	man->CreateNtupleIColumn("rsectorID2");
	man->CreateNtupleDColumn("edep2");

	man->CreateNtupleDColumn("dt");
	man->CreateNtupleDColumn("d");

	man->FinishNtuple(4);





	//delete man;
	G4cout<< "MyRunAction" << G4endl;
}

MyRunAction::~MyRunAction()
{
G4cout<< "~MyRunAction" << G4endl;
}

void MyRunAction::BeginOfRunAction(const G4Run* run)
{
	//testé
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	G4int runID = run->GetRunID();

	std::stringstream strRunID;
	strRunID << runID;

	const MyDetectorConstruction * detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

	G4int e = detectorConstruction->GetCristalWidth();

	std::stringstream strCristalWidth;
	strCristalWidth << e;
	//man->OpenFile("../root/output" + strRunID.str() +"_e_"+ strCristalWidth.str() + ".root");

	man->OpenFile("../../../calculTOF_outputs/G4Outputs/test" + strRunID.str() + ".root");

	G4cout<< "BeginOfRunAction" << G4endl;

}
void MyRunAction::EndOfRunAction(const G4Run*)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	man->Write();
	man->CloseFile();

	G4cout<< "EndOfRunAction" << G4endl;
}
