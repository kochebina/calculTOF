#ifndef SCAN_GE_HH
#define SCAN_GE_HH

#include "MetaDetector.hh"

class ScanGE : public MetaDetector //classe de base pour éviter de modifier directement construction.cc à chaque fois.
//On peut mimer les messengers avec des Get/set d'attributs et en créeant une classe qui hérite de celle-ci.
{
public :
	ScanGE();
	~ScanGE();

	virtual void Construct(G4LogicalVolume*logicWorld,G4Material* LYSO_GE, G4Material* worldMat);
	void GenerateHeaderLUT(G4String modality, G4double scannerRadius, G4int nRings,
		G4int nElem, G4int nLayer, G4int nCrystByLayer,
		G4double crystDepth, G4double crystTransaxial, G4double crystAxial,
	G4int voxNTransaxial, G4int voxNAxial,
G4double fovTransaxial, G4double fovAxial,
G4double meanDOI, G4double minAngDiff,
G4String description );
/*
modality: PET
scanner radius: 437.6
number of rings in scanner: 1
number of elements: 624
number of layers: 1
number of crystals in layer: 624
crystals size depth: 20
crystals size transaxial: 4
crystals size axial: 4
voxels number transaxial: 256
voxels number axial: 128
field of view transaxial: 768.
field of view axial: 256.
mean depth of interaction: -1
min angle difference: 90
description: 1 ring PET system
*/
void GenerateBinaryLUT();
/*
for(iLayer=0;iLayer <NLayer;iLayer++)
	for(iRing=0;iRing <NRing[iLayer];iLayer++)
		for(iDet=0;iDet <NDet[iLayer]/NRing[iLayer];iDet++)
		 {
			fwrite (& Posx[iLayer ][ iRing ][ iDet],sizeof(FLTNBLUT) ,1,fPtr);
			fwrite (& Posy[iLayer ][ iRing ][ iDet],sizeof(FLTNBLUT) ,1,fPtr);
			fwrite (& Posz[iLayer ][ iRing ][ iDet],sizeof(FLTNBLUT) ,1,fPtr);
		  fwrite (& OrVx[iLayer ][ iRing ][ iDet],sizeof(FLTNBLUT) ,1,fPtr);
		  fwrite (& OrVy[iLayer ][ iRing ][ iDet],sizeof(FLTNBLUT) ,1,fPtr);
		  fwrite (& OrVz[iLayer ][ iRing ][ iDet],sizeof(FLTNBLUT) ,1,fPtr);
		 }
*/

private :
G4LogicalVolume *MD_scoringVolume;

};

#endif
