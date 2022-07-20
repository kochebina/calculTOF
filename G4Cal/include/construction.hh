#ifndef CONSTRUCTION_HH
#define CONSTRUCTION_HH

#include "G4SystemOfUnits.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4GenericMessenger.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "detector.hh"


class MyDetectorConstruction : public G4VUserDetectorConstruction
{
public :
	MyDetectorConstruction();
	~MyDetectorConstruction();

	//G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;}
	G4LogicalVolume *GetScoringVolume() const {return fScoringVolume;} //utilisé pour les cuts. A adapter en fonction de la géométrie
	void SetScoringVolume ( G4LogicalVolume * lV)  {fScoringVolume = lV;} 
	//G4LogicalVolume *GetReadoutVolume() const {return lOptical;}
	G4int GetCristalWidth() const {return e;}

	virtual G4VPhysicalVolume *Construct();

private :
	G4LogicalVolume *logicDetector;
	virtual void ConstructSDandField();

	G4int nCols,nRows;

	G4Box *solidWorld, *solidRadiator, *solidScintillator;
	G4Box * sHitBox;
	G4LogicalVolume *logicWorld, *logicScintillator;
	G4LogicalVolume* lPET, * lMod, * lUnit, * lBlock, * lOptical, * lCrystal;
	G4LogicalVolume* lHitBox;
	G4VPhysicalVolume *physWorld, *physScintillator;
	G4VPhysicalVolume* pPET, * pMod, * pUnit, * pBlock,* pOptical, * pCrystal;
	G4VPhysicalVolume* pHitBox;

  G4Box* solidTarget;
	G4LogicalVolume* logicTarget;
	G4VPhysicalVolume* physicTarget;

	G4GenericMessenger *fMessenger;

	G4LogicalVolume *fScoringVolume;

	G4Material *SiO2, *H2O, *Aerogel, *worldMat, *LYSO_GE,*LSO;
	G4Element *C;

	G4double xWorld, yWorld, zWorld,e,ly;

	G4OpticalSurface *mirrorSurface;

	 void DefineMaterials();
	 void ConstructScintillator();
	 void ConstructScanGE_OnlyCrystals();
	 void ConstructScanGE();
	 void ConstructTOFTestStructure();
	 void ConstructDebugGeom();

};

#endif
