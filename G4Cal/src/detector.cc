#include "detector.hh"
#include "construction.hh"
#include "myHit.hh"


#include "G4SDManager.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ios.hh"
#include "g4analysis.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"

#include "G4GDMLParser.hh"
//#include "G4GDMLReadSolids.hh"

#include <math.h>





MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name) ,HitsCollection(nullptr), ID(-1)
{
	collectionName.insert("myCollection");
	//G4cout<< "MySensitiveDetector" << G4endl;
}


MySensitiveDetector::~MySensitiveDetector()
{
	//G4cout<< "~MySensitiveDetector" << G4endl;
}


void MySensitiveDetector::Initialize(G4HCofThisEvent* hce)
{
	//G4cout<< "MySensitiveDetector::Initialize" << G4endl;
	HitsCollection = new myHitsCollection(SensitiveDetectorName,collectionName[0]);

	if (ID <0)
	{ID = G4SDManager::GetSDMpointer()->GetCollectionID(HitsCollection); }

	hce->AddHitsCollection(ID,HitsCollection);
	//G4cout << "*********************" << G4endl;
	//printf("beginning of event");
	//G4cout << "*********************" << G4endl;
	//G4cout<< "MySensitiveDetector::Initialize" << G4endl;

}



G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *R0hist)
{
	//G4cout<< "MySensitiveDetector::ProcessHits" << G4endl;

	/*
	G4int activity = 50000; //en bcq
	G4int dureeAcquisition = 1.0; //en s

	G4cout<< "activity" << activity << G4endl;

	G4int nbrEventPerRun = activity * dureeAcquisition; //nombre de d�sint�gration par run
	//event


	G4cout<< "nbrEventPerRun" << nbrEventPerRun << G4endl;
	G4double dt = (dureeAcquisition* 1.0/nbrEventPerRun*1.0); // = 1/activity, en s
	*/
	//G4cout<< "ProcessHits" << G4endl;

	// a mettre dans une classe "hits" ?
	G4double edep = aStep->GetTotalEnergyDeposit();
	//fEventAction->addEdep(edep);
	const G4Event * event = G4RunManager::GetRunManager()->GetCurrentEvent();
	G4int evt = event->GetEventID();
	G4double T0 = event->GetPrimaryVertex(0)->GetT0()/ps; //vaut 0 en fait. Tous les events sont g�n�r�s en m�me temps.


	//Track
	G4Track* track = aStep->GetTrack();
	G4int trackID = track->GetTrackID(); //track d�j� d�fini plus haut
	G4int stepNumber = track->GetCurrentStepNumber();
	G4int parentID = track->GetParentID();

	G4ParticleDefinition* particle = track->GetDefinition();
  	G4String particleName  = particle->GetParticleName();

	//step

	//stepPoint
	G4StepPoint* preStepPoint = aStep->GetPreStepPoint(); //arriv�e dans le d�tecteur du photon
	G4StepPoint* postStepPoint = aStep->GetPostStepPoint(); //sortie du d�tecteur

	//G4double time = 0;

	//time = time + dt * evt;// + T0 Illusion du temps qui passe, mais tous les events sont bien "simultan�s"

	/*
	G4cout << time << G4endl;
	G4cout << dt << G4endl;
	*/
	G4double localTime = postStepPoint->GetLocalTime();
	G4double properTime = postStepPoint->GetProperTime();
	G4double globalTime = postStepPoint->GetGlobalTime();

	//G4double globalTime = preStepPoint->GetGlobalTime();

	G4double totalEnergy = postStepPoint->GetTotalEnergy();
	G4double kEnergy = postStepPoint->GetKineticEnergy();
	G4ThreeVector posPhoton = postStepPoint->GetPosition();


	/*
	G4int opticalID = touchable->GetCopyNumber(1);
	G4int blockID = touchable->GetCopyNumber(2);
	G4int unitID = touchable->GetCopyNumber(3);
	G4int ModuleID = touchable->GetCopyNumber(4);
	*/

	//nom du process
	const G4VProcess* PreProcess = preStepPoint->GetProcessDefinedStep(); //Process which defined the current Step.
	G4String prepname = "";
	if (PreProcess) prepname = PreProcess->GetProcessName();
	const G4VProcess* PostProcess = postStepPoint->GetProcessDefinedStep(); //Process which defined the current Step.
	G4String postpname = "";
	if (PostProcess) postpname = PostProcess->GetProcessName(); //utilis� dans Gate


  //  For all processes except transportation, we select the PostStepPoint volume
  //  For the transportation, we select the PreStepPoint volume
	//G4cout<< "MySensitiveDetector::test" << G4endl;
  const G4VTouchable* touchable;
   if ( postpname == "Transportation" )
      {touchable = (const G4TouchableHistory*)(preStepPoint->GetTouchable() );}
  else
      {touchable = (const G4TouchableHistory*)(postStepPoint->GetTouchable() );}

	G4VPhysicalVolume* physVol = touchable->GetVolume(); //depth =0 par d�faut. 1 devrait donner celui juste au-dessus->faux
	G4String volName = physVol->GetName();
	G4int layerID = touchable->GetReplicaNumber(0);
	G4int crystalID = touchable->GetReplicaNumber(0);
	G4int submoduleID = touchable->GetReplicaNumber(0);
	G4int moduleID = touchable->GetReplicaNumber(0);
	G4int rsectorID = touchable->GetReplicaNumber(0);

//G4cout<< "MySensitiveDetector::test" << G4endl;
	/*
	G4SDManager* fSDM = G4SDManager::GetSDMpointer();
	G4RunManager* fRM = G4RunManager::GetRunManager();
	G4int collectionID = fSDM->GetCollectionID("crystal/test");
	const G4Event* currentEvent = fRM->GetCurrentEvent();
	G4HCofThisEvent* HCofEvent = currentEvent->GetHCofThisEvent();
	myHitsCollection* HitsCollection = (myHitsCollection*)(HCofEvent->GetHC(collectionID));

	G4cout<< HitsCollection << G4endl;
	G4cout<< ID << G4endl;
	G4cout<< test << G4endl;

	G4String HCname = HitsCollection->GetSDname();
        HCname += "/";
  	HCname += HitsCollection->GetName();
   	//G4cout << GetCollectionID(HCname) << G4endl;
   	G4cout << HCname << G4endl;

	//G4cout<< collectionID << G4endl;

	*/
	/*
	G4cout << "*********************" << G4endl;
	printf("HitInfoBeforeNTuple : \n");
	printf("trackNumber : %ld \n", trackID);
	printf("globalTime : %lf \n",globalTime);
	G4cout << "postProcessName : " << postpname << G4endl;
	printf("edep : %lf \n",edep);
	G4cout << "*********************" << G4endl;
*/

	//if ((postpname == "Transportation") && (edep==0.)){return true;} //gateCrystalHit.hh l.201
	if (edep <= 0) {return{true};}


	if (HitsCollection != NULL)
	{

		myHit* hit = new myHit(); //(*myHitsCollection)[hitNumber];
		hitNumber++;
	  if (trackID==1)
			{hitNumberTrack1++;}
	  else if (trackID==2)
			{hitNumberTrack2++;}

		hit->fillHitInfo(hitNumber,hitNumberTrack1, hitNumberTrack2, trackID, evt,parentID,
		posPhoton,
		edep,globalTime,
		layerID,
		crystalID,
		submoduleID,
		moduleID,
		rsectorID,
	  particleName);
		/*
		char* listOfTypes = "ds"; //d pour double, s pour string, i pour int. Rien d'autre. Mettre les args dans l'ordre.
		hit->FillNTuple(listOfTypes,2,kEnergy,postpname); //fonction variadique, tout argument optionnel
		*/
		hit->FillNTuple(); //fonction variadique, tout argument optionnel

		HitsCollection->insert(hit);
	}
	else {	G4cout<< "pbm" << G4endl;}


/*
	int i = 0;
	G4AnalysisManager* man = G4AnalysisManager::Instance();

	man->FillNtupleIColumn(2, i, evt);   //num�ro de l'event
	i++;

	man->FillNtupleDColumn(2, i, posPhoton[0] / mm); //interaction
	i++;

	man->FillNtupleDColumn(2, i, posPhoton[1] / mm);
	i++;

	man->FillNtupleDColumn(2, i, posPhoton[2] / mm);
	i++;

	man->FillNtupleDColumn(2, i, globalTime /ps); //comme l'unit� est la ns, /ps revient � multiplier par 1000
	i++;

	man->FillNtupleDColumn(2, i, localTime /ps); //
	i++;

	man->FillNtupleDColumn(2, i, properTime /ps); //
	i++;

	man->FillNtupleDColumn(2, i, T0 /ps); // "le" T0
	i++;

	man->FillNtupleSColumn(2, i, prepname); //S pour string
	i++;

	man->FillNtupleSColumn(2, i, postpname); //utilis� dans Gate
	i++;

	man->FillNtupleIColumn(2, i, trackID); //i pour int I,F,S et D sont les seuls types possibles
	i++;

	man->FillNtupleDColumn(2, i, totalEnergy/MeV);
	i++;

	man->FillNtupleDColumn(2, i, kEnergy/MeV);
	i++;

	man->FillNtupleIColumn(2, i, stepNumber);
	i++;

	man->FillNtupleIColumn(2, i, hitNumber);
	i++;

	man->FillNtupleIColumn(2, i, hitNumberTrack1);
	i++;

	man->FillNtupleIColumn(2, i, hitNumberTrack2);
	i++;

	man->FillNtupleIColumn(2, i, parentID);
	i++;

	man->FillNtupleSColumn(2, i, volName);
	i++;

	man->FillNtupleSColumn(2, i, particleName);
	i++;

	man->FillNtupleIColumn(2, i, layerID);
	i++;

	man->FillNtupleIColumn(2, i, crystalID);
	i++;

	man->FillNtupleIColumn(2, i, submoduleID);
	i++;

	man->FillNtupleIColumn(2, i, moduleID);
	i++;


	man->FillNtupleIColumn(2, i, rsectorID);
	i++;

	man->FillNtupleDColumn(2, i, edep/MeV);
	i++;

	man->AddNtupleRow(2);
	*/

	return true;
}

