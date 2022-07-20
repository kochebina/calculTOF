#include "physics.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4SystemOfUnits.hh"
#include "construction.hh"
#include "G4Region.hh"
#include "G4ProductionCuts.hh"
#include "G4EmParameters.hh"
#include "G4RunManager.hh"

MyPhysicsList::MyPhysicsList()
{
	RegisterPhysics (new G4EmStandardPhysics_option3()); //le cut par défaut est de 1.0mm
	//RegisterPhysics (new G4OpticalPhysics());
	//RegisterPhysics (new G4DecayPhysics());
	//RegisterPhysics ( new G4RadioactiveDecayPhysics());

	
	
	G4cout<< "MyPhysicsList" << G4endl;
	
}

MyPhysicsList::~MyPhysicsList()
{
	G4cout<< "~MyPhysicsList" << G4endl;
}


void  MyPhysicsList::SetCuts()
{
	// Definition of  threshold of production of secondary particles
	
        G4Region *region = new G4Region("crystal");
        G4ProductionCuts *  cuts(new G4ProductionCuts );
        G4double  defaultProductionCut( 1.0 * mm );
        
        const G4VUserPhysicsList *  physicsList(G4RunManager::GetRunManager()->GetUserPhysicsList());
        if ( physicsList ) {defaultProductionCut =physicsList->GetDefaultCutValue();}
        
        cuts->SetProductionCut(defaultProductionCut);
        
	SetCutValue(defaultProductionCut, "gamma");
	SetCutValue(defaultProductionCut, "e-");                          

        region->SetProductionCuts( cuts );
        
        const MyDetectorConstruction * detectorConstruction = static_cast<const MyDetectorConstruction*> (G4RunManager::GetRunManager()->GetUserDetectorConstruction());
	G4LogicalVolume *fScoringVolume = detectorConstruction->GetScoringVolume();
     
        region->AddRootLogicalVolume( fScoringVolume );
	
	
		G4EmParameters *emPar;
	
	emPar= G4EmParameters::Instance();
	emPar->SetApplyCuts(true);
	
	 DumpCutValuesTable(); 
	 
	 G4cout<< "MyPhysicsList::SetCuts" << G4endl;
}
