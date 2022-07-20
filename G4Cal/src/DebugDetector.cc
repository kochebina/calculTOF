#include "DebugDetector.hh"
#include "MetaDetector.hh"
DebugDetector::DebugDetector()
{	G4cout<< "DebugDetector" << G4endl;}
DebugDetector::~DebugDetector()
{	G4cout<< "~DebugDetector" << G4endl;
}

void DebugDetector::Construct(G4LogicalVolume*logicWorld,G4Material* LSO)
{
	G4double x = 3.0*0.5*cm;
	G4double y = 1.0*0.5*cm;
	G4double z = 1.0*0.5*cm;

	G4Box * sHitBox = new G4Box("hitBox", x,y,z);
	G4LogicalVolume *lHitBox = new G4LogicalVolume(sHitBox,LSO,"lHitBox");
	G4VPhysicalVolume *pHitBox = new G4PVPlacement(0, G4ThreeVector(20.0 * cm,0.,0.), lHitBox,"pHitBox",logicWorld,false,0,true);
	this->SetScoringVolume(lHitBox);
	G4cout<< "DebugDetector::Construct" << G4endl;

}
