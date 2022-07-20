#include "single.hh"
#include "TTree.h"
#include "TRandom.h"
#include <TMath.h>
#include <iostream>
#include <math.h>


single::single(TTree* Hits)
{
std::cout << "single::single" << std::endl;
}


single::~single(){delete Single; std::cout << "single::~single" << std::endl;}


void single::createTreeSingle(TTree* Hits, TFile* outputFile)
{
  /*const TString pattern = "positron";
  if (filename.Contains(pattern)) {fillTreeSingle_positron(Hits,filename);}
  else if (filename.Contains("v2")) {fillTreeSingle_b2b_v2(Hits,filename);}*/
  fillTreeSingle_b2b_WTA(Hits,outputFile);


  std::cout << "single::createTreeSingle" << std:: endl;
}

void single::fillTreeSingle_positron(TTree* Hits, TString filename)
{
//A remplir


std::cout << "single::fillTreeSingle_positron" << std:: endl;
}
void single::fillTreeSingle_b2b_WTA(TTree* Hits, TFile* outputFile)
{
	//variables pour le single
	Double_t energie, energie1, energie2, globalPosX,temps,globalPosY, globalPosZ;
	Int_t STrackID;
	Int_t SeventID;
	Int_t SrsectorID,SmoduleID,SsubmoduleID,ScrystalID,SlayerID;
	Int_t SrsectorID1,SmoduleID1,SsubmoduleID1,ScrystalID1,SlayerID1;
	Int_t SrsectorID2,SmoduleID2,SsubmoduleID2,ScrystalID2,SlayerID2;
	Double_t x1, y1, z1, x2, y2, z2;
  Int_t Scoincidence;

	// define the variable(s) of interest, type of variable must be respected
	Double_t X, Y, Z, time, edep; //edep : pondération (éventuelle) du centroïde
	Int_t eventID;
	Int_t trackID;
  Int_t rsectorID,moduleID,submoduleID,crystalID,layerID;
  Char_t particleName; //Char_t* -> seg fault. string : pas le bon type...

	//branches d'intérêt
	TBranch* bX;
	TBranch* bY;
	TBranch* bZ;
	TBranch* beventID;
	TBranch* btrackID;
	TBranch* btime;
	TBranch* bedep;
	TBranch* brsectorID;
	TBranch* bmoduleID;
	TBranch* bsubmoduleID;
	TBranch* bcrystalID;
	TBranch* blayerID;
  TBranch* bparticleName;

	// Set branch address
	Hits->SetBranchAddress("posX", &X, &bX);
	Hits->SetBranchAddress("posY", &Y, &bY);
	Hits->SetBranchAddress("posZ", &Z, &bZ);
	Hits->SetBranchAddress("eventID", &eventID, &beventID);
	Hits->SetBranchAddress("trackID", &trackID, &btrackID);
	Hits->SetBranchAddress("time", &time, &btime);
	//Hits->SetBranchAddress("GateTime", &time, &btime); //positron
	Hits->SetBranchAddress("edep", &edep, &bedep);
	Hits->SetBranchAddress("rsectorID", &rsectorID, &brsectorID);
	Hits->SetBranchAddress("moduleID", &moduleID, &bmoduleID);
	Hits->SetBranchAddress("submoduleID", &submoduleID, &bsubmoduleID);
	Hits->SetBranchAddress("crystalID", &crystalID, &bcrystalID);
	Hits->SetBranchAddress("layerID", &layerID, &blayerID);
  Hits->SetBranchAddress("particleName", &particleName, &bparticleName);

	// Get number of hits in the TTree
	int nH = (int)Hits->GetEntries();

	//initialisation

	 this->Single->Branch("globalPosX",&globalPosX,"globalPosX/D");
	 this->Single->Branch("globalPosY",&globalPosY,"globalPosY/D");
	 this->Single->Branch("globalPosZ",&globalPosZ,"globalPosZ/D");
	 this->Single->Branch("time",&temps,"time/D");
	 this->Single->Branch("energy",&energie,"energy/D");
	 this->Single->Branch("trackID",&STrackID,"trackID/I");
	 this->Single->Branch("eventID",&SeventID,"eventID/I");
	 this->Single->Branch("rsectorID",&SrsectorID,"rsectorID/I");
	 this->Single->Branch("moduleID",&SmoduleID,"moduleID/I");
	 this->Single->Branch("submoduleID",&SsubmoduleID,"submoduleID/I");
	 this->Single->Branch("crystalID",&ScrystalID,"crystalID/I");
	 this->Single->Branch("layerID",&SlayerID,"layerID/I");
   this->Single->Branch("coincidence",&Scoincidence,"coincidence/I");

	bool gamma1, gamma2;
	gamma1 = false;
	gamma2 = false;
	int i = 0;
	int currentEvent = 0;
	Double_t energieTot1;
	Double_t energieTot2;
	Double_t temps1;
	Double_t temps2;


  //variables debug
  int nbrEvent = 0;

	// Loop over hits
	while ( (i < nH) )
	{
		energie1 = 0;
		energie2 = 0;
		energieTot1 = 0;
    energieTot2 = 0;
    x1 = 0;
		y1 = 0;
		z1 = 0;
		x2 = 0;
		y2 = 0;
		z2 = 0;
		//std ::cout <<"debutEvent  " << "eventID : " <<  eventID << "currentEvent : " << currentEvent << std :: endl;

    btrackID->GetEntry(i);
    beventID->GetEntry(i);
    bX->GetEntry(i);
    bY->GetEntry(i);
    bZ->GetEntry(i);
    bedep->GetEntry(i);
    btime->GetEntry(i);
    brsectorID->GetEntry(i);
    bmoduleID->GetEntry(i);
    bsubmoduleID->GetEntry(i);
    bcrystalID->GetEntry(i);
    blayerID->GetEntry(i);
    bparticleName->GetEntry(i);

    currentEvent = eventID;
    temps1 = time ; //on ne peut pas initialiser à 0, il n'y a pas de "temps max"
		temps2 = time ;
    nbrEvent++;
		while ((i < nH) && (eventID == currentEvent))
		{
      if (particleName == 'g')
      {
				if (trackID == 1)
				{
					energieTot1 = energieTot1 + edep;
					if (energie1 < edep)
					{
						x1 = X;
						y1 = Y;
						z1 = Z;

            SrsectorID1 = rsectorID;
            SmoduleID1 = moduleID;
            SsubmoduleID1 = submoduleID;
            ScrystalID1 = crystalID;
            SlayerID1 = layerID;

						energie1 = edep;

            temps1 = time; //comme dans Gate
					}
					gamma1 = true;
				}

  				else if (trackID == 2)
  				{
  					energieTot2 = energieTot2 + edep;
  					if (energie2 < edep)
  					{
  						x2 = X;
  						y2 = Y;
  						z2 = Z;

              SrsectorID2 = rsectorID;
              SmoduleID2 = moduleID;
              SsubmoduleID2 = submoduleID;
              ScrystalID2 = crystalID;
              SlayerID2 = layerID;

  						energie2 = edep;

              temps2 = time;
  					}

  					//if (time < temps2) {temps2 = time;}
  					gamma2 = true;
  				}
        }
				i++;
        btrackID->GetEntry(i);
				beventID->GetEntry(i);
				bX->GetEntry(i);
				bY->GetEntry(i);
				bZ->GetEntry(i);
				bedep->GetEntry(i);
				btime->GetEntry(i);
				brsectorID->GetEntry(i);
				bmoduleID->GetEntry(i);
				bsubmoduleID->GetEntry(i);
				bcrystalID->GetEntry(i);
				blayerID->GetEntry(i);
        bparticleName->GetEntry(i);
			}

			//std ::cout << i << std :: endl;
		//std ::cout <<"finEvent  " << "eventID : " <<  eventID << "currentEvent : " << currentEvent << std :: endl;
      if (gamma1 && energieTot1>0)
      {
				globalPosX = x1;
				globalPosY = y1;
				globalPosZ = z1;
				temps = temps1 * 1000; //G4 est en ns par défaut, ici on passe en ps
				energie = energieTot1;
				STrackID = 1;
				SeventID = eventID;
				SrsectorID = SrsectorID1; //faux pour le moment
				SmoduleID = SmoduleID1;
				SsubmoduleID = SsubmoduleID1;
				ScrystalID = ScrystalID1;
				SlayerID = SlayerID1; //jusqu'ici
        if (gamma1 && gamma2) {Scoincidence = 1;} else {Scoincidence = 0;}
				this->Single->Fill();
      }

      if (gamma2 && energieTot2>0)
      {
				globalPosX = x2;
				globalPosY = y2;
				globalPosZ = z2;
				temps = temps2 * 1000; //G4 est en ns par défaut, ici on passe en ps
				energie = energieTot2;
				STrackID = 2;
				SeventID = eventID;
				SrsectorID = SrsectorID2;
				SmoduleID = SmoduleID2;
				SsubmoduleID = SsubmoduleID2;
				ScrystalID = ScrystalID2;
				SlayerID = SlayerID2;
        if (gamma1 && gamma2) {Scoincidence = 1;} else {Scoincidence = 0;}
				this->Single->Fill();
      }
      gamma1 = false;
      gamma2 = false;
	}
  std ::cout <<nbrEvent << std :: endl;
  std ::cout << this->Single->GetMaximum("eventID") << std :: endl;


  outputFile->WriteObject(this->Single, "Singles");

	//writeTree(this->Single,filename);

	Hits->ResetBranchAddresses();
	this->Single->ResetBranchAddresses();
  /*
	std::cout << " hits de particules secondaires pour ce run : " << ps << std:: endl;
	std::cout << " nombre d'events traités : " << j << "test : " << deuxGammasCount + (s1 - deuxGammasCount) + (s2 - deuxGammasCount) <<  std:: endl;
  std::cout << " dernier eventID : " << eventID << std:: endl;
  std::cout << " nombre d'events ayant deux gammas i.e. de coincidences prévues : " << deuxGammasCount << std:: endl;
	std::cout << " nombre de hits dans le track 1 : " << c1 << std:: endl;
	std::cout << " nombre de hits dans le track 2 : " << c2 << std:: endl;
	std::cout << " nombre de singles dans le track 1 : " << s1 << std:: endl;
	std::cout << " nombre de singles dans le track 2 : " << s2 << std:: endl;
	std::cout << " nombre de singles du track1 non détectés : " << ns1 << std:: endl;
	std::cout << " nombre de singles du track2 non détectés : " << ns2 << std:: endl;
	std::cout << " nombre de singles théorique : " << 1000000 <<" nombre de singles réel : " << this->Single->GetEntries() << std:: endl;
	std::cout << " nombre de hits : " << nH << "test : " << c1 + c2 + ps << std:: endl;
	std::cout << "single::fillTreeSingle_b2b" << std:: endl;
  */
}

