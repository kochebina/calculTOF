#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
#include "G4VHitsCollection.hh"

#include "myHit.hh"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class MySensitiveDetector : public G4VSensitiveDetector
{
public :
	MySensitiveDetector(G4String);
	virtual ~MySensitiveDetector();

	virtual void Initialize(G4HCofThisEvent* hce);
	virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
	virtual void EndOfEvent(G4HCofThisEvent* hce);


private :

	void ProcessCoincidences(G4String policy, G4VHitsCollection* hc);
	void ProcessCoincidencesTest(G4String policy, G4VHitsCollection* hc);
	G4double distance(G4double x1,G4double x2,G4double y1,G4double y2,G4double z1,G4double z2);
	// HitsCollection;
	myHitsCollection* HitsCollection;
	G4int hitNumber = 0;
	G4int hitNumberTrack1 = 0;
	G4int hitNumberTrack2 = 0;
	G4int ID;


};

#endif
