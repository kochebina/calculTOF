#include "drawFunction.hh"
#include <TROOT.h>
#include <TSystem.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TFile.h>
#include <TMath.h>
#include <TF1.h>
#include <TF2.h>
#include <TF3.h>
#include <TProfile.h>
#include <TKey.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TRandom.h>
#include <TPostScript.h>
#include <TNtuple.h>
#include <TChain.h>
#include <TCut.h>
#include <TCutG.h>
#include <TEventList.h>
#include <TBenchmark.h>
#include <TSystem.h>
#include <TApplication.h>
#include <TClassTable.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TString.h>
#include <TView.h>
#include <TObject.h>
#include <TStyle.h>
#include <TFile.h>
#include <TSystemDirectory.h>
#include <TImageDump.h>
#include <TGraph2D.h>

#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <limits.h>

#include <iostream>



void drawgraphs() {
	std::cout << "drawgraphs" << std::endl;
	TFile* f = TFile::Open("outputs/Geant4_cut1mm.root");


	TTree* Hits = (TTree*)f->Get("Hits");
	//TTree* Events = (TTree*)f->Get("Event");

	// define the variable(s) of interest, type of variable must be respected
	Double_t X;
	Double_t Y;
	Double_t Z;
	Double_t time;
	Int_t eventID;
	Int_t trackID;
	Int_t stepNumber;
	Int_t parentID;

	Char_t preProcessName[40];
	Char_t postProcessName[40];

	Double_t edep;

	//variables interm�diaires et/ou finales
	Double_t x1, y1, z1;
	Double_t x2, y2, z2;
	Double_t time1;
	Double_t time2;
	Double_t deltaT;

	//branches d'int�r�t
	TBranch* bX;
	TBranch* bY;
	TBranch* bZ;
	TBranch* beventID;
	TBranch* btrackID;
	TBranch* btime;
	TBranch* bstepNumber;
	TBranch* bparentID;

	TBranch* bpreProcessName;
	TBranch* bpostProcessName;

	TBranch* bedep;


	// Set branch address
	Hits->SetBranchAddress("X", &X, &bX);
	Hits->SetBranchAddress("Y", &Y, &bY);
	Hits->SetBranchAddress("Z", &Z, &bZ);
	Hits->SetBranchAddress("EventID", &eventID, &beventID);
	Hits->SetBranchAddress("trackID", &trackID, &btrackID);
	Hits->SetBranchAddress("GlobalTime", &time, &btime);
	Hits->SetBranchAddress("stepNumber", &stepNumber, &bstepNumber);
	Hits->SetBranchAddress("parentID", &parentID, &bparentID);

	Hits->SetBranchAddress("PreProcessName", preProcessName, &bpreProcessName);
	Hits->SetBranchAddress("PostProcessName", postProcessName, &bpostProcessName);

	//Events->SetBranchAddress("fEdep", &edep, &bedep);

	TH3D* h = new TH3D("h", "positions des hits", 100, -800, 800, 100, -800, 800, 100, -800, 800);
	TH3D* hfirst = new TH3D("hfirst", "positions des premiers hits", 100, -800, 800, 100, -800, 800, 100, -800, 800);
	TH1D* ht = new TH1D("ht", "Delta t", 100, -220, 220);

	// Get number of hits in the TTree
	int nH = (int)Hits->GetEntries();

	TArrayD  *tabX = new TArrayD(nH);

	//initialisation
	x1 = 0;
	y1 = 0;
	z1 = 0;

	x2 = 0;
	y2 = 0;
	z2 = 0;

	// Loop over hits
	for (int i = 0; i < nH; i++)
	{
		//get event i
		bX->GetEntry(i);
		bY->GetEntry(i);
		bZ->GetEntry(i);
		btime->GetEntry(i);
		bparentID->GetEntry(i);
		beventID->GetEntry(i);

		btrackID->GetEntry(i);
		bstepNumber->GetEntry(i);

		bpreProcessName->GetEntry(i);
		bpostProcessName->GetEntry(i);


		if ((parentID == 0) & (trackID == 1) & (preProcessName[0] == 'T') & ((postProcessName[0] == 'c') || (postProcessName[0] == 'p') || (postProcessName[0] == 'R')))
		{
			x1 = X; y1 = Y; z1 = Z; time1 = time; // position du premier hit ?
			hfirst->Fill(x1, y1, z1);
		}

		if ((parentID == 0) & (trackID == 2) & (preProcessName[0] == 'T') & ((postProcessName[0] == 'c') || (postProcessName[0] == 'p') || (postProcessName[0] == 'R')))
		{
			x2 = X; y2 = Y; z2 = Z; time2 = time;
			hfirst->Fill(x2, y2, z2);
		}

		//fill histogram
		h->Fill(X, Y, Z); //mettre dans le bon ordre...

		deltaT = time1 - time2;
		ht->Fill(deltaT);

		tabX->AddAt(X,i);
	}


	//std::cout << *tabX << std::endl;


	TH1D* h2 = new TH1D("h2", "energie deposee par event", 100, 0, 1.2);
/*	// Get number of events in the TTree
	int nE = (int)Events->GetEntries();


	// Loop over events
	for (int i = 0; i < nE; i++)
	{
		//get event i
		bedep->GetEntry(i);
		//fill histogram
		h2->Fill(edep); //mettre dans le bon ordre...
	}

*/

	//remove the stat from upper right corner
	gStyle->SetOptStat(0);
	//remove the title
	gStyle->SetOptTitle(0);
	//define fonts sizes
	gStyle->SetPalette(1);
	gStyle->SetTextSize(0.16);
	gStyle->SetLabelSize(0.06, "x");
	gStyle->SetLabelSize(0.06, "y");
	gStyle->SetLabelSize(0.06, "z");
	gStyle->SetTitleSize(0.06, "x");
	gStyle->SetTitleSize(0.05, "y");
	gStyle->SetTitleSize(0.06, "z");
	gStyle->SetLineWidth(2);

	//Define canvas
	TCanvas* can = new TCanvas("can", "can", 1920, 1080);
	//Define paramters of the canvas
	can->SetFillColor(0);
	can->SetBorderMode(0);
	can->SetBorderSize(3);
	can->SetBottomMargin(0.14);
	can->SetLeftMargin(0.3);
	can->SetFrameBorderMode(2);
	can->SetFrameLineWidth(1);


	h->GetXaxis()->SetTitle("X");
	h->GetYaxis()->SetTitleOffset(0.7);
	h->GetYaxis()->SetTitle("Y");
	h->Draw();

	hfirst->SetMarkerStyle(2);

	hfirst->SetMarkerColor(2);
	hfirst->Draw("same");


	auto legend = new TLegend(0.01, 0.1, 0.2, 0.9);
	legend->SetHeader("position des preStepPoints"); // option "C" allows to center the header
	legend->AddEntry(h, "en rouge : position des preStepPoints verifiant la condition ");
	legend->Draw();


	//Define canvas
	TCanvas* can4 = new TCanvas("can4", "can4", 1920, 1080);
	//Define paramters of the canvas
	can4->SetFillColor(0);
	can4->SetBorderMode(0);
	can4->SetBorderSize(3);
	can4->SetBottomMargin(0.14);
	can4->SetLeftMargin(0.3);
	can4->SetFrameBorderMode(2);
	can4->SetFrameLineWidth(1);
	//can4->SaveAs("test.png");

	ht->GetXaxis()->SetTitle("dt");
	ht->Draw();

}

