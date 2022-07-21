#include "ScanGE.hh"
#include "MetaDetector.hh"
ScanGE::ScanGE()
{	G4cout<< "ScanGE" << G4endl;}
ScanGE::~ScanGE()
{	G4cout<< "~ScanGE" << G4endl;
}

void ScanGE::Construct(G4LogicalVolume*logicWorld,G4Material* LYSO_GE, G4Material* worldMat)
{

	/* Dans Gate  :
	 1) Signa PET/MR camera definition
	 Hierarchy:
	   CylindricalPET
		 1.module (Gate level 1, rsector) -> module
		   2.unit   (Gate level 2, module) -> unit
			 3.block   (Gate level 3, submodule) -> block
			   4.optical unit  (Gate level 4, crystal) -> optical
				 5.crystal        (Gate level 5, layer0) -> crystal
	*/

	// structure

	G4String Name1 = "cylindricalPET";
	G4double pRMin = 290. * mm;
	G4double pRMax = 350. * mm;
	G4double pDz = 270. *0.5* mm;
	G4double startAngle = 0. * deg;
	G4double spanningAngle = 360. * deg;

	G4Tubs* cPET = new G4Tubs(Name1, pRMin, pRMax, pDz, startAngle, spanningAngle); //solid
	G4LogicalVolume *lPET = new G4LogicalVolume(cPET, worldMat, "lPET");
	G4VPhysicalVolume *pPET = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), lPET, "pPET", logicWorld, false, 0, true);


	// 1.

	G4String Name2 = "Module";
	G4double x_module = 64.5 *0.5* mm;
	G4double y_module = 25. *0.5* mm;
	G4double z_module = 250.4 *0.5* mm;
	//This is 311.8 mm radius + half depth 12.5
	G4double tx_module = 0;
	G4double ty_module = 324.3 * mm;
	G4double tz_module = 0;

	G4Box* Mod = new G4Box(Name2, x_module, y_module, z_module); //solid
	G4LogicalVolume *lMod = new G4LogicalVolume(Mod, worldMat, "lMod");


	// 2.

	G4String Name3 = "Unit";
	//x,y,tx et tz sont les mêmes que pour Module
	G4double x_unit = 64.5 *0.5* mm;
	G4double y_unit = 25. *0.5* mm;
	G4double z_unit = 47.84 *0.5* mm;


	//G4Translate3D t2(G4ThreeVector(tx_unit, ty_unit, tz_unit)); //vecteur nul en fait

	G4Box* Unit = new G4Box(Name3, x_unit, y_unit, z_unit); //solid
	G4LogicalVolume *lUnit = new G4LogicalVolume(Unit, worldMat, "lUnit");

	// 3.

	G4String Name4 = "Block";
	//y,z,tx,ty et tz sont les mêmes que pour Unit
	G4double x_block = 15.9 *0.5* mm;
	G4double y_block = 25. *0.5* mm;
	G4double z_block = 47.84 *0.5* mm;

	G4Box* Block = new G4Box(Name4, x_block, y_block, z_block); //solid
	G4LogicalVolume *lBlock = new G4LogicalVolume(Block, worldMat, "lBlock");

	// 4.

	G4String Name5 = "Optical";
	//x,y,tx,ty et tz sont les mêmes que pour Block
	G4double x_optical = 15.9*0.5*mm;
	G4double y_optical = 25*0.5*mm;
	G4double z_optical = 15.9 *0.5* mm;

	G4Box* Optical = new G4Box(Name5, x_optical, y_optical, z_optical); //solid
	G4LogicalVolume *lOptical = new G4LogicalVolume(Optical, worldMat, "lOptical");


	// 5.

	G4String Name6 = "Crystal";
	//y,tx,ty et tz sont les mêmes que pour Optical
	G4double x_crystal = 3.95 *0.5* mm;
	G4double y_crystal = 25*0.5*mm;
	G4double z_crystal = 5.3 *0.5* mm;

	G4Box* Crystal = new G4Box(Name6, x_crystal, y_crystal, z_crystal); //solid
	G4LogicalVolume *lCrystal = new G4LogicalVolume(Crystal, LYSO_GE, "lCrystal");

  this->SetScoringVolume(lCrystal);


	// structure done

	// replication

	// crystal repeater -> optical unit cubic array

	G4int ringRepeatNumber = 28; // de 0 à 28 marche
	G4double ringRepeatAngle = 360.0 / ringRepeatNumber * deg;

	G4int unitRepeatNumber = 5;//5; // de -2 à 3 marche
	G4double unitRepeatZ = 50.64 * mm; // (250.4 - 47.8) / (5-1)   (L-l)/(n-1)

	G4int blockRepeatNumber = 4;//4; // de -1 à 2
	G4double blockRepeatX = 16.20 *  mm;

	G4int opticalRepeatNumber = 3;//3; //de -1 à 2
	G4double opticalRepeatZ = 15.97 * mm;

	G4int crystalRepeatNumberX = 4;//4; //de -1 à 2
	G4double crystalRepeatX = 3.9833 *mm;
	G4int crystalRepeatNumberZ = 3;//3; //de -1 à 2
	G4double crystalRepeatZ = 5.3 *mm;


	//fScoringVolume = lCrystal;


	//place the modules in a ring along the axial direction

	for (G4int ring = 0; ring < ringRepeatNumber; ring++) // module repeater(circular) -> Ring
	{
		G4Rotate3D rotRingZ(ring * ringRepeatAngle, G4ThreeVector(0, 0, 1));

		G4Translate3D tRing(G4ThreeVector(tx_module, ty_module, tz_module));

		G4Transform3D  transformRing = (rotRingZ) * (tRing); //translation puis rotation
		G4VPhysicalVolume *pMod = new G4PVPlacement(transformRing, lMod, "pMod", lPET, false, ring, true); //on inclut dans le PET
	}


	for (G4int unit = 0; unit < unitRepeatNumber; unit++) // unit repeater -> Module
	{
		G4double unit_OffsetZ = unitRepeatZ * .5 * ( 1 - unitRepeatNumber );
		G4Translate3D transUnitZ(G4ThreeVector(0.*mm, 0. * mm, unit_OffsetZ + unitRepeatZ * unit ));//(z_unit/2) - (z_module/2) + unitRepeatZ * unit ));
		G4VPhysicalVolume *pUnit = new G4PVPlacement(transUnitZ, lUnit, "pUnit", lMod, false, unit, true); //on inclut dans le Module
	}

	for (G4int block = 0; block < blockRepeatNumber; block++) // block repeater -> unit
	{
		G4double block_OffsetX = blockRepeatX * .5 * ( 1 - blockRepeatNumber );
		G4Translate3D transBlockX(G4ThreeVector(block_OffsetX + blockRepeatX * block, 0. * mm, 0. * mm));

		G4VPhysicalVolume *pBlock = new G4PVPlacement(transBlockX, lBlock, "pBlock", lUnit, false, block, true); //on inclut dans Unit
	}
	for (G4int optical = 0; optical < opticalRepeatNumber; optical++) // optical unit repeater -> block
	{
		G4double optical_OffsetZ = opticalRepeatZ * .5 * ( 1 - opticalRepeatNumber );
		G4Translate3D transOpticalZ(G4ThreeVector(0. * mm, 0 * mm, optical_OffsetZ +opticalRepeatZ * optical));

		G4VPhysicalVolume *pOptical = new G4PVPlacement(transOpticalZ, lOptical, "pOptical", lBlock, false, optical, true); //on inclut dans Block
	}
	G4int monCrystalID = 0;
	for (G4int crystalZ = 0; crystalZ < crystalRepeatNumberZ; crystalZ++) // crystal repeater -> optical unit
	{
		for (G4int crystalX = 0; crystalX < crystalRepeatNumberX; crystalX++) // crystal repeater -> optical unit
		{

			G4double crystal_OffsetX = crystalRepeatX * .5 * ( 1 - crystalRepeatNumberX );
			G4double crystal_OffsetZ = crystalRepeatZ * .5 * ( 1 - crystalRepeatNumberZ );
			G4Translate3D transCrystalXZ(G4ThreeVector(crystal_OffsetX + crystalRepeatX * crystalX, 0 * mm, crystal_OffsetZ + crystalRepeatZ * crystalZ));

			G4VPhysicalVolume *pCrystal = new G4PVPlacement(transCrystalXZ, lCrystal, "pCrystal", lOptical, false, monCrystalID++, true); //on inclut dans Optical
		}
	}




	///////////////////// For Castor //////////////////////////
			 // Fill this part if the geometry changes a lot (not only sizes, number of crystals and their positions)
			 //
			 // axial = z
			 // trans = x
			 // unit = mod
			 // block = submod
			 // optical = ...
			 // crystal = crystal
			 //

			 G4double scanner_radius=(ty_module-y_module); // radius in mm (from isocenter to crystal surface)
			 //nb scanner elements
			 G4int nb_rsectors=ringRepeatNumber;
			 G4int nb_trans_mod=1;
			 G4int nb_axial_mod=unitRepeatNumber;
			 G4int nb_trans_submod=blockRepeatNumber;
			 G4int nb_axial_submod=opticalRepeatNumber; //1
			 G4int nb_trans_crystal=crystalRepeatNumberX;
			 G4int nb_axial_crystal=crystalRepeatNumberZ;//*opticalRepeatNumber;
			 // Gaps between scanner elements
			 G4double gap_trans_mod=0;
			 G4double gap_axial_mod=unitRepeatZ-z_unit*2;
			 G4double gap_trans_submod=blockRepeatX-x_block*2;
			 G4double gap_axial_submod=0;
			 G4double gap_trans_crystal=crystalRepeatX-x_crystal*2;
			 G4double gap_axial_crystal=crystalRepeatZ-z_crystal*2;
			 // crystal dimensions (mm)
			 G4double crystal_size_depth=y_crystal*2;
			 G4double crystal_size_trans=x_crystal*2;
			 G4double crystal_size_axial=z_crystal*2;

			 MyDetectorConstruction *constr=new MyDetectorConstruction();
			 constr->GenerateCastorOptionFile(scanner_radius,
													//nb scanner elements
													 nb_rsectors,
													 nb_trans_mod,
													 nb_axial_mod,
													 nb_trans_submod,
													 nb_axial_submod,
													 nb_trans_crystal,
													 nb_axial_crystal,
													// Gaps between scanner elements
													 gap_trans_mod,
													 gap_axial_mod,
													 gap_trans_submod,
													 gap_axial_submod,
													 gap_trans_crystal,
													 gap_axial_crystal,
													// crystal dimensions (mm)
													 crystal_size_depth,
													 crystal_size_trans,
													 crystal_size_axial);

			 /////////////////////////////////////////////////////////////////


	/*GenerateLUT(G4String modality, G4double scannerRadius, G4int nRings,
		G4int nElem, G4int nLayer, G4int nCrystByLayer,
		G4double crystDepth, G4double crystTransaxial, G4double crystAxial,
	G4int voxNTransaxial, G4int voxNAxial,
	G4double fovTransaxial, G4double fovAxial,
	G4double meanDOI, G4double minAngDiff,
	G4String description );*/


}

/*
GenerateHeaderLUT(G4String modality, G4double scannerRadius, G4int nRings,
	G4int nElem, G4int nLayer, G4int nCrystByLayer,
	G4double crystDepth, G4double crystTransaxial, G4double crystAxial,
G4int voxNTransaxial, G4int voxNAxial,
G4double fovTransaxial, G4double fovAxial,
G4double meanDOI, G4double minAngDiff,
G4String description )
{

	man->OpenFile("../root/outputs/test" + strRunID.str() + ".root");
}
*/
