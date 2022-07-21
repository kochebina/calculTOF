#include "event.hh"
#include  "myHit.hh"

#include "G4SDManager.hh"

#include "G4Event.hh"
//#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4VHitsCollection.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
//#include "g4analysis.hh"
MyEventAction::MyEventAction(MyRunAction*)
{
	fEdep = 0.;
}

MyEventAction::~MyEventAction()
{}

void MyEventAction::BeginOfEventAction(const G4Event* event)
{
	//G4cout<< "MyEventAction::BeginOfEventAction" << G4endl;
	fEdep = 0.;
}

void MyEventAction::EndOfEventAction(const G4Event* event)
{
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	man->FillNtupleDColumn(0,0,fEdep);

	man->AddNtupleRow(0);

		//G4cout<< "MyEventAction::EndOfEventAction" << G4endl;
}