void single::fillTreeSingle_b2b_v2(TTree* Hits, TString filename)
{
	//variables pour le single
	Double_t energie, energie1, energie2, globalPosX,temps,globalPosY, globalPosZ;
	Int_t STrackID,SlayerID;
	Double_t x1, y1, z1, x2, y2, z2;
	Double_t energieTot1;
	Double_t energieTot2;
	Double_t temps1;
	Double_t temps2;

	// define the variable(s) of interest, type of variable must be respected
	Double_t X, Y, Z, time, edep; //edep : pondération (éventuelle) du centroïde
	Int_t eventID;
	Int_t trackID;
	Int_t layerID;

	//branches d'intérêt
	TBranch* bX;
	TBranch* bY;
	TBranch* bZ;
	TBranch* beventID;
	TBranch* btrackID;
	TBranch* btime;
	TBranch* bedep;
	TBranch* blayerID;

	// Set branch address
	Hits->SetBranchAddress("posX", &X, &bX);
	Hits->SetBranchAddress("posY", &Y, &bY);
	Hits->SetBranchAddress("posZ", &Z, &bZ);
	Hits->SetBranchAddress("eventID", &eventID, &beventID);
	Hits->SetBranchAddress("trackID", &trackID, &btrackID);
	Hits->SetBranchAddress("layerID", &layerID, &blayerID);
	Hits->SetBranchAddress("globalTime", &time, &btime);
	//Hits->SetBranchAddress("GateTime", &time, &btime); //positron
	Hits->SetBranchAddress("edep", &edep, &bedep);

	// Get number of hits in the TTree
	int nH = (int)Hits->GetEntries();

	//initialisation

	 this->Single->Branch("globalPosX",&globalPosX,"globalPosX/D");
	 this->Single->Branch("globalPosY",&globalPosY,"globalPosY/D");
	 this->Single->Branch("globalPosZ",&globalPosZ,"globalPosZ/D");
	 this->Single->Branch("time",&temps,"time/D");
	 this->Single->Branch("energy",&energie,"energy/D");
	 //this->Single->Branch("trackID",&STrackID,"trackID/I");
	 this->Single->Branch("layerID",&SlayerID,"layerID/I");


	 int eventCourant, layerCourant;
	 int i = 0;
	 while(i < nH)
	 {
	 	beventID->GetEntry(i);
	 	eventCourant = eventID;

	 	btime->GetEntry(i);
		temps1 = time ;
		temps2 = time ;

		energie1 = 0;
		energie2 = 0;
		energieTot1 = 0;
	        energieTot2 = 0;


	        x1 = 0;
		y1 = 0;
		z1 = 0;
		x2 = 0;
		y2 = 0;
		z2 = 0;


	 	while ((i<nH) && (eventCourant == eventID))
	 	{
	 		blayerID->GetEntry(i);
	 		layerCourant = layerID;

	 		while ((i<nH) && (layerCourant == layerID) && (eventCourant == eventID))
	 		{
	 			bX->GetEntry(i);
				bY->GetEntry(i);
				bZ->GetEntry(i);
				bedep->GetEntry(i);
				btime->GetEntry(i);
				btrackID->GetEntry(i);
				if (trackID == 1)
				{
		 			energieTot1 = energieTot1 + edep;

					x1 = x1 + X * edep;
					y1 = y1 + Y * edep;
					z1 = z1 + Z * edep;

					if (time < temps1) {temps1 = time;}
				}
				if (trackID == 2)
				{
		 			energieTot2 = energieTot2 + edep;

					x2 = x2 + X * edep;
					y2 = y2 + Y * edep;
					z2 = z2 + Z * edep;

					if (time < temps2) {temps2 = time;}
				}
				i++;
				blayerID->GetEntry(i);
				beventID->GetEntry(i);
	 		}
	 		if (energieTot1 >0)
	 		{
		 		globalPosX = x1/energieTot1;
				globalPosY = y1/energieTot1;
				globalPosZ = z1/energieTot1;
				temps = temps1;
				SlayerID = layerID;
				energie = energieTot1;
				this->Single->Fill();
			}
			energieTot1 = 0;
			x1 = 0;
			y1 = 0;
			z1 = 0;
			btime->GetEntry(i);
			temps1 = time ;
			if (energieTot2 >0)
	 		{
		 		globalPosX = x2/energieTot2;
				globalPosY = y2/energieTot2;
				globalPosZ = z2/energieTot2;
				temps = temps2;
				SlayerID = layerID;
				energie = energieTot2;
				this->Single->Fill();
			}
			energieTot2 = 0;
			x2 = 0;
			y2 = 0;
			z2 = 0;
			btime->GetEntry(i);
			temps2 = time ;
	 	}

	 }



	writeTree(this->Single,filename);

	Hits->ResetBranchAddresses();
	this->Single->ResetBranchAddresses();

	std::cout << "single::fillTreeSingle_b2b_v2" << std:: endl;

}

