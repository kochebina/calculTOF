#ifndef BACKTOBACKGENERATOR_HH
#define BACKTOBACKGENERATOR_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleTable.hh"
#include "G4Geantino.hh"
#include "G4Gamma.hh"
#include "G4IonTable.hh"

#include "g4root.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SPSAngDistribution.hh"
#include "G4GeneralParticleSource.hh"

class BackToBackGenerator
{
public :
BackToBackGenerator(G4ParticleGun* Source,
	G4SPSAngDistribution* angGen,
	G4SPSRandomGenerator* bias,
	G4SPSPosDistribution *posGen);
	~BackToBackGenerator();

	void Prepare();
	void Shoot(G4Event *anEvent, G4bool straightToX);

private :
	G4ParticleGun* fSource;
	G4SPSAngDistribution* angGenerator;
	G4SPSRandomGenerator* biasRndm;
	G4SPSPosDistribution *posGenerator ;
};


#endif
