#include "compareFunction.hh"

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
//#include <TCanvas.h>
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

#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <limits.h>

#include <iostream>

TCanvas *compareGateG4XYZ()
{
	//** Gate **//

	TFile* fGate = TFile::Open("../outputs/Gate_cut1mm.root"); //lancer l'exécutable depuis build

	TTree* HitsGate = (TTree*)fGate->Get("Hits");

	// define the variable(s) of interest, type of variable must be respected
	Float_t X;
	Float_t Y;
	Float_t Z;

	//branches d'intérêt
	TBranch* bX;
	TBranch* bY;
	TBranch* bZ;

	// Set branch address
	HitsGate->SetBranchAddress("posX", &X, &bX);
	HitsGate->SetBranchAddress("posY", &Y, &bY);
	HitsGate->SetBranchAddress("posZ", &Z, &bZ);

	TH3D* h = new TH3D("h", "positions des hits Gate", 100, -800, 800, 100, -800, 800, 100, -800, 800);

	int nHG = (int)HitsGate->GetEntries();

	//** Geant4 **//

	TFile* fGeant4 = TFile::Open("../outputs/Geant4_cut1mm.root");

	TTree* HitsG4 = (TTree*)fGeant4->Get("Hits");

	// define the variable(s) of interest, type of variable must be respected
	Double_t X4;
	Double_t Y4;
	Double_t Z4;

	//branches d'intérêt
	TBranch* bX4;
	TBranch* bY4;
	TBranch* bZ4;


	// Set branch address
	HitsG4->SetBranchAddress("posX", &X4, &bX4);
	HitsG4->SetBranchAddress("posY", &Y4, &bY4);
	HitsG4->SetBranchAddress("posZ", &Z4, &bZ4);

	TH3D* h4 = new TH3D("h4", "positions des hits Geant4", 100, -800, 800, 100, -800, 800, 100, -800, 800);


	// Get number of hits in the TTree
	int nH4 = (int)HitsG4->GetEntries();


	// Loop over hits

	for (int i = 0; i < nHG; i++)
	{
		//get event i
		bX->GetEntry(i);
		bY->GetEntry(i);
		bZ->GetEntry(i);
		//std::cout << X << std::endl;

		//fill histogram
		h->Fill(X, Y, Z);

	}

		// Loop over hits
	for (int i = 0; i < nH4; i++)
	{
		//get event i
		bX4->GetEntry(i);
		bY4->GetEntry(i);
		bZ4->GetEntry(i);


		//fill histogram
		h4->Fill(X4, Y4, Z4);

	}

	//remove the stat from upper right corner

	gStyle->SetOptStat(0);
	//remove the title
	//gStyle->SetOptTitle(0);
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
	h->GetZaxis()->SetTitle("Z");

	h->Draw();

	h4->SetMarkerColor(2);
	h4->Draw("same");


	auto legend = new TLegend(0.01, 0.1, 0.2, 0.9);
	legend->SetHeader("position des Hits"); // option "C" allows to center the header
	legend->AddEntry(h, "en rouge : position des Hits de Geant4 ");
	legend->Draw();

	return can;

}




TCanvas * compareGateG4time()
{
		//** Gate **//

	TFile* fGate = TFile::Open("../outputs/Gate_cut1mm.root"); //lancer l'exécutable depuis build

	TTree* HitsGate = (TTree*)fGate->Get("Hits");

	// define the variable(s) of interest, type of variable must be respected
	Double_t time;

	//branches d'intérêt
	TBranch* btime;

	// Set branch address
	HitsGate->SetBranchAddress("time", &time, &btime);


	TH1D* h = new TH1D("h", "time Gate (Geant4 en rouge)", 100, -0.02, 1.2);

	int nHG = (int)HitsGate->GetEntries();

	//** Geant4 **//

	TFile* fGeant4 = TFile::Open("../outputs/Geant4_cut1mm.root");

	TTree* HitsG4 = (TTree*)fGeant4->Get("Hits");

	// define the variable(s) of interest, type of variable must be respected
	Double_t timeG4;

	//branches d'intérêt
	TBranch* btimeG4;

	// Set branch address
	HitsG4->SetBranchAddress("time", &timeG4, &btimeG4);

	TH1D* h4 = new TH1D("h4", "time Geant4", 100, -0.02, 1.2);


	// Get number of hits in the TTree
	int nH4 = (int)HitsG4->GetEntries();


	// Loop over hits

	for (int i = 0; i < nHG; i++)
	{
		//get event i
		btime->GetEntry(i);


		//fill histogram
		h->Fill(time);

	}

		// Loop over hits
	for (int i = 0; i < nH4; i++)
	{
		//get event i
		btimeG4->GetEntry(i);


		//fill histogram
		h4->Fill(timeG4);

	}

	//remove the stat from upper right corner

	gStyle->SetOptStat(0);
	//remove the title
	//gStyle->SetOptTitle(0);
	//define fonts sizes
	gStyle->SetPalette(1);
	gStyle->SetTextSize(0.16);
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

/*
	h->GetXaxis()->SetTitle("X");
	h->GetYaxis()->SetTitleOffset(0.7);
	h->GetYaxis()->SetTitle("Y");
	h->GetZaxis()->SetTitle("Z");
*/
	h->Draw();

	h4->SetLineColor(2);
	h4->Draw("same");

/*
	auto legend = new TLegend(0.01, 0.1, 0.2, 0.9);
	legend->SetHeader("time des Hits"); // option "C" allows to center the header
	legend->AddEntry(h, "en rouge : time de Geant4 en ps. Gate en s");
	legend->Draw();
*/
	return can;



}


