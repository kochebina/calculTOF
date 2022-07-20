#ifndef META_DETECTOR_HH
#define META_DETECTOR_HH

#include "construction.hh"
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

class MetaDetector //classe de base pour éviter de modifier directement construction.cc à chaque fois.
//On peut mimer les messengers avec des Get/set d'attributs et en créeant une classe qui hérite de celle-ci.
{
public :
	MetaDetector(){};
	virtual ~MetaDetector(){};

	virtual G4LogicalVolume *GetScoringVolume() const {return MD_scoringVolume;} //utilisé pour les cuts. A adapter en fonction de la géométrie
  virtual void SetScoringVolume( G4LogicalVolume * scoringVolume)  {MD_scoringVolume = scoringVolume;}
	virtual G4LogicalVolume* GetScoringVolume()  {return MD_scoringVolume; }
	virtual void Construct(G4LogicalVolume*logicWorld){};

private :
G4LogicalVolume *MD_scoringVolume;

};

#endif
