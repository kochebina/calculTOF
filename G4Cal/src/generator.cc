#include "generator.hh"
//#include "G4SPSAngDistribution.hh"
#include "G4SPSRandomGenerator.hh"
#include "G4SPSPosDistribution.hh"
//#include "Random.hh"

MyPrimaryGenerator::MyPrimaryGenerator()
{
BToB->Prepare();
}

MyPrimaryGenerator::~MyPrimaryGenerator()
{
	delete fSource;
	delete angGenerator;
	delete biasRndm;
	delete posGenerator;
	//G4cout<< "~MyPrimaryGenerator" << G4endl;
}

void MyPrimaryGenerator::GeneratePrimaries(G4Event *anEvent)
{
	G4bool	straightToX = false;
	BToB->Shoot(anEvent,straightToX);
}
