#include "coincidences.hh"
#include <iostream>
#include "TFile.h"
#include <math.h> //pow,acos,sqrt
#include <stdlib.h> //abs

coincidences::coincidences() {};
coincidences::~coincidences() {delete Coincidences;};


void coincidences::fillTreeCoincidences (TTree * Singles, TFile* outputFile)
{
	//variables pour les coincidences
	Double_t energie, energie1, energie2, globalPosX1,globalPosX2, temps1,temps2, globalPosY1,globalPosY2, globalPosZ1, globalPosZ2, dt;
	Double_t x1, y1, z1, x2, y2, z2;

	// variables pour les singles
	Double_t globalPosX, globalPosY, globalPosZ, time, edep;
	Int_t eventID;
	Int_t trackID;
	Int_t coincidence;

	//branches d'intérêt
	TBranch* bglobalPosX;
	TBranch* bglobalPosY;
	TBranch* bglobalPosZ;
	TBranch* beventID;
	TBranch* btrackID;
	TBranch* btime;
	TBranch* bedep;
	TBranch* bcoincidence;

	// Set branch address
	Singles->SetBranchAddress("globalPosX", &globalPosX, &bglobalPosX); //mm
	Singles->SetBranchAddress("globalPosY", &globalPosY, &bglobalPosY);
	Singles->SetBranchAddress("globalPosZ", &globalPosZ, &bglobalPosZ);
	Singles->SetBranchAddress("trackID", &trackID, &btrackID);
	Singles->SetBranchAddress("eventID", &eventID, &beventID);
	Singles->SetBranchAddress("time", &time, &btime);  //secondes
	Singles->SetBranchAddress("energy", &edep, &bedep); //MeV
	Singles->SetBranchAddress("coincidence", &coincidence, &bcoincidence);

	// Get number of hits in the TTree
	int nS = (int)Singles->GetEntries();

	//initialisation

	 this->Coincidences->Branch("globalPosX1",&globalPosX1,"globalPosX/D");
	 this->Coincidences->Branch("globalPosY1",&globalPosY1,"globalPosY/D");
	 this->Coincidences->Branch("globalPosZ1",&globalPosZ1,"globalPosZ/D");
	 this->Coincidences->Branch("time1",&temps1,"time/D");
	 this->Coincidences->Branch("energy1",&energie1,"energy/D");
	 this->Coincidences->Branch("globalPosX2",&globalPosX2,"globalPosX/D");
	 this->Coincidences->Branch("globalPosY2",&globalPosY2,"globalPosY/D");
	 this->Coincidences->Branch("globalPosZ2",&globalPosZ2,"globalPosZ/D");
	 this->Coincidences->Branch("time2",&temps2,"time/D");
	 this->Coincidences->Branch("energy2",&energie2,"energy/D");

	 this->Coincidences->Branch("dt",&dt,"dt/D");

	int ps = 0; //particules secondaires
	int eventCourant;
	int i = 0;
	int j = 0;
	int c1 = 0;
	int c2 = 0;
	int t1, t2;
	int hf = 0;
	int nB2B = 0;
	int nc = 0;
	int nbc = 0;
	int nse = 0;
	int e1,e2;
	Double_t fenetre = 10000000; //10000 ns
	Double_t energieTot1;
	Double_t energieTot2;

	// Loop over singles
	while ( i < nS-1 )
	{
		j = i+1;
		btrackID->GetEntry(i);
		t1 = trackID;
		btrackID->GetEntry(j);
		t2 = trackID;


		beventID->GetEntry(i);
		e1 = eventID;
		beventID->GetEntry(j);
		e2 = eventID;

		if (e1 == e2)//les deux singles doivent être du même event
		{
			if ((t1 == 1 && t2 ==2) ) //deux singles qui se suivent = coincidence
			{
				//std::cout << "t1  & t2" << std::endl;

				btime->GetEntry(i);
				temps1 = time;
				btime->GetEntry(j);
				temps2 = time;

				dt = temps1 - temps2;
				//if (dt < fenetre)
				//{
					bglobalPosY->GetEntry(i);
					globalPosY1 = globalPosY;
					bglobalPosY->GetEntry(j);
					globalPosY2 = globalPosY;

					bglobalPosX->GetEntry(i);
					globalPosX1 = globalPosX;
					bglobalPosX->GetEntry(j);
					globalPosX2 = globalPosX;

					bglobalPosZ->GetEntry(i);
					globalPosZ1 = globalPosZ;
					bglobalPosZ->GetEntry(j);
					globalPosZ2 = globalPosZ;

					//std::cout << "distance entre les singles : " << distance(globalPosY2,globalPosY1,globalPosX2,globalPosX1,globalPosZ2,globalPosZ1)  << std::endl;

					bedep->GetEntry(i);
					energie1 = edep;
					bedep->GetEntry(j);
					energie2 = edep;

					this->Coincidences->Fill();
					nbc++;
				//}
				//else {hf++;}

			}
			else if (t1 == 2 && t2 == 1) //pareil mais dans l'autre sens. Utile?
			{

				//std::cout << "t2  & t1" << std::endl;

				btime->GetEntry(i);
				temps1 = time;
				btime->GetEntry(j);
				temps2 = time;

				dt = temps2 - temps1; //inverser ?

				//if (dt < fenetre)
				//{
					bglobalPosY->GetEntry(i);
					globalPosY2 = globalPosY;
					bglobalPosY->GetEntry(j);
					globalPosY1 = globalPosY;

					bglobalPosX->GetEntry(i);
					globalPosX2 = globalPosX;
					bglobalPosX->GetEntry(j);
					globalPosX1 = globalPosX;

					bglobalPosZ->GetEntry(i);
					globalPosZ2 = globalPosZ;
					bglobalPosZ->GetEntry(j);
					globalPosZ1 = globalPosZ;

					//std::cout << "distance entre les singles : " << distance(globalPosY2,globalPosY1,globalPosX2,globalPosX1,globalPosZ2,globalPosZ1)  << std::endl;

					bedep->GetEntry(i);
					energie2 = edep;
					bedep->GetEntry(j);
					energie1 = edep;

					this->Coincidences->Fill();
					nbc++;
				//}
				//else {hf++;}

			}
			else{nc++;//std :: cout << "t1 : " << t1 <<"t2" << t2 << std::endl;
		//std :: cout << "i : " << i <<"j" << j << std::endl;
	}
			i = i + 2;
		}
		else{ /*std :: cout << "Singles pas du même event" << std::endl;*/ nse++;i++;}

	}


	outputFile->WriteObject(this->Coincidences, "Coincidences");
	Singles->ResetBranchAddresses();
	this->Coincidences->ResetBranchAddresses();
/*
	std::cout  << "hors fenêtre : " <<hf << std::endl;
	std::cout  << "pas backToBack : " << nB2B <<  std::endl;
	std::cout  << "scatter ou random : " << nc <<   std::endl;
	std::cout  << "nS : " << nS <<  std::endl;
	std::cout  << "nombre de deux singles pas du même event : " << nse <<  std::endl;
	std::cout  << "nombre de coincidences : " << nbc <<  std::endl;
	std::cout  << "single::fillTreeCoincidences" << std:: endl;
	*/
}

void coincidences::writeTree(TTree* treeToWrite,TString filename)
{
  TFile* target= new TFile(filename,"recreate");
  target->WriteObject(treeToWrite, "Coincidences");
  target->Close();
}
Double_t coincidences::distance(Double_t x1,Double_t x2,Double_t y1,Double_t y2,Double_t z1,Double_t z2) //distance entre deux points
{
return sqrt(pow(x1-x2,2) + pow(y1-y2,2) + pow(z1-z2,2));
}