void DrawDirectory() {

	Int_t i = 0;
	TString ext = ".root";
	TString dirname = "outputs";
	TSystemDirectory dir(dirname, dirname);
	TList* files = dir.GetListOfFiles();
	if (files) {
		TSystemFile* file;
		TString fname;
		TIter next(files);
		while ((file = (TSystemFile*)next())) {
			fname = file->GetName();
			if (!file->IsDirectory() && fname.EndsWith(ext)) {
				TString absoluteName = dirname +'/'+ fname;
				//std::cout << fname.Length() << std::endl;
				TFile* f = TFile::Open(absoluteName);



				TTree* Hits = (TTree*)f->Get("TousSTEP");
				TTree* Events = (TTree*)f->Get("Event");

				// define the variable(s) of interest, type of variable must be respected
				Double_t X;
				Double_t Y;
				Double_t Z;
				Double_t time;
				Int_t eventID;
				Int_t trackID;
				Int_t stepNumber;
				Int_t parentID;

				Char_t preProcessName[40];
				Char_t postProcessName[40];

				Double_t edep;

				//variables interm�diaires et/ou finales
				Double_t x1, y1, z1;
				Double_t x2, y2, z2;
				Double_t time1;
				Double_t time2;
				Double_t deltaT;

				//branches d'int�r�t
				TBranch* bX;
				TBranch* bY;
				TBranch* bZ;
				TBranch* beventID;
				TBranch* btrackID;
				TBranch* btime;
				TBranch* bstepNumber;
				TBranch* bparentID;

				TBranch* bpreProcessName;
				TBranch* bpostProcessName;

				TBranch* bedep;


				// Set branch address
				Hits->SetBranchAddress("X", &X, &bX);
				Hits->SetBranchAddress("Y", &Y, &bY);
				Hits->SetBranchAddress("Z", &Z, &bZ);
				Hits->SetBranchAddress("EventID", &eventID, &beventID);
				Hits->SetBranchAddress("trackID", &trackID, &btrackID);
				Hits->SetBranchAddress("GlobalTime", &time, &btime);
				Hits->SetBranchAddress("stepNumber", &stepNumber, &bstepNumber);
				Hits->SetBranchAddress("parentID", &parentID, &bparentID);

				Hits->SetBranchAddress("PreProcessName", preProcessName, &bpreProcessName);
				Hits->SetBranchAddress("PostProcessName", postProcessName, &bpostProcessName);

				Events->SetBranchAddress("fEdep", &edep, &bedep);

				TH3D* h = new TH3D("h", "positions des hits", 100, -800, 800, 100, -800, 800, 100, -800, 800);
				TH3D* hfirst = new TH3D("hfirst", "positions des premiers hits", 100, -800, 800, 100, -800, 800, 100, -800, 800);
				TH1D* ht = new TH1D("ht", "Delta t", 100, -200, 200);

				// Get number of hits in the TTree
				int nH = (int)Hits->GetEntries();

				//initialisation
				x1 = 0;
				y1 = 0;
				z1 = 0;

				x2 = 0;
				y2 = 0;
				z2 = 0;

				// Loop over hits
				for (int i = 0; i < nH; i++)
				{
					//get event i
					bX->GetEntry(i);
					bY->GetEntry(i);
					bZ->GetEntry(i);
					btime->GetEntry(i);
					bparentID->GetEntry(i);
					beventID->GetEntry(i);

					btrackID->GetEntry(i);
					bstepNumber->GetEntry(i);

					bpreProcessName->GetEntry(i);
					bpostProcessName->GetEntry(i);


					if ((parentID == 0) & (trackID == 1) & (preProcessName[0] == 'T') & ((postProcessName[0] == 'c') || (postProcessName[0] == 'p') || (postProcessName[0] == 'R')))
					{
						x1 = X; y1 = Y; z1 = Z; time1 = time; // position du premier hit ?
						hfirst->Fill(x1, y1, z1);
					}

					if ((parentID == 0) & (trackID == 2) & (preProcessName[0] == 'T') & ((postProcessName[0] == 'c') || (postProcessName[0] == 'p') || (postProcessName[0] == 'R')))
					{
						x2 = X; y2 = Y; z2 = Z; time2 = time;
						hfirst->Fill(x2, y2, z2);
					}
					//fill histogram
					h->Fill(X, Y, Z); //mettre dans le bon ordre...

					deltaT = time1 - time2;
					ht->Fill(deltaT);
				}

				TH1D* h2 = new TH1D("h2", "energie deposee par event", 100, 0, 1.2);
				// Get number of events in the TTree
				int nE = (int)Events->GetEntries();

				// Loop over events
				for (int i = 0; i < nE; i++)
				{
					//get event i
					bedep->GetEntry(i);
					//fill histogram
					h2->Fill(edep); //mettre dans le bon ordre...
				}



				//remove the stat from upper right corner
				gStyle->SetOptStat(0);
				//remove the title
				gStyle->SetOptTitle(0);
				//define fonts sizes
				gStyle->SetPalette(1);
				gStyle->SetTextSize(0.16);
				gStyle->SetLabelSize(0.06, "x");
				gStyle->SetLabelSize(0.06, "y");
				gStyle->SetLabelSize(0.06, "z");
				gStyle->SetTitleSize(0.06, "x");
				gStyle->SetTitleSize(0.05, "y");
				gStyle->SetTitleSize(0.06, "z");
				gStyle->SetLineWidth(2);

				//Define canvas
				TString *name = new TString(fname, fname.Length() - 5);
				TCanvas* can = new TCanvas(name->Data(), name->Data(), 1920, 1080); //nom titre taille taille
				//Define paramters of the canvas
				can->SetFillColor(0);
				can->SetBorderMode(0);
				can->SetBorderSize(3);
				can->SetBottomMargin(0.14);
				can->SetLeftMargin(0.3);
				can->SetFrameBorderMode(2);
				can->SetFrameLineWidth(1);


				h->GetXaxis()->SetTitle("X");
				h->GetYaxis()->SetTitleOffset(0.7);
				h->GetYaxis()->SetTitle("Y");
				h->Draw();

				hfirst->SetMarkerStyle(2);

				hfirst->SetMarkerColor(2);
				hfirst->Draw("same");


				auto legend = new TLegend(0.01, 0.1, 0.2, 0.9);
				legend->SetHeader("position des preStepPoints"); // option "C" allows to center the header
				legend->AddEntry(h, "en rouge : position des preStepPoints verifiant la condition ");
				legend->Draw();




				//Define canvas
				TString suffixe = "_dt";
				TString canvasName = *name + suffixe;
				TCanvas* can4 = new TCanvas(canvasName, canvasName, 1920, 1080);
				//Define paramters of the canvas
				can4->SetFillColor(0);
				can4->SetBorderMode(0);
				can4->SetBorderSize(3);
				can4->SetBottomMargin(0.14);
				can4->SetLeftMargin(0.3);
				can4->SetFrameBorderMode(2);
				can4->SetFrameLineWidth(1);

				ht->GetXaxis()->SetTitle("dt");
				ht->Draw();

				TString prefixe = "graphs/";
				TString ext2 = ".png";
				TString filename = prefixe + *name + ext2;
				TImageDump* imgdump = new TImageDump(filename.Data());
				can->Paint();
				imgdump->Close();

				filename = prefixe + *name + suffixe + ext2;
				imgdump = new TImageDump(filename.Data());
				can4->Paint();
				imgdump->Close();


				i++;
				//f->Close();

			}
		}
	}
}