void MySensitiveDetector::EndOfEvent(G4HCofThisEvent* hce)
{
	G4VHitsCollection* hc = hce->GetHC(0);

	//ProcessCoincidences("WinnerTakeAll",hc); //WinnerTakeAll possible aussi
	ProcessCoincidencesTest("WinnerTakeAll",hc);
	hitNumber = 0;
	hitNumberTrack1 = 0;
	hitNumberTrack2 = 0;
	/*
	G4cout << "*********************" << G4endl;
	printf("end of event");
	G4cout << "*********************" << G4endl;
 */
}


void MySensitiveDetector::ProcessCoincidences(G4String policy, G4VHitsCollection* hc)
{
	//declarations
	myHit* hit;
	G4int trackID,eventID;
	G4int layerID1,crystalID1,submoduleID1,moduleID1,rsectorID1,layerID2,crystalID2,submoduleID2,moduleID2,rsectorID2;
	G4double edep;
	G4double time;
	G4double edepMax1;
	G4double edepMax2;
	G4double timeMin1,timeMin2;
	G4double energieTot1, energieTot2;
	G4double X1,Y1,Z1,X2,Y2,Z2;
	G4ThreeVector pos;
	G4bool gamma1;
	G4bool gamma2;

	//Initialisation

	G4int numberOfHits = hc->GetSize();
	/*
	G4cout << "*************************** "  << G4endl;
	G4cout << "nombre de hits dans l'event " << numberOfHits << G4endl;
	G4cout << "*************************** "  << G4endl;
*/
	//hit = static_cast<myHit*>(hc->GetHit(j)); don't know why it doesn't work here

	edepMax1 = 0;
	edepMax2 = 0;

	energieTot1 = 0;
	energieTot2 = 0;

	X1 = 0;
	Y1 = 0;
	Z1 = 0;

	X2 = 0;
	Y2 = 0;
	Z2 = 0;

	gamma1 = false;
	gamma2 = false;

	//processing des hits, donne id�alement deux singles � la fin de la boucle for
	if (policy == "WinnerTakeAll")
	{
		for (unsigned int j = 0; j < numberOfHits; j++)
		{
			hit = static_cast<myHit*>(hc->GetHit(j));
			trackID = hit->gethitTrackID();
			edep = hit->gethitEdep();
			pos = hit->gethitPos();
			time = hit->gethitTime();

				if (trackID == 1 && edep > 0)
				{
					energieTot1 = energieTot1 + edep;

					if (edep > edepMax1)
					{
						edepMax1 = edep;

						X1 =  pos[0];
						Y1 =  pos[1];
						Z1 =  pos[2];

						layerID1 = hit->gethitLayerID();
						crystalID1 = hit->gethitCrystalID();
						submoduleID1 = hit->gethitSubmoduleID();
						moduleID1 = hit->gethitModuleID();
						rsectorID1 = hit->gethitRsectorID();

					}
					if (time < timeMin1 || !gamma1) //deuxieme condition pour initialisation
					{
						timeMin1 = time;
					}
					gamma1 = true;
				}

				else if (trackID == 2 && edep > 0)
				{
					energieTot2 = energieTot2 + edep;

					if (edep > edepMax2)
					{
						edepMax2 = edep;

						X2 =  pos[0];
						Y2 =  pos[1];
						Z2 =  pos[2];

						layerID2 = hit->gethitLayerID();
						crystalID2 = hit->gethitCrystalID();
						submoduleID2 = hit->gethitSubmoduleID();
						moduleID2 = hit->gethitModuleID();
						rsectorID2 = hit->gethitRsectorID();

					}
					if (time < timeMin2 || !gamma2)
					{
						timeMin2 = time;
					}
					gamma2 = true;
				}
		}

	}
	else if (policy == "Centroid")
	{
		G4double
		layerID1temp = 0,
		layerID2temp = 0,
		crystalID1temp = 0,
		crystalID2temp = 0,
		submoduleID1temp = 0,
		submoduleID2temp = 0,
		moduleID1temp = 0,
		moduleID2temp = 0,
		rsectorID1temp = 0,
		rsectorID2temp = 0;

		for (unsigned int j = 0; j < numberOfHits; j++)
		{
			hit = static_cast<myHit*>(hc->GetHit(j));
			trackID = hit->gethitTrackID();
			edep = hit->gethitEdep();
			pos = hit->gethitPos();
			time = hit->gethitTime();


			if (trackID == 1 && edep > 0)
			{
				energieTot1 = energieTot1 + edep;

				X1 = X1 + pos[0] * edep;
				Y1 = Y1 + pos[1] * edep;
				Z1 = Z1 + pos[2] * edep;

				layerID1temp = layerID1temp + hit->gethitLayerID() * edep;
				crystalID1temp = crystalID1temp + hit->gethitCrystalID() * edep;
				submoduleID1temp = submoduleID1temp + hit->gethitSubmoduleID() * edep;
				moduleID1temp = moduleID1temp + hit->gethitModuleID() * edep;
				rsectorID1temp = rsectorID1temp + hit->gethitRsectorID() * edep;

				if (time < timeMin1 || !gamma1)
				{
					timeMin1 = time;
				}
				gamma1 = true;
			}

			else if (trackID == 2 && edep > 0)
			{
				energieTot2 = energieTot2 + edep;

				X2 = X2 + pos[0] * edep;
				Y2 = Y2 + pos[1] * edep;
				Z2 = Z2 + pos[2] * edep;

				layerID2temp = layerID2temp + hit->gethitLayerID() * edep;
				crystalID2temp = crystalID2temp + hit->gethitCrystalID() * edep;
				submoduleID2temp = submoduleID2temp + hit->gethitSubmoduleID() * edep;
				moduleID2temp = moduleID2temp + hit->gethitModuleID() * edep;
				rsectorID2temp = rsectorID2temp + hit->gethitRsectorID() * edep;

				if (time < timeMin2 || !gamma2)
				{
					timeMin2 = time;
				}
				gamma2 = true;
			}
		}
		X1 = X1 / energieTot1;
		Y1 = Y1 / energieTot1;
		Z1 = Z1 / energieTot1;

		X2 = X2 / energieTot2;
		Y2 = Y2 / energieTot2;
		Z2 = Z2 / energieTot2;

		layerID1 = round(layerID1temp / energieTot1);
		crystalID1 = round(crystalID1temp / energieTot1);
		submoduleID1 = round(submoduleID1temp / energieTot1);
		moduleID1 = round(moduleID1temp / energieTot1);
		rsectorID1 = round(rsectorID1temp / energieTot1);

		layerID2 = round(layerID2temp / energieTot2);
		crystalID2 = round(crystalID2temp / energieTot2);
		submoduleID2 = round(submoduleID2temp / energieTot2);
		moduleID2 = round(moduleID2temp / energieTot2);
		rsectorID2 = round(rsectorID2temp / energieTot2);
	}

eventID = hit->gethitEventID();
// remplissage singles //
if( gamma1)
{
	//remplissage des NTuples
 int i = 0;
 G4AnalysisManager* man = G4AnalysisManager::Instance();

 man->FillNtupleIColumn(3, i, eventID);
 i++;
 man->FillNtupleDColumn(3, i, X1 / mm);
 i++;
 man->FillNtupleDColumn(3, i, Y1 / mm);
 i++;
 man->FillNtupleDColumn(3, i, Z1 / mm);
 i++;
 man->FillNtupleDColumn(3, i, timeMin1 /ps); //comme l'unit� est la ns, /ps revient � multiplier par 1000
 i++;
 man->FillNtupleIColumn(3, i, layerID1);
 i++;
 man->FillNtupleIColumn(3, i, crystalID1);
 i++;
 man->FillNtupleIColumn(3, i, submoduleID1);
 i++;
 man->FillNtupleIColumn(3, i, moduleID1);
 i++;
 man->FillNtupleIColumn(3, i, rsectorID1);
 i++;
 man->FillNtupleDColumn(3, i, energieTot1 / MeV);
 i++;
 man->AddNtupleRow(3);
}

// remplissage singles //
if( gamma2)
{
	//remplissage des NTuples
 int i = 0;
 G4AnalysisManager* man = G4AnalysisManager::Instance();

 man->FillNtupleIColumn(3, i, eventID);
 i++;
 man->FillNtupleDColumn(3, i, X2 / mm);
 i++;
 man->FillNtupleDColumn(3, i, Y2 / mm);
 i++;
 man->FillNtupleDColumn(3, i, Z2 / mm);
 i++;
 man->FillNtupleDColumn(3, i, timeMin2 /ps);
 i++;
 man->FillNtupleIColumn(3, i, layerID2);
 i++;
 man->FillNtupleIColumn(3, i, crystalID2);
 i++;
 man->FillNtupleIColumn(3, i, submoduleID2);
 i++;
 man->FillNtupleIColumn(3, i, moduleID2);
 i++;
 man->FillNtupleIColumn(3, i, rsectorID2);
 i++;
 man->FillNtupleDColumn(3, i, energieTot2 / MeV);
 i++;
 man->AddNtupleRow(3);
}




	G4double fenetre = 1000; //1000ns � priori
	G4double dt,d;


// remplissage coincidences //
	if (gamma1 && gamma2)
	{
		dt = timeMin1 - timeMin2;
		d = distance(X1,X2,Y1,Y2,Z1,Z2);
		//if (dt < fenetre) //coincidence !
		//{
		    //remplissage des NTuples
			int i = 0;
			G4AnalysisManager* man = G4AnalysisManager::Instance();

			man->FillNtupleIColumn(4, i, eventID);
			i++;

			man->FillNtupleDColumn(4, i, X1 / mm);
			i++;
			man->FillNtupleDColumn(4, i, Y1 / mm);
			i++;
			man->FillNtupleDColumn(4, i, Z1 / mm);
			i++;

			man->FillNtupleDColumn(4, i, X2 / mm);
			i++;
			man->FillNtupleDColumn(4, i, Y2 / mm);
			i++;
			man->FillNtupleDColumn(4, i, Z2 / mm);
			i++;


			man->FillNtupleDColumn(4, i, timeMin1 /ps); //comme l'unit� est la ns, /ps revient � multiplier par 1000
			i++;
			man->FillNtupleDColumn(4, i, timeMin2 /ps);
			i++;
			/*
			G4cout << "*********************" << G4endl;
			printf("timeMin1 : %lf \n",timeMin1);
			printf("timeMin2 : %lf \n",timeMin2);
			G4cout << "*********************" << G4endl;
*/

			man->FillNtupleIColumn(4, i, layerID1);
			i++;
			man->FillNtupleIColumn(4, i, crystalID1);
			i++;
			man->FillNtupleIColumn(4, i, submoduleID1);
			i++;
			man->FillNtupleIColumn(4, i, moduleID1);
			i++;
			man->FillNtupleIColumn(4, i, rsectorID1);
			i++;
			man->FillNtupleDColumn(4, i, energieTot1 / MeV);
			i++;

			man->FillNtupleIColumn(4, i, layerID2);
			i++;
			man->FillNtupleIColumn(4, i, crystalID2);
			i++;
			man->FillNtupleIColumn(4, i, submoduleID2);
			i++;
			man->FillNtupleIColumn(4, i, moduleID2);
			i++;
			man->FillNtupleIColumn(4, i, rsectorID2);
			i++;
      man->FillNtupleDColumn(4, i, energieTot2 / MeV);
			i++;

			man->FillNtupleDColumn(4, i, dt / ns);
			i++;
			man->FillNtupleDColumn(4, i, d / mm);
			i++;

    	man->AddNtupleRow(4);

		//}
	//}
	/*else if (gamma1 && !gamma2) //gamma1 suffirait ?
	{
		dt = -1; //une valeur s�re ?
		//remplir avec gamma2 par d�faut ?
	}
	else if (!gamma1 && gamma2) //gamma2 suffirait ?
	{
		dt = -1; //une valeur s�re ?
		//remplir avec gamma1 par d�faut ?
	}*/
}
}
void MySensitiveDetector::ProcessCoincidencesTest(G4String policy, G4VHitsCollection* hc)
{
	//declarations
	myHit* hit;
	G4int trackID,eventID;
	G4int layerID1,crystalID1,submoduleID1,moduleID1,rsectorID1,layerID2,crystalID2,submoduleID2,moduleID2,rsectorID2;
	G4double edep;
	G4double time;
	G4double edepMax1;
	G4double edepMax2;
	G4double timeMin1,timeMin2;
	G4double energieTot1, energieTot2;
	G4double X1,Y1,Z1,X2,Y2,Z2;
	G4ThreeVector pos;
	G4bool gamma1;
	G4bool gamma2;
	G4String particleName;

	//Initialisation

	G4int numberOfHits = hc->GetSize();


	edepMax1 = 0;
	edepMax2 = 0;

	energieTot1 = 0;
	energieTot2 = 0;

	X1 = 0;
	Y1 = 0;
	Z1 = 0;

	X2 = 0;
	Y2 = 0;
	Z2 = 0;

	gamma1 = false;
	gamma2 = false;

	//processing des hits, donne id�alement deux singles � la fin de la boucle for

	if (policy == "WinnerTakeAll")
	{
		for (unsigned int j = 0; j < numberOfHits; j++)
		{
			hit = static_cast<myHit*>(hc->GetHit(j));
			trackID = hit->gethitTrackID();
			edep = hit->gethitEdep();
			pos = hit->gethitPos();
			time = hit->gethitTime();
			hitNumberTrack1 = hit->gethitNumberTrack1();
			hitNumberTrack2 = hit->gethitNumberTrack2();
			particleName = hit->gethitParticleName();

			if (particleName == "gamma")
			{
				if (trackID == 1)
				{
					energieTot1 = energieTot1 + edep;

					if (edep > edepMax1)
					{
						edepMax1 = edep;

						X1 =  pos[0];
						Y1 =  pos[1];
						Z1 =  pos[2];

						layerID1 = hit->gethitLayerID();
						crystalID1 = hit->gethitCrystalID();
						submoduleID1 = hit->gethitSubmoduleID();
						moduleID1 = hit->gethitModuleID();
						rsectorID1 = hit->gethitRsectorID();

					//}
					//if (hitNumberTrack1==1 || !gamma1 )
					//{
						timeMin1 = time;
					}
					gamma1 = true;
				}

				else if (trackID == 2)
				{
					energieTot2 = energieTot2 + edep;

					if (edep > edepMax2)
					{
						edepMax2 = edep;

						X2 =  pos[0];
						Y2 =  pos[1];
						Z2 =  pos[2];

						layerID2 = hit->gethitLayerID();
						crystalID2 = hit->gethitCrystalID();
						submoduleID2 = hit->gethitSubmoduleID();
						moduleID2 = hit->gethitModuleID();
						rsectorID2 = hit->gethitRsectorID();

					// }
					// if (hitNumberTrack2 ==2 || !gamma2 )
					// {
						timeMin2 = time;
					}
					gamma2 = true;
				}
			}
			//else {G4cout << "e- caught"<<G4endl;}
	}
}
eventID = hit->gethitEventID();
// remplissage singles //
if( gamma1)
{
	//remplissage des NTuples
 int i = 0;
 G4AnalysisManager* man = G4AnalysisManager::Instance();

 man->FillNtupleIColumn(3, i, eventID);
 i++;
 man->FillNtupleDColumn(3, i, X1 / mm);
 i++;
 man->FillNtupleDColumn(3, i, Y1 / mm);
 i++;
 man->FillNtupleDColumn(3, i, Z1 / mm);
 i++;
 man->FillNtupleDColumn(3, i, timeMin1 /ps); //comme l'unit� est la ns, /ps revient � multiplier par 1000
 i++;
 man->FillNtupleIColumn(3, i, layerID1);
 i++;
 man->FillNtupleIColumn(3, i, crystalID1);
 i++;
 man->FillNtupleIColumn(3, i, submoduleID1);
 i++;
 man->FillNtupleIColumn(3, i, moduleID1);
 i++;
 man->FillNtupleIColumn(3, i, rsectorID1);
 i++;
 man->FillNtupleDColumn(3, i, energieTot1 / MeV);
 i++;
 man->AddNtupleRow(3);
}

// remplissage singles //
if( gamma2)
{
	//remplissage des NTuples
 int i = 0;
 G4AnalysisManager* man = G4AnalysisManager::Instance();

 man->FillNtupleIColumn(3, i, eventID);
 i++;
 man->FillNtupleDColumn(3, i, X2 / mm);
 i++;
 man->FillNtupleDColumn(3, i, Y2 / mm);
 i++;
 man->FillNtupleDColumn(3, i, Z2 / mm);
 i++;
 man->FillNtupleDColumn(3, i, timeMin2 /ps);
 i++;
 man->FillNtupleIColumn(3, i, layerID2);
 i++;
 man->FillNtupleIColumn(3, i, crystalID2);
 i++;
 man->FillNtupleIColumn(3, i, submoduleID2);
 i++;
 man->FillNtupleIColumn(3, i, moduleID2);
 i++;
 man->FillNtupleIColumn(3, i, rsectorID2);
 i++;
 man->FillNtupleDColumn(3, i, energieTot2 / MeV);
 i++;
 man->AddNtupleRow(3);
}




	G4double fenetre = 1000; //1000ns � priori
	G4double dt,d;


// remplissage coincidences //
	if (gamma1 && gamma2)
	{
		//dt = (timeMin1 - timeMin2) * (G4UniformRand() * 2 -1);
		dt = (timeMin1 - timeMin2);
		d = distance(X1,X2,Y1,Y2,Z1,Z2);
		//if (dt < fenetre) //coincidence !
		//{
		    //remplissage des NTuples
			int i = 0;
			G4AnalysisManager* man = G4AnalysisManager::Instance();

			man->FillNtupleIColumn(4, i, eventID);
			i++;

			man->FillNtupleDColumn(4, i, X1 / mm);
			i++;
			man->FillNtupleDColumn(4, i, Y1 / mm);
			i++;
			man->FillNtupleDColumn(4, i, Z1 / mm);
			i++;

			man->FillNtupleDColumn(4, i, X2 / mm);
			i++;
			man->FillNtupleDColumn(4, i, Y2 / mm);
			i++;
			man->FillNtupleDColumn(4, i, Z2 / mm);
			i++;


			man->FillNtupleDColumn(4, i, timeMin1 /ps); //comme l'unit� est la ns, /ps revient � multiplier par 1000
			i++;
			man->FillNtupleDColumn(4, i, timeMin2 /ps);
			i++;
			/*
			G4cout << "*********************" << G4endl;
			printf("timeMin1 : %lf \n",timeMin1);
			printf("timeMin2 : %lf \n",timeMin2);
			G4cout << "*********************" << G4endl;
*/

			man->FillNtupleIColumn(4, i, layerID1);
			i++;
			man->FillNtupleIColumn(4, i, crystalID1);
			i++;
			man->FillNtupleIColumn(4, i, submoduleID1);
			i++;
			man->FillNtupleIColumn(4, i, moduleID1);
			i++;
			man->FillNtupleIColumn(4, i, rsectorID1);
			i++;
			man->FillNtupleDColumn(4, i, energieTot1 / MeV);
			i++;

			man->FillNtupleIColumn(4, i, layerID2);
			i++;
			man->FillNtupleIColumn(4, i, crystalID2);
			i++;
			man->FillNtupleIColumn(4, i, submoduleID2);
			i++;
			man->FillNtupleIColumn(4, i, moduleID2);
			i++;
			man->FillNtupleIColumn(4, i, rsectorID2);
			i++;
      man->FillNtupleDColumn(4, i, energieTot2 / MeV);
			i++;

			man->FillNtupleDColumn(4, i, dt / ps);
			i++;
			man->FillNtupleDColumn(4, i, d / mm);
			i++;

    	man->AddNtupleRow(4);

}
}





G4double MySensitiveDetector::distance(G4double x1,G4double x2,G4double y1,G4double y2,G4double z1,G4double z2)
{
	return sqrt(pow(x1-x2,2) + pow(y1-y2,2) + pow(z1-z2,2));
}
