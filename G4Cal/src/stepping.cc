#include "stepping.hh"

MySteppingAction::MySteppingAction(MyEventAction *eventAction)
{
	fEventAction = eventAction;
}

MySteppingAction::~MySteppingAction()
{}

void MySteppingAction::UserSteppingAction(const G4Step *step)
{
	/*
	// a mettre dans une classe "hits" ?
	G4double edep = step->GetTotalEnergyDeposit();
	fEventAction->addEdep(edep);
	
	//event
	G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
	//Track
	G4Track *track = step->GetTrack();
	G4int trackID = track->GetTrackID(); //track déjà défini plus haut
	G4int stepNumber = track->GetCurrentStepNumber();
	G4int parentID = track->GetParentID();
	//step
	
	//stepPoint
	G4StepPoint *preStepPoint = step->GetPreStepPoint(); //arrivée dans le détecteur du photon
	G4StepPoint *postStepPoint = step->GetPostStepPoint(); //sortie du détecteur
	
	G4double time = preStepPoint->GetGlobalTime(); //vérifié comme dans Gate
	G4double totalEnergy = preStepPoint->GetTotalEnergy();
	G4ThreeVector posPhoton = preStepPoint->GetPosition();
	
	const G4VTouchable *touchable = preStepPoint->GetTouchable();
	G4VPhysicalVolume *physVol = touchable->GetVolume();
	G4String volName = physVol -> GetName();
	G4int crystalID = touchable->GetCopyNumber();
	
	G4int opticalID = touchable->GetCopyNumber(1);
	G4int blockID = touchable->GetCopyNumber(2);
	G4int unitID = touchable->GetCopyNumber(3);
	G4int ModuleID = touchable->GetCopyNumber(4);
	
	
	//nom du process
	 const G4VProcess* PreProcess = preStepPoint->GetProcessDefinedStep(); //Process which defined the current Step.
	G4String prepname = "";
	if (PreProcess) prepname = PreProcess->GetProcessName();
	const G4VProcess* PostProcess = postStepPoint->GetProcessDefinedStep(); //Process which defined the current Step.
	G4String postpname = "";
	if (PostProcess) postpname = PostProcess->GetProcessName();
	
	
	
	
	int i =0;
	G4AnalysisManager *man = G4AnalysisManager::Instance();
	
 	man->FillNtupleIColumn(1, i,evt);   //numéro de l'event
 	i++;
 	
	man->FillNtupleDColumn(1, i, posPhoton[0]/mm); //interaction
	i++;

	man->FillNtupleDColumn(1, i, posPhoton[1]/mm);
	i++;

	man->FillNtupleDColumn(1, i, posPhoton[2]/mm);
	i++;
	
	man->FillNtupleDColumn(1, i, time/ps); // Time of creation of the event in which the track belongs
        i++;  
	
	man->FillNtupleSColumn(1, i, prepname); //S pour string
	i++;
	
	man->FillNtupleSColumn(1, i, postpname); //S pour string
	i++;
	
	man->FillNtupleIColumn(1, i, trackID); //i pour int I,F,S et D sont les seuls types possibles
	i++;

	man->FillNtupleDColumn(1, i, totalEnergy);  
	i++;
	   
	man->FillNtupleIColumn(1, i, stepNumber);  
	i++;
	  
	man->FillNtupleIColumn(1, i, parentID);  
	i++; 
	  
	man->FillNtupleSColumn(1, i, volName);  
	i++;

	man->FillNtupleIColumn(1, i, crystalID);
	i++;

	man->FillNtupleDColumn(1, i, edep);
	i++;
	man->AddNtupleRow(1);

	*/
	
	
}
