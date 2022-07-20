#include "TOFDebugDetector2.hh"
#include "MetaDetector.hh"
TOFDebugDetector2::TOFDebugDetector2()
{	G4cout<< "TOFDebugDetector2" << G4endl;}
TOFDebugDetector2::~TOFDebugDetector2()
{	G4cout<< "~TOFDebugDetector2" << G4endl;
}

void TOFDebugDetector2::Construct(G4LogicalVolume*logicWorld,G4Material* LYSO_GE, G4Material* worldMat)
{


	G4String Name5 = "Optical";
	//x,y,tx,ty et tz sont les mêmes que pour Block
	G4double y_optical = 15.9*0.5*mm;
	G4double x_optical = 25*0.5*mm;
	G4double z_optical = 15.9 *0.5* mm;

	G4Box* Optical = new G4Box(Name5, x_optical, y_optical, z_optical); //solid
	G4LogicalVolume *lOptical = new G4LogicalVolume(Optical, worldMat, "lOptical");


	// 5.

	G4String Name6 = "Crystal";
	//y,tx,ty et tz sont les mêmes que pour Optical
	G4double z_crystal = 3.95 *0.5* mm;
	G4double x_crystal = 25*0.5*mm;
	G4double y_crystal = 5.3 *0.5* mm;

	G4Box* Crystal = new G4Box(Name6, x_crystal, y_crystal, z_crystal); //solid
	G4LogicalVolume *lCrystal = new G4LogicalVolume(Crystal, LYSO_GE, "lCrystal");

  this->SetScoringVolume(lCrystal);






	G4int crystalRepeatNumberZ = 4;
	G4double crystalRepeatZ = 3.9833 *mm;
	G4int crystalRepeatNumberY = 3;
	G4double crystalRepeatY = 5.3 *mm;



		G4Translate3D transOpticalZ(G4ThreeVector(100. * mm, 0. * mm, 5. * mm));
		G4VPhysicalVolume *pOptical = new G4PVPlacement(transOpticalZ, lOptical, "pOptical", logicWorld, false, 0, true);
    pOptical = new G4PVPlacement(0,G4ThreeVector(-100. * mm, 0. * mm, 5. * mm), lOptical, "pOptical", logicWorld, false, 1, true);

	G4int monCrystalID = 0;
	for (G4int crystalZ = 0; crystalZ < crystalRepeatNumberZ; crystalZ++) // crystal repeater -> optical unit
	{
		for (G4int crystalY = 0; crystalY < crystalRepeatNumberY; crystalY++) // crystal repeater -> optical unit
		{

			G4double crystal_OffsetY = crystalRepeatY * .5 * ( 1 - crystalRepeatNumberY );
			G4double crystal_OffsetZ = crystalRepeatZ * .5 * ( 1 - crystalRepeatNumberZ );
			G4Translate3D transCrystalYZ(G4ThreeVector(0 * mm, crystal_OffsetY + crystalRepeatY * crystalY,  crystal_OffsetZ + crystalRepeatZ * crystalZ));

			G4VPhysicalVolume *pCrystal = new G4PVPlacement(transCrystalYZ, lCrystal, "pCrystal", lOptical, false, monCrystalID++, true); //on inclut dans Optical
		}
	}
  this->SetScoringVolume(lCrystal);
}
