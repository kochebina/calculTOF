#include "construction.hh"
#include "DebugDetector.hh"
#include "TOFDebugDetector.hh"
#include "TOFDebugDetector2.hh"
#include "TOFDebugDetector50.hh"
#include "ScanGE.hh"

#include "G4SDManager.hh"

MyDetectorConstruction::MyDetectorConstruction()
{
	fMessenger = new G4GenericMessenger(this, "/detector/", "Detector Construction");

	fMessenger->DeclareProperty("nCols", nCols, "Number of columns");
	fMessenger->DeclareProperty("nRows", nRows, "Number of rows");
	fMessenger->DeclareProperty("eCristal",e, "épaisseur (1e dim) des cristaux");
	fMessenger->DeclareProperty("eCristal",ly, "largeur (2e dim) des cristaux");

	nCols = 100;
	nRows = 100;

	e = 5*mm;
	ly = 5*mm;

	DefineMaterials();

	 xWorld = 2*m;
	 yWorld = 2*m;
	 zWorld = 2*m;

	  G4cout<< "MyDetectorConstruction" << G4endl;
}

MyDetectorConstruction::~MyDetectorConstruction()
{
	G4cout<< "~MyDetectorConstruction" << G4endl;
}

void MyDetectorConstruction::DefineMaterials()
{
	G4NistManager *nist = G4NistManager::Instance();

	//construction aerogel

	SiO2 = new G4Material("SiO2", 2.201*g/cm3, 2); //nom, densité, nombre éléments
	SiO2->AddElement(nist->FindOrBuildElement("Si"),1); // élément, quantité
	SiO2->AddElement(nist->FindOrBuildElement("O"),2);

	H2O = new G4Material("H2O", 1.000*g/cm3, 2);
	H2O->AddElement(nist->FindOrBuildElement("O"),1);
	H2O->AddElement(nist->FindOrBuildElement("H"),2);

	C = nist->FindOrBuildElement("C");

	/*
	Aerogel = new G4Material ("Aerogel", 0.200*g/cm3, 3);
	Aerogel->AddMaterial(SiO2, 62.5*perCent);
	Aerogel->AddMaterial(H2O, 37.4*perCent);
	Aerogel->AddElement(C, 0.1*perCent);

	G4MaterialPropertiesTable *mptAerogel = new G4MaterialPropertiesTable();
	mptAerogel->AddProperty("RINDEX", energy, rindexAerogel, 2); //2 valeurs d'indice

	Aerogel->SetMaterialPropertiesTable(mptAerogel);
	*/

	worldMat = nist->FindOrBuildMaterial("G4_AIR");

	//indice refraction aerogel et monde pour effet cherenkov

	G4double energy[2] = {1.239841939*eV/0.2, 1.239841939*eV/0.9}; //conversion moment du photon -> 0.2 : blue light, 0.9 : red light?
	//G4double rindexAerogel[2] = {1.1, 1.1};
	G4double rindexWorld[2] = {1.0, 1.0};

	G4double rindexLSO[2] = {1.78, 1.78}; //A modifier
	G4double reflectivity[2] = {1.0 ,1.0};

	G4MaterialPropertiesTable * mptWorld = new G4MaterialPropertiesTable();
	mptWorld->AddProperty("RINDEX", energy, rindexWorld, 2);

	worldMat->SetMaterialPropertiesTable(mptWorld);


	//LYSO-GE
	G4double fractionmass;
	LYSO_GE = new G4Material("LYSO_GE", 7.21*g/cm3, 4);
	LYSO_GE->AddElement(nist-> FindOrBuildElement("Lu"),fractionmass= 72.99*perCent);
	LYSO_GE->AddElement(nist-> FindOrBuildElement("Y"),fractionmass= 2.79*perCent);
	LYSO_GE->AddElement(nist-> FindOrBuildElement("Si"),fractionmass= 6.28*perCent);
	LYSO_GE->AddElement(nist-> FindOrBuildElement("O"),fractionmass= 17.94*perCent);

G4cout << "salutMaterial"<<G4endl;

	LSO = new G4Material("LSO", 7.4*g/cm3, 3);
	LSO->AddElement(nist-> FindOrBuildElement("Lu"),2);
	LSO->AddElement(nist-> FindOrBuildElement("Si"),1);
	LSO->AddElement(nist-> FindOrBuildElement("O"),5);
	/*

	G4double fraction[2] = {1.0,1.0}; //même intensité pour toutes les longueurs d'ondes

	G4MaterialPropertiesTable *mptLSO = new G4MaterialPropertiesTable();
	mptLSO->AddProperty("RINDEX",energy, rindexLSO,2);
	mptLSO->AddProperty("FASTCOMPONENT",energy,fraction, 2);
	mptLSO->AddConstProperty("SCINTILLATIONYIELD", 38./keV); //nbre photons/eV
	mptLSO->AddConstProperty("RESOLUTIONSCALE", 1.0);
	mptLSO->AddConstProperty("FASTTIMECONSTANT", 250*ns);
	mptLSO->AddConstProperty("YIELDRATIO", 1.);

	LSO-> SetMaterialPropertiesTable(mptLSO);
	*/




	mirrorSurface = new G4OpticalSurface("mirrorSurface");

	mirrorSurface ->SetType(dielectric_metal);
	mirrorSurface ->SetFinish(ground);
	mirrorSurface ->SetModel(unified);

	G4MaterialPropertiesTable *mptMirror = new G4MaterialPropertiesTable();

	mptMirror->AddProperty("REFLECTIVITY", energy, reflectivity, 2);

	mirrorSurface->SetMaterialPropertiesTable(mptMirror);

	G4cout<< "Definematerials" << G4endl;
}
/*
void MyDetectorConstruction::ConstructScintillator()
{

	solidScintillator = new G4Box("solidScintillator", e, ly, 6*mm);

	logicScintillator = new G4LogicalVolume(solidScintillator, LYSO_GE, "logicalScintillator");

	G4LogicalSkinSurface *skin = new G4LogicalSkinSurface("skin", logicWorld, mirrorSurface);

	fScoringVolume = logicScintillator;

	for (G4int i = 0; i <6; i++)  //# unit repeater -> Module
	{

		for (G4int j = 0; j< 16; j++)  //# module repeater (circular)
		{
			G4Rotate3D rotZ(j*22.5*deg, G4ThreeVector(0,0,1)); // 16 * 22.5 = 360
			G4Translate3D transXScint(G4ThreeVector(ly/tan(22.5/2*deg) + e, 0*mm, -40*mm +i*15*mm));

			G4Transform3D  transformScint = (rotZ)*(transXScint); //translation puis rotation
			physScintillator = new G4PVPlacement(transformScint, logicScintillator, "physScintillator", logicWorld, false,0,true);

		}
	}
	G4cout<< "ConstructScintillator" << G4endl;

}
*/
G4VPhysicalVolume *MyDetectorConstruction::Construct()
{

	solidWorld = new G4Box("solidWorld", xWorld, yWorld, zWorld);// x/2,y/2,z/2, en mètres

	logicWorld = new G4LogicalVolume(solidWorld, worldMat,"logicWorld");

	physWorld = new G4PVPlacement(0, G4ThreeVector(0.,0.,0.), logicWorld, "physWorld", 0, false, 0, true);


	//ConstructDebugGeom();
/*
	DebugDetector *d = new DebugDetector();
	d->Construct(logicWorld, LSO);
	this->SetScoringVolume(d->GetScoringVolume());
	*/
	ScanGE *scanner = new ScanGE();
	scanner->Construct(logicWorld,LYSO_GE,worldMat);
	this->SetScoringVolume(scanner->GetScoringVolume());
	G4cout<< "Construct" << G4endl;
	return physWorld;
}