TCanvas * compareGateG4timeT0(TString rootFileGate, TString rootFileG4,TString datatype)
{
		//** Gate **//

	TFile* fGate = TFile::Open(rootFileGate); //lancer l'exécutable depuis build

	TTree* dataGate = (TTree*)fGate->Get(datatype);

	// define the variable(s) of interest, type of variable must be respected
	Double_t time;
	Double_t T0;

	//branches d'intérêt
	TBranch* btime;
	TBranch* bT0;

	// Set branch address
	dataGate->SetBranchAddress("time", &time, &btime);
	dataGate->SetBranchAddress("T0", &T0, &bT0);


  Int_t nbins, xmin, xmax;
	nbins = 100;
	xmin = 250;
	xmax = 450;

	TH1D* h = new TH1D("h", "time-T0 Gate (en ?s) & globalTime Geant4 (en rouge en ps) :" + datatype, nbins, xmin, xmax);

	int nHG = (int)dataGate->GetEntries();

	//** Geant4 **//

	TFile* fGeant4 = TFile::Open(rootFileG4);

	TTree* dataG4 = (TTree*)fGeant4->Get(datatype);

	// define the variable(s) of interest, type of variable must be respected
	Double_t timeG4;

	//branches d'intérêt
	TBranch* btimeG4;

	// Set branch address
	dataG4->SetBranchAddress("time", &timeG4, &btimeG4);

	TH1D* h4 = new TH1D("h4","time-T0 Gate & globalTime Geant4 (en rouge en ps) :" + datatype, nbins, xmin,xmax);


	// Get number of hits in the TTree
	int nH4 = (int)dataG4->GetEntries();


	// Loop over hits

	for (int i = 0; i < nHG; i++)
	{
		//get event i
		btime->GetEntry(i);
		bT0->GetEntry(i);

		//fill histogram
		//h->Fill((time - (T0 * pow(10,9)) ) * pow(10,12));// pow(10,12)); //passage en ps Singles
		h->Fill((time - T0 ) * pow(10,12));
	}

		// Loop over hits
	for (int i = 0; i < nH4; i++)
	{
		//get event i
		btimeG4->GetEntry(i);


		//fill histogram
		h4->Fill(timeG4);

	}

	//remove the stat from upper right corner

	gStyle->SetOptStat(0);
	//remove the title
	//gStyle->SetOptTitle(0);
	//define fonts sizes
	gStyle->SetPalette(1);
	gStyle->SetTextSize(0.16);
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


	h->GetXaxis()->SetTitle("time (ps)");
	//h->GetYaxis()->SetTitle(rootFileGate + " " + rootFileG4);

	h4->Draw();
	h->Draw("same");
	h4->SetLineColor(2);






	auto legend = new TLegend(0.01, 0.1, 0.2, 0.9);
	legend->SetHeader("fichiers root"); // option "C" allows to center the header
	legend->AddEntry(h,rootFileGate );
	legend->AddEntry(h4,rootFileG4 );
	legend->Draw();

	return can;
dataGate->ResetBranchAddresses();
dataG4->ResetBranchAddresses();



}