void single::fillTreeSingle_Gate(TTree* Hits) // a modifier
{
	//variables pour le single
	Float_t energie, energie1, energie2, globalPosX,temps, globalPosY, globalPosZ;
	Int_t STrackID;
	Int_t SeventID;
	Float_t x1, y1, z1, x2, y2, z2;

	// define the variable(s) of interest, type of variable must be respected
	Float_t X, Y, Z, edep; //edep : pondération (éventuelle) du centroïde
	Double_t time;
	Int_t eventID;
	Int_t trackID;

	//branches d'intérêt
	TBranch* bX;
	TBranch* bY;
	TBranch* bZ;
	TBranch* beventID;
	TBranch* btrackID;
	TBranch* btime;
	TBranch* bedep;

	// Set branch address
	Hits->SetBranchAddress("posX", &X, &bX);
	Hits->SetBranchAddress("posY", &Y, &bY);
	Hits->SetBranchAddress("posZ", &Z, &bZ);
	Hits->SetBranchAddress("eventID", &eventID, &beventID);
	Hits->SetBranchAddress("trackID", &trackID, &btrackID);
	Hits->SetBranchAddress("time", &time, &btime);
	Hits->SetBranchAddress("edep", &edep, &bedep);

	// Get number of hits in the TTree
	int nH = (int)Hits->GetEntries();

	//initialisation

	 this->Single->Branch("globalPosX",&globalPosX,"globalPosX/F");
	 this->Single->Branch("globalPosY",&globalPosY,"globalPosY/F");
	 this->Single->Branch("globalPosZ",&globalPosZ,"globalPosZ/F");
	 this->Single->Branch("time",&temps,"time/F");
	 this->Single->Branch("energy",&energie,"energy/F");
	 this->Single->Branch("trackID",&STrackID,"trackID/I");
	 this->Single->Branch("eventID",&SeventID,"eventID/I");


	int ps = 0; //particules secondaires
	int eventCourant;
	int i = 0;
	int count = 0;
	int currentEvent = 0;
	Float_t energieTot1;
	Float_t energieTot2;
	Float_t temps1;
	Float_t temps2;

	// Loop over hits
	while ( (i < nH) )
	{
		btime->GetEntry(i);
		temps1 = time; //on ne peut pas initialiser à 0, il n'y a pas de "temps max"
		temps2 = time;

		energie1 = 0;
		energie2 = 0;
		energieTot1 = 0;
	        energieTot2 = 0;

	        x1 = 0;
		y1 = 0;
		z1 = 0;
		x2 = 0;
		y2 = 0;
		z2 = 0;

		beventID->GetEntry(i);
		currentEvent = eventID;
		do
		{
			beventID->GetEntry(i);
			btrackID->GetEntry(i);
			bX->GetEntry(i);
			bY->GetEntry(i);
			bZ->GetEntry(i);
			bedep->GetEntry(i);
			btime->GetEntry(i);
			i++;
			if(eventID == currentEvent)
			{
				if (trackID == 1 && edep > 0)
				{
					energieTot1 = energieTot1 + edep;

					x1 = x1 + X * edep;
					y1 = y1 + Y * edep;
					z1 = z1 + Z * edep;

					if (time < temps1) {temps1 = time;}
				}

				else if (trackID == 2 && edep > 0)
				{
					energieTot2 = energieTot2 + edep;

					x2 = x2 + X * edep;
					y2 = y2 + Y * edep;
					z2 = z2 + Z * edep;

					if (time < temps2) {temps2 = time;}
				}
				else {ps++;}
			}
			else {i--;} //A enlever
		}
		while ((i < nH) && (eventID == currentEvent));


		if (energieTot1 > 0 )
		{
			globalPosX = x1/energieTot1;
			globalPosY = y1/energieTot1;
			globalPosZ = z1/energieTot1;
			temps = temps1;
			energie = energieTot1;
			STrackID = 1;
			this->Single->Fill();
		}
		if (energieTot2 > 0 )
		{
			globalPosX = x2/energieTot2;
			globalPosY = y2/energieTot2;
			globalPosZ = z2/energieTot2;
			temps = temps2;
			energie = energieTot2;
			STrackID = 2;
			this->Single->Fill();
		}
	}

	TString filename = "../outputs/Single_GateHits.root";
	writeTree(this->Single,filename);

	Hits->ResetBranchAddresses();
	this->Single->ResetBranchAddresses();
	std::cout << ps << " particules secondaires pour ce run" << std:: endl;
	std::cout << "single::fillTreeSingle_Gate" << std:: endl;
}