void MyDetectorConstruction::ConstructSDandField()
{

	MySensitiveDetector *sensDet = new MySensitiveDetector("SensitiveDetector");

  	G4SDManager::GetSDMpointer()->AddNewDetector(sensDet); //sinon initialize de detector.cc refuse de coopérer


        // Setting aTrackerSD to all logical volumes with the same name
        // of "lCrystal".
	if (fScoringVolume != NULL)
	{
		fScoringVolume->SetSensitiveDetector(sensDet);
		//SetSensitiveDetector("lCrystal",sensDet,true);
	}

	G4cout<< "ConstructSDandField" << G4endl;
}
/*
//approximation
void MyDetectorConstruction::ConstructScanGE_OnlyCrystals()
{
	// 1.
	G4double x = 3.95 * mm;
	G4double y = 25. * mm;
	G4double z = 5.3 * mm; //en supposant que les repères soient équivalents -> à vérifier

	G4String Name6 = "Crystal";
	G4Box* Crystal = new G4Box(Name6, x, y, z); //solid
	lCrystal = new G4LogicalVolume(Crystal, LYSO_GE, "lCrystal");
	//pCrystal = new G4PVPlacement(t2, lCrystal, "pCrystal", lOptical, false, 0, true); //on inclut dans Optical

	// structure done

	// replication

	// crystal repeater -> optical unit cubic array

	G4int ringRepeatNumber = 28; // de 0 à 28 marche
	G4double ringRepeatAngle = 360.0 / ringRepeatNumber * deg;

	G4int unitRepeatNumber = 3; // de -2 à 3 marche
	G4double unitRepeatZ = 50.64 * 2 * mm; //pourquoi faut-il faire *2 pour que ça ne se chevauche pas ?

	G4int blockRepeatNumber = 2; // de -1 à 2
	G4double blockRepeatX = 16.20 * 2 * mm;

	G4int opticalRepeatNumber = 2; //de -1 à 2
	G4double opticalRepeatZ = 15.97 * 2 * mm;

	G4int crystalRepeatNumberX = 2; //de -1 à 2
	G4double crystalRepeatX = 3.9833 * 2 * mm;
	G4int crystalRepeatNumberZ = 2; //de -1 à 2
	G4double crystalRepeatZ = 5.3 * 2 * mm;


	fScoringVolume = lCrystal;

	for (G4int ring = 0; ring < ringRepeatNumber; ring++) // module repeater(circular) -> Ring
	{
		for (G4int unit = -2; unit < unitRepeatNumber; unit++) // unit repeater -> Module
		{
			for (G4int block = -1; block < blockRepeatNumber; block++) // block repeater -> unit
			{
				for (G4int optical = -1; optical < opticalRepeatNumber; optical++) // optical unit repeater -> block
				{
					for (G4int crystalX = -1; crystalX < crystalRepeatNumberX; crystalX++) // crystal repeater -> optical unit
					{
						//crystalRepeatX
						for (G4int crystalZ = -1; crystalZ < crystalRepeatNumberZ; crystalZ++) // crystal repeater -> optical unit
						{
							//crystalRepeatZ
							G4Translate3D transCrystalXZ(G4ThreeVector(crystalRepeatX * crystalX + blockRepeatX * block, 324.3 * mm, crystalRepeatZ * crystalZ + opticalRepeatZ * optical + unitRepeatZ * unit));
							G4Rotate3D rotRingZ(ring * ringRepeatAngle, G4ThreeVector(0, 0, 1));

							G4Transform3D  doAPET = (rotRingZ) * (transCrystalXZ); //translation puis rotation
							pCrystal = new G4PVPlacement(doAPET, lCrystal, "pCrystal", logicWorld, false, crystalX + crystalZ + ring + unit + block + optical, false); //on inclut dans Optical
						}
					}
				}
			}
		}
	}
}
*/
void MyDetectorConstruction::ConstructTOFTestStructure()
{
	G4double xTarget = 25.0*mm;
	G4double yTarget = 25.0 * mm;
	G4double zTarget = 25.0 * mm;
  solidTarget = new G4Box("solidTarget", xTarget, yTarget, zTarget);// x/2,y/2,z/2, en mètres

	logicTarget = new G4LogicalVolume(solidTarget, LYSO_GE,"logicTarget");
  G4double rMin = 290.0 * mm;
	physicTarget = new G4PVPlacement(0, G4ThreeVector(rMin,0.,0.), logicTarget, "physTarget", logicWorld, false, 0, true);
	physicTarget = new G4PVPlacement(0, G4ThreeVector(-rMin,0.,0.), logicTarget, "physTarget", logicWorld, false, 1, true);


G4cout<< "ConstructTOFTestStructure" << G4endl;

}