TCanvas* compareDt_rand(TString fileGate, TString fileG4)
{
  // open the file
TFile *f = TFile::Open(fileGate);
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
 TH1F* h = new TH1F("h","dt Gate vs Geant4 (en rouge) en ps",100,-200  ,200 );//* pow(10,-10));
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
			dt = dt * pow(10,12); // pour mettre en ps

			//fill histogram
			 h->Fill(dt);
			}
 }

   // open the file
TFile *f4 = TFile::Open(fileG4);
// get the TTree
TTree *Tree4 = (TTree*)f4->Get("Coincidences");

//Tree->SetBranchStatus("*",0);
Tree4->SetBranchStatus("time1",1);
Tree4->SetBranchStatus("time2",1);

Double_t G4_time1;
Double_t G4_time2;
TBranch * G4_btime1;
TBranch * G4_btime2;
Float_t G4_dt;

// Set branch address
Tree4->SetBranchAddress("time1",&G4_time1,&G4_btime1);
Tree4->SetBranchAddress("time2",&G4_time2,&G4_btime2);
// Get number of events in the TTree
int n4=(int)Tree4->GetEntries();

// Define a histogram with 100 bins, on x from 0 to 1
 TH1F* h4 = new TH1F("h4","dt Gate vs Geant4 (en rouge) en ps",100,-200 ,200 );
// Loop over events

for(int j=0;j<n4;j++)
{

	//get event i
	G4_btime1 -> GetEntry(j);
	G4_btime2 -> GetEntry(j);

	G4_dt = (G4_time1 - G4_time2);
	//fill histogram
	 h4->Fill(G4_dt);

 }



	TCanvas *can = new TCanvas("can_fillDtGate_rand","can",1920,1080);
	h->GetXaxis()->SetTitle("time (ps)");
	h4->GetXaxis()->SetTitle("time (ps)");

	h4->SetLineColor(2);
	h4->Draw();
	h->Draw("same");
	return can;
}

TCanvas * compareTwoTrees(TString champ, TString fileGate, TString fileG4, TString datatype = "Hits")
{

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

	can->Divide(2,2);

	//** Gate **//
	TFile* fGate = TFile::Open(fileGate); //lancer l'exécutable depuis build
	TTree* Hits = (TTree*)fGate->Get(datatype);
	can->cd(1);
	Hits->Draw(champ);

	can->cd(2);
	Hits->Draw(champ);


	//** Geant4 **//
	TFile* fGeant4 = TFile::Open(fileG4);
        Hits = (TTree*)fGeant4->Get(datatype);
        Hits->SetLineColor(2);

	Hits->Draw(champ,"","same");

	can->cd(3);
	Hits->Draw(champ);



	return can;
}


TCanvas * compareTwoTrees(TString champ1,TString champ2, TString file1, TString file2, TString datatype)
{
//open file1 and file2 to get datatype (namely hits or singles) of tree1 and tree2 and compare champ1 of tree1 against champ2 of tree2

	//Define canvas
	TCanvas* can = new TCanvas("can_compareTwoTrees", "can", 1920, 1080);
	//Define paramters of the canvas
	can->SetFillColor(0);
	can->SetBorderMode(0);
	can->SetBorderSize(3);
	can->SetBottomMargin(0.14);
	can->SetLeftMargin(0.3);
	can->SetFrameBorderMode(2);
	can->SetFrameLineWidth(1);

	can->Divide(2,2);

	//** Gate **//
	TFile* f1 = TFile::Open(file1); //lancer l'exécutable depuis build
	TTree* tree1 = (TTree*)f1->Get(datatype);
	can->cd(1);
	tree1->Draw(champ1);

	can->cd(2);
	tree1->Draw(champ1);


	//** Geant4 **//
	TFile* f2 = TFile::Open(file2);
        TTree* tree2 = (TTree*)f2->Get(datatype);
        tree2->SetLineColor(2);

	tree2->Draw(champ2,"","same");

	can->cd(3);
	tree2->Draw(champ2);



	return can;
}

