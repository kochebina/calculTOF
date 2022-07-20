#include "BackToBackGenerator.hh"
//#include "G4SPSAngDistribution.hh"
#include "G4SPSRandomGenerator.hh"
#include "G4SPSPosDistribution.hh"
//#include "Random.hh"
#include "G4RandomDirection.hh"
BackToBackGenerator::BackToBackGenerator(G4ParticleGun* Source,
  G4SPSAngDistribution* angGen,
  G4SPSRandomGenerator* bias,
  G4SPSPosDistribution *posGen)
{
   fSource = Source;
	 angGenerator = angGen;
	 biasRndm = bias;
	 posGenerator = posGen;
}

BackToBackGenerator::~BackToBackGenerator()
{
}

void BackToBackGenerator::Prepare()
{

	//fSource = new G4GeneralParticleSource();

	G4int n_particle = 1;
	fSource = new G4ParticleGun(n_particle);


	G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
	G4ParticleDefinition *particle = particleTable->FindParticle("gamma");




	fSource->SetParticleMomentum(511.*keV); //default is meV
	fSource->SetParticleDefinition(particle);

	//G4cout<< "MyPrimaryGenerator" << G4endl;

	biasRndm = new G4SPSRandomGenerator();

	posGenerator = new G4SPSPosDistribution();
	posGenerator->SetBiasRndm(biasRndm);

	angGenerator = new G4SPSAngDistribution();
	//angGenerator->SetPosDistribution(posGenerator);
	//angGenerator->SetBiasRndm(biasRndm);
	angGenerator->SetMinTheta(0 * deg);
	angGenerator->SetMaxTheta(180 * deg);
  angGenerator->SetMinPhi(0 * deg);
  angGenerator->SetMaxPhi(360 * deg);
	angGenerator->SetAngDistType("iso");


}


void BackToBackGenerator::Shoot(G4Event *anEvent, G4bool straightToX)
{
	//adapted from https://gitlab.cern.ch/geant4/geant4/-/blob/master/examples/extended/eventgenerator/particleGun/src/PrimaryGeneratorAction1.cc
	const G4double r = 10.*mm; //source linéaire
  const G4double zmax = 10*mm; //0 source ponctuelle  135 source linéaire de 270mm

  //vertex 1 uniform on cylinder
  //
  G4double alpha = CLHEP::twopi*G4UniformRand();  //alpha uniform in (0, 2*pi)
  G4double ux = std::cos(alpha);
  G4double uy = std::sin(alpha);
  G4double z = zmax*(2*G4UniformRand() - 1);  //z uniform in (-zmax, +zmax)

  G4ThreeVector pos; //initialisation obligatoire...
  if(straightToX) {pos.set(0,0,0);}
	else{pos.set(r*ux,r*uy,z);}
	//
  fSource->SetParticlePosition(pos);
//end adapted from


int i = 0;
G4AnalysisManager* man = G4AnalysisManager::Instance();

man->FillNtupleDColumn(0, i, pos[0]);
i++;
man->FillNtupleDColumn(0, i, pos[1]);
i++;
man->FillNtupleDColumn(0, i, pos[2]);
i++;
man->AddNtupleRow(0);

  G4ThreeVector mom;
  if (straightToX) {mom.set(1,0,0);}
	else{mom = G4RandomDirection();}

	fSource->SetParticleMomentumDirection(mom);
	fSource->GeneratePrimaryVertex(anEvent);

	fSource->SetParticleMomentumDirection(-mom);
  fSource->GeneratePrimaryVertex(anEvent);

	//G4cout<< "GeneratePrimaries" << G4endl;
}


/*
void BackToBackGenerator::Shoot(G4Event *anEvent, G4ThreeVector momValue)
{
	//adapted from https://gitlab.cern.ch/geant4/geant4/-/blob/master/examples/extended/eventgenerator/particleGun/src/PrimaryGeneratorAction1.cc
	const G4double r = 0.*mm; //source linéaire
  const G4double zmax = 0*mm; //0 source ponctuelle  135 source linéaire de 270mm

  //vertex 1 uniform on cylinder
  //
  G4double alpha = CLHEP::twopi*G4UniformRand();  //alpha uniform in (0, 2*pi)
  G4double ux = std::cos(alpha);
  G4double uy = std::sin(alpha);
  G4double z = zmax*(2*G4UniformRand() - 1);  //z uniform in (-zmax, +zmax)

  G4ThreeVector pos; //initialisation obligatoire...
  if(straightToX) {pos.set(0,0,0);}
	else{pos.set(r*ux,r*uy,z);}
	//
  fSource->SetParticlePosition(pos);
//end adapted from


int i = 0;
G4AnalysisManager* man = G4AnalysisManager::Instance();

man->FillNtupleDColumn(0, i, pos[0]);
i++;
man->FillNtupleDColumn(0, i, pos[1]);
i++;
man->FillNtupleDColumn(0, i, pos[2]);
i++;
man->AddNtupleRow(0);

  G4ThreeVector mom;
  if (straightToX) {mom.set(1,0,0);}
	else{mom = angGenerator->GenerateOne();}

	fSource->SetParticleMomentumDirection(mom);
	fSource->GeneratePrimaryVertex(anEvent);

	fSource->SetParticleMomentumDirection(-mom);
  fSource->GeneratePrimaryVertex(anEvent);

	//G4cout<< "GeneratePrimaries" << G4endl;
}
*/