void MyDetectorConstruction::GenerateCastorOptionFile(G4float scanner_radius, // radius in mm (from isocenter to crystal surface)
                           //nb scanner elements
                           G4int nb_rsectors,
                           G4int nb_trans_mod,
                           G4int nb_axial_mod,
                           G4int nb_trans_submod,
                           G4int nb_axial_submod,
                           G4int nb_trans_crystal,
                           G4int nb_axial_crystal,
                           // Gaps between scanner elements
                           G4float gap_trans_mod,
                           G4float gap_axial_mod,
                           G4float gap_trans_submod,
                           G4float gap_axial_submod,
                           G4float gap_trans_crystal,
                           G4float gap_axial_crystal,
                           // crystal dimensions (mm)
                           G4float crystal_size_depth,
                           G4float crystal_size_trans,
                           G4float crystal_size_axial)
{
  G4cout<<"!!!!!!!! it  works!!!!! "<<G4endl;
  G4cout<< "scanner_radius "  << scanner_radius<<G4endl;
  G4cout<< "nb_rsectors "  << nb_rsectors<<G4endl;
  G4cout<< "nb_trans_mod " << nb_trans_mod<<G4endl;
  G4cout<< "nb_axial_mod " << nb_axial_mod<<G4endl;
  G4cout<< "nb_trans_submod " << nb_trans_submod<<G4endl;
  G4cout<< "nb_axial_submod " << nb_axial_submod<<G4endl;
  G4cout<< "nb_trans_crystal " << nb_trans_crystal<<G4endl;
  G4cout<< "nb_axial_crystal " << nb_axial_crystal<<G4endl;
  // Gaps between scanner elements
  G4cout<< "gap_trans_mod " << gap_trans_mod<<G4endl;
  G4cout<< "gap_axial_mod " << gap_axial_mod<<G4endl;
  G4cout<< "gap_trans_submod " << gap_trans_submod<<G4endl;
  G4cout<< "gap_axial_submod " << gap_axial_submod<<G4endl;
  G4cout<< "gap_trans_crystal " << gap_trans_crystal<<G4endl;
  G4cout<< "gap_axial_crystal " << gap_axial_crystal<<G4endl;
  // crystal dimensions (mm)
  G4cout<< "crystal_size_depth " << crystal_size_depth<<G4endl;
  G4cout<< "crystal_size_trans " << crystal_size_trans<<G4endl;
  G4cout<< "crystal_size_axial " << crystal_size_axial<<G4endl;


  std::ofstream myfile;
  myfile.open ("G4toCastorOptFile.txt");
  myfile << scanner_radius<<'\n';
  myfile << nb_rsectors<<'\n';
  myfile << nb_trans_mod<<'\n';
  myfile << nb_axial_mod<<'\n';
  myfile << nb_trans_submod<<'\n';
  myfile << nb_axial_submod<<'\n';
  myfile << nb_trans_crystal<<'\n';
  myfile << nb_axial_crystal<<'\n';
  // Gaps between scanner elements
  myfile << gap_trans_mod<<'\n';
  myfile << gap_axial_mod<<'\n';
  myfile << gap_trans_submod<<'\n';
  myfile << gap_axial_submod<<'\n';
  myfile << gap_trans_crystal<<'\n';
  myfile << gap_axial_crystal<<'\n';
  // crystal dimensions (mm)
  myfile << crystal_size_depth<<'\n';
  myfile << crystal_size_trans<<'\n';
  myfile << crystal_size_axial<<'\n';
  myfile.close();



}