TCanvas * compareTwoTrees(TString champ1,TString champ2, TString file1, TString file2, TString datatype, Double_t xmin,Double_t xmax, Int_t nBins)
{
		//** Gate **//
	TFile* f1= TFile::Open(file1); //lancer l'exécutable depuis build
	TTree* dataf1 = (TTree*)f1->Get(datatype);

	TFile* f2= TFile::Open(file2); //lancer l'exécutable depuis build
	TTree* dataf2 = (TTree*)f2->Get(datatype);

	// define the variable(s) of interest, type of variable must be respected
	Double_t data1;
	Float_t data2;

	//branches d'intérêt
	TBranch* bdata1;
	TBranch* bdata2;

	// Set branch address
	dataf1->SetBranchAddress(champ1, &data1, &bdata1);
	dataf2->SetBranchAddress(champ2, &data2, &bdata2);

	TCanvas* can = new TCanvas("can_compareTwoTrees", "can", 1920, 1080);
	can->SetFillColor(0);
	can->SetBorderMode(0);
	can->SetBorderSize(3);
	can->SetBottomMargin(0.14);
	can->SetLeftMargin(0.3);
	can->SetFrameBorderMode(2);
	can->SetFrameLineWidth(1);


	TH1D* h1 = new TH1D("h1", champ1 + " vs " + champ2 +" : "+ datatype, nBins, xmin, xmax);
	TH1D* h2 = new TH1D("h2", champ1 + " vs " + champ2 +" : "+ datatype, nBins, xmin, xmax);

	int n1 = (int)dataf1->GetEntries();
	int n2 = (int)dataf2->GetEntries();
	// Loop over hits
	for (int i = 0; i < n1; i++)
	{
		//get event i
		bdata1->GetEntry(i);
		h1->Fill(data1);
	}

	// Loop over hits
	for (int i = 0; i < n2; i++)
	{
		//get event i
		bdata2->GetEntry(i);
		h2->Fill(data2);
	}


	h1->GetXaxis()->SetTitle(champ1 + " " + champ2 );
	h2->SetLineColor(2);

	can->Divide(2,2);
	can->cd(1);
	h1->Draw();

	can->cd(2);
	h1->Draw();
	h2->Draw("same");

	can->cd(3);
	h2->Draw();
	can->cd(4);



	auto legend = new TLegend(0.5,0.7);
	legend->SetHeader("fichiers root"); // option "C" allows to center the header
	legend->AddEntry(h1,file1);
	legend->AddEntry(h2,file2);
	legend->Draw();



	return can;
	dataf1->ResetBranchAddresses();
	dataf2->ResetBranchAddresses();



}
 // TCanvas* compareTime_T0(TString rootFileG4, TString rootFileGate)
 // {
	//  //Define canvas
 // 	TCanvas* can = new TCanvas("can", "can", 1920, 1080);
 // 	//Define paramters of the canvas
 // 	can->SetFillColor(0);
 // 	can->SetBorderMode(0);
 // 	can->SetBorderSize(3);
 // 	can->SetBottomMargin(0.14);
 // 	can->SetLeftMargin(0.3);
 // 	can->SetFrameBorderMode(2);
 // 	can->SetFrameLineWidth(1);
 //
 // 	can->Divide(2,2);
 //
 // 	//** Gate **//
 // 	TFile* Gate = TFile::Open(rootFileGate); //lancer l'exécutable depuis build
 // 	TTree* HitsG = (TTree*)f1->Get(Hits);
	// TTree* SinglesG = (TTree*)f1->Get(Singles);
 // 	can->cd(1);
 //
	// // define the variable(s) of interest, type of variable must be respected
	// Double_t timeH;
	// Double_t T0H;
 //
	// //branches d'intérêt
	// TBranch* btimeH;
	// TBranch* bT0H;
 //
	// // Set branch address
	// HitsGate->SetBranchAddress("time", &time, &btime);
	// HitsGate->SetBranchAddress("T0", &T0, &bT0);
 //
 //
	// int nHG = (int)HitsG->GetEntries();
	// // Loop over hits
 //
	// for (int i = 0; i < nHG; i++)
	// {
	// 	//get event i
	// 	btime->GetEntry(i);
	// 	bT0->GetEntry(i);
 //
	// 	//fill histogram
	// 	h->Fill(timeH - T0H);
	// }
 //
 //
 //
 // 	Hits->Draw("time-T0");
 //
 // 	can->cd(2);
 // 	Singles->Draw("time-T0");
 //
 //
 // 	//** Geant4 **//
 // 	TFile* f2 = TFile::Open(rootFileG4);
 //         TTree* tree2 = (TTree*)f2->Get(datatype);
 //         tree2->SetLineColor(2);
 //
 // 	tree2->Draw(champ2,"","same");
 //
 // 	can->cd(3);
 // 	tree2->Draw(champ2);
 //
 //
 //
 // 	return can;
 //
 //
 //
 //
 // }