TCanvas * drawCoincidences(TString filename)
{
std::cout << "drawCoincidences" << std::endl;
	TFile* f = TFile::Open(filename);
	TTree* coincidences = (TTree*)f->Get("Coincidences");

	// define the variable(s) of interest, type of variable must be respected
	Double_t X1;
	Double_t Y1;
	Double_t Z1;
	Double_t X2;
	Double_t Y2;
	Double_t Z2;
	Double_t time1;
	Double_t time2;
	Int_t eventID;

	Double_t energie1;
	Double_t energie2;

	//variables interm�diaires et/ou finales
	Double_t deltaT;

	//branches d'int�r�t
	TBranch* bX1;
	TBranch* bY1;
	TBranch* bZ1;
	TBranch* bX2;
	TBranch* bY2;
	TBranch* bZ2;
	TBranch* beventID;
	TBranch* btime1;
	TBranch* btime2;

	TBranch* benergie1;
	TBranch* benergie2;


	// Set branch address
	coincidences->SetBranchAddress("globalPosX1", &X1, &bX1);
	coincidences->SetBranchAddress("globalPosY1", &Y1, &bY1);
	coincidences->SetBranchAddress("globalPosZ1", &Z1, &bZ1);
	std::cout << "salut" << std::endl;
	coincidences->SetBranchAddress("globalPosX2", &X2, &bX2);
	coincidences->SetBranchAddress("globalPosY2", &Y2, &bY2);
	coincidences->SetBranchAddress("globalPosZ2", &Z2, &bZ2);
	std::cout << "salut" << std::endl;
	coincidences->SetBranchAddress("time1", &time1, &btime1);
	coincidences->SetBranchAddress("time2", &time2, &btime2);
	coincidences->SetBranchAddress("energy1", &energie1, &benergie1);
	coincidences->SetBranchAddress("energy2", &energie2, &benergie2);


	/*
	TH3D* h = new TH3D("h", "positions des hits", 100, -800, 800, 100, -800, 800, 100, -800, 800);
	TH3D* hfirst = new TH3D("hfirst", "positions des premiers hits", 100, -800, 800, 100, -800, 800, 100, -800, 800);
	TH1D* ht = new TH1D("ht", "Delta t", 100, -220, 220);
	*/



	TGraph2D * test = new TGraph2D();
	// Get number of hits in the TTree
	int nC = (int)coincidences->GetEntries();

	//initialisation
	//remove the stat from upper right corner
	gStyle->SetOptStat(0);
	//remove the title
	gStyle->SetOptTitle(0);
	//define fonts sizes
	gStyle->SetPalette(1);
	gStyle->SetTextSize(0.16);
	gStyle->SetLabelSize(0.06, "x");
	gStyle->SetLabelSize(0.06, "y");
	gStyle->SetLabelSize(0.06, "z");
	gStyle->SetTitleSize(0.06, "x");
	gStyle->SetTitleSize(0.05, "y");
	gStyle->SetTitleSize(0.06, "z");
	gStyle->SetLineWidth(2);

	//Define canvas
	TCanvas* can = new TCanvas("can", "can", 1920, 1080);
	//Define paramters of the canvas
	can->SetFillColor(0);
	can->SetBorderMode(0);
	can->SetBorderSize(3);
	can->SetBottomMargin(0.14);
	can->SetLeftMargin(0.3);
	can->SetFrameBorderMode(2);
	can->SetFrameLineWidth(1);

	// Loop over hits
	for (int i = 0; i < nC; i++)
	{
		//get event i
		bX1->GetEntry(i);
		bY1->GetEntry(i);
		bZ1->GetEntry(i);
		bX2->GetEntry(i);
		bY2->GetEntry(i);
		bZ2->GetEntry(i);

		btime1->GetEntry(i);
		btime2->GetEntry(i);

		test->SetPoint(i,X1,Y1,Z1);
		i++;
		test->SetPoint(i,X2,Y2,Z2);

		/*
		//fill histogram
		deltaT = time1 - time2;
		ht->Fill(deltaT);
		*/
	}

	test->Draw();

	return can;
/*
	//Define canvas
	TCanvas* can4 = new TCanvas("can4", "can4", 1920, 1080);
	//Define paramters of the canvas
	can4->SetFillColor(0);
	can4->SetBorderMode(0);
	can4->SetBorderSize(3);
	can4->SetBottomMargin(0.14);
	can4->SetLeftMargin(0.3);
	can4->SetFrameBorderMode(2);
	can4->SetFrameLineWidth(1);
	//can4->SaveAs("test.png");

	ht->GetXaxis()->SetTitle("dt");
	ht->Draw();
*/

}