void single::writeTree(TTree* arbreAEcrire,TString filename)
{
  TFile* target=new TFile(filename,"recreate");
  target->WriteObject(arbreAEcrire, "Singles");
  target->Close();
}

void single::blurMyBranch(TTree* tree, Double_t sigma, TString branchName)
{ //multiplication par une gaussienne de sigma donné et de moyenne la donnée dans l'arbre.
  //modifie l'arbre directement et crée un nouveau fichier

	Double_t blurredVar;
	Double_t branchVar;
	TBranch* bbranchVar;
	TString newBranchName = "blurred" + branchName;

	TBranch *toFill = tree->Branch(newBranchName,&blurredVar,"test/D");

	tree->SetBranchAddress(branchName, &branchVar, &bbranchVar);
	int nH = (int)tree->GetEntries();

	TRandom * r = new TRandom();
	for (int i = 0; i < nH; i++)
	{
		bbranchVar->GetEntry(i);
		blurredVar = r->Gaus(branchVar,sigma); //mean, variance
		std::cout << blurredVar << std:: endl;
		std::cout << i << std:: endl;
		toFill->Fill();
	}
	 std::cout <<(int)tree->GetEntries() << std::endl;

	tree->Scan();
	TString filename = "../outputs/Single_" + newBranchName + ".root";
	writeTree(tree,filename);
	tree->ResetBranchAddresses();
	std::cout << "single::blurMyBranch" << std:: endl;

}
