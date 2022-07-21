#include "TOFDebugDetector.hh"
#include "MetaDetector.hh"
TOFDebugDetector::TOFDebugDetector()
{	G4cout<< "TOFDebugDetector" << G4endl;}
TOFDebugDetector::~TOFDebugDetector()
{	G4cout<< "~TOFDebugDetector" << G4endl;
}

void TOFDebugDetector::Construct(G4LogicalVolume*logicWorld,G4Material* LSO)
{

  G4double xTarget = 3.0*0.5*cm; 
	G4double yTarget = 1.0*0.5*cm;
	G4double zTarget = 1.0*0.5*cm;
  G4Box *solidTarget = new G4Box("solidTarget", xTarget, yTarget, zTarget);// x/2,y/2,z/2, en mètres

	G4LogicalVolume *logicTarget = new G4LogicalVolume(solidTarget, LSO,"logicTarget");
  G4double rMin = 100 * mm;
	G4VPhysicalVolume *physicTarget = new G4PVPlacement(0, G4ThreeVector(rMin,0.,0.), logicTarget, "physTarget", logicWorld, false, 0, true);
	physicTarget = new G4PVPlacement(0, G4ThreeVector(-rMin,0.,0.), logicTarget, "physTarget", logicWorld, false, 1, true);
  this->SetScoringVolume(logicTarget);

G4cout<< "ConstructTOFDebugDetector" << G4endl;
}