void showGate_dt(TString  filename)
{
	TFile* f = TFile::Open(filename);
	TTree* Coincidences;
	f->GetObject("Coincidences", Coincidences);
	Coincidences->Draw("time1-time2");
}

void GateScanHits(TString  filename)
{
	TFile* f = TFile::Open(filename);
	TTree* Hits;
	f->GetObject("Hits", Hits);

	TTree* Singles;
	f->GetObject("Singles", Singles);

	Int_t eventID;
	Int_t trackID;

	Int_t gantryID; // ?
	Int_t rsectorID; //1
	Int_t moduleID; //2
	Int_t submoduleID; //3
	Int_t crystalID; //4
	Int_t layerID;  //5

	Float_t edep;


	//branches d'int�r�t
	TBranch* beventID;
	TBranch* btrackID;

	TBranch* bgantryID;
	TBranch* brsectorID;
	TBranch* bmoduleID;
	TBranch* bsubmoduleID;
	TBranch* bcrystalID;
	TBranch* blayerID;

	TBranch* bedep;


	// Set branch address
	Hits->SetBranchAddress("eventID", &eventID, &beventID);
	Hits->SetBranchAddress("trackID", &trackID, &btrackID);

	Hits->SetBranchAddress("gantryID", &gantryID, &bgantryID);
	Hits->SetBranchAddress("rsectorID", &rsectorID, &brsectorID);
	Hits->SetBranchAddress("moduleID", &moduleID, &bmoduleID);
	Hits->SetBranchAddress("submoduleID", &submoduleID, &bsubmoduleID);
	Hits->SetBranchAddress("crystalID", &crystalID, &bcrystalID);
	Hits->SetBranchAddress("layerID", &layerID, &blayerID);

	Hits->SetBranchAddress("edep", &edep, &bedep);


	// Get number of hits in the TTree
	int nH = (int)Hits->GetEntries();
	//nH = 1000;

	//variables boucle
	int i = 0;
	int c1 = 0;
	int c2 = 0;
	int j = 0;
	int ps = 0;
	int deuxGammasCount = 0;

	bool gamma1 = false;
	bool gamma2 = false;

	bool touriste1Repere; //y a t-il un touriste dans cet event pour ce track  ?
	bool touriste2Repere;

	bool premierModule1;
	bool premierModule2;

	int currentEvent;
	int currentModule1;
	int currentModule2;




	//initialisation
	int touriste1 = 0; //nombre d'events o� le track1 parcourt plusieurs module
	int touriste2 = 0;

	// Loop over hits
	while ( (i < nH) )
	{

		//d�but event

		beventID->GetEntry(i);
		currentEvent = eventID;
		bedep->GetEntry(i);
		btrackID->GetEntry(i);

		touriste1Repere = false;
		touriste2Repere = false;

		premierModule1 = true;
		premierModule2 = true;

		//std ::cout <<"debut event : " <<  eventID << "currentEvent : " << currentEvent << std :: endl;

		if (((trackID == 1) || (trackID == 2)) && edep >0)
		{
			do
			{
				beventID->GetEntry(i);
				btrackID->GetEntry(i);
				bedep->GetEntry(i);

				if(eventID == currentEvent)
				{
					if (trackID == 1 && edep > 0)
					{
						brsectorID->GetEntry(i); // [A MODIFIER SI DEPTH CHANGE ]
						if(premierModule1) {currentModule1 = rsectorID; premierModule1 = false;} //premier module rencontr�. Doit �tre le seul dans l'event pour un track donn�. // [A MODIFIER SI DEPTH CHANGE ]
						else
						{
							//std ::cout <<"moduleID track1 : " << moduleID <<  "moduleCourant1 : " << currentModule1<< std :: endl;
							if ((rsectorID != currentModule1) && !touriste1Repere ) // [A MODIFIER SI DEPTH CHANGE ]
							{touriste1++;touriste1Repere = true;}//std ::cout <<"touriste1Repere"<< std :: endl;} //un seul touriste par event
						}
						gamma1 = true;
						c1++;
					}
					else if (trackID == 2 && edep > 0)
					{
						brsectorID->GetEntry(i); // [A MODIFIER SI DEPTH CHANGE ]

						if(premierModule2) {currentModule2 = rsectorID; premierModule2 = false;} // [A MODIFIER SI DEPTH CHANGE ]
						else
						{
							//std ::cout <<"moduleID track2 : " << moduleID <<  "moduleCourant2 : " << currentModule2<< std :: endl;
							if ((rsectorID != currentModule2) && !touriste2Repere)  // [A MODIFIER SI DEPTH CHANGE ]
							{touriste2++;touriste2Repere = true;}//std ::cout <<"touriste2Repere"<< std :: endl;}
						}

						gamma2 = true;
						c2++;
					}
					else   {ps++;}
					i++;
				}
			}
			while ((i < nH) && (eventID == currentEvent));
		}
		else   {ps++; i++;}
		//std ::cout <<"finEvent  " << "eventID : " <<  eventID << "currentEvent : " << currentEvent << std :: endl;

		if (gamma1 && gamma2) {deuxGammasCount++;}
		gamma1 = false;
		gamma2 = false;
		j++;
	}

	Hits->ResetBranchAddresses();
	Double_t nS = Singles->GetEntries();

	std::cout << ps << " hits de particules secondaires pour ce run" << std:: endl;
	std::cout << j << " nombre d'events trait�s : " <<  std:: endl;
	std::cout << deuxGammasCount << " nombre d'events ayant deux gammas" << std:: endl;
	std::cout <<  " nombre de hits dans le track 1 : " << c1 << std:: endl;
	std::cout <<  " nombre de hits dans le track 2 : " << c2 << std:: endl;
	std::cout << "nombre d'events o� le track1 parcourt plusieurs cristaux : " << touriste1 << std:: endl; // [A MODIFIER SI DEPTH CHANGE ]
	std::cout << "nombre d'events o� le track2 parcourt plusieurs cristaux : " << touriste2 << std:: endl; // [A MODIFIER SI DEPTH CHANGE ]
	std::cout << nH << " nombre de hits : " << "nombre de singles (Gate) : " << nS << std:: endl;
	printf("�cart relatif : %f \n" ,(double) ((1 - (nS -touriste1 -touriste2) / nS)) * 100);

}
TCanvas* fillDtGate_rand(TString filename)
{
  // open the file
TFile *f = TFile::Open(filename);
// get the TTree
TTree *Tree = (TTree*)f->Get("Coincidences");
//cout<< Tree->GetEntries()<<endl;
// define the variable(s) of interest, type of variable must be respected
Double_t time1;
Double_t time2;
TBranch * btime1;
TBranch * btime2;
Float_t dt;
Int_t eventID1, eventID2;
TBranch *  beventID1,* beventID2;

//Tree->SetBranchStatus("*",0);
Tree->SetBranchStatus("time1",1);
Tree->SetBranchStatus("time2",1);
Tree->SetBranchStatus("eventID1",1);
Tree->SetBranchStatus("eventID2",1);

// Set branch address
Tree->SetBranchAddress("time1",&time1,&btime1);
Tree->SetBranchAddress("time2",&time2,&btime2);
Tree->SetBranchAddress("eventID1",&eventID1,&beventID1);
Tree->SetBranchAddress("eventID2",&eventID2,&beventID2);
// Get number of events in the TTree
int n=(int)Tree->GetEntries();

// Define a histogram with 100 bins, on x from 0 to 1
 TH1F* h = new TH1F("h","h",100,-2 *pow(10,-10)  ,2 * pow(10,-10) );//* pow(10,-10));
 //TH1F* h = new TH1F("h","h",100,-10000,10000);
// Loop over events
TRandom r;
unsigned int ntot = 1;
double prob = 0.5;
for(int i=0;i<n;i++)
{
	beventID1 -> GetEntry(i);
	beventID2 -> GetEntry(i);
			if (eventID1 == eventID2)
			{
			//get event i
			btime1 -> GetEntry(i);
			btime2 -> GetEntry(i);

			dt = (time2 - time1) * (r.Binomial(ntot,prob) * 2 -1); // (a - b) * ({0;1} * 2 -1 = {-1;1})   ;
			//dt = dt * pow(10,12); // pour mettre en ps

			//fill histogram
			 h->Fill(dt);
			}
 }
	TCanvas *can = new TCanvas("can_fillDtGate_rand","can",1920,1080);
	h->Draw();
	return can;
}
/*
TCanvas * showTimeMin(TString filename)
{
  // open the file
TFile *f = TFile::Open(filename);
// get the TTree
TTree *TreeHits = (TTree*)f->Get("Hits");
TTree *TreeCo = (TTree*)f->Get("Coincidences");
//cout<< Tree->GetEntries()<<endl;
// define the variable(s) of interest, type of variable must be respected
Double_t hitGlobalTime;
Double_t time1;
Double_t time2;

TBranch * bhitGlobalTime;
TBranch * btime1;
TBranch * btime2;

Float_t dt;

Int_t eventID1, eventID2, trackID;
TBranch *beventID1,*beventID2, *btrackID;

//Tree->SetBranchStatus("*",0);
Tree->SetBranchStatus("time1",1);
Tree->SetBranchStatus("time2",1);
Tree->SetBranchStatus("eventID1",1);
Tree->SetBranchStatus("eventID2",1);

// Set branch address
Tree->SetBranchAddress("time1",&time1,&btime1);
Tree->SetBranchAddress("time2",&time2,&btime2);
Tree->SetBranchAddress("eventID1",&eventID1,&beventID1);
Tree->SetBranchAddress("eventID2",&eventID2,&beventID2);
// Get number of events in the TTree
int n=(int)Tree->GetEntries();

// Define a histogram with 100 bins, on x from 0 to 1
 TH1F* h1 = new TH1F("h1","h1",100,0,2);
 TH1F* h2 = new TH1F("h2","h2",100,0,2);

// Loop over events

for(int i=0;i<n;i++)
{
	beventID1 -> GetEntry(i);
	beventID2 -> GetEntry(i);
			if (eventID1 == eventID2)
			{
			//get event i
			btime1 -> GetEntry(i);
			btime2 -> GetEntry(i);

			dt = (time2 - time1) * (r.Binomial(ntot,prob) * 2 -1); // (a - b) * ({0;1} * 2 -1 = {-1;1})   ;
			//dt = dt * pow(10,12); // pour mettre en ps

			//fill histogram
			 h->Fill(dt);
			}
 }
	TCanvas *can = new TCanvas("can","can",1920,1080);
	h->Draw();
	return can;
}
*/
