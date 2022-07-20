#include "TTree.h"
#include "TH1.h"
#include "TCanvas.h"

#ifndef drawFunction_hh
#define drawFunction_hh
void drawgraphs();
void DrawDirectory();
void showGate_dt(TString filename);
void GateScanHits(TString  filename);
TCanvas * drawCoincidences(TString filename);

TCanvas* fillDtGate_rand(TString filename);

template <class typeOfField> TCanvas *  drawFieldFromTree(TTree * tree, char field[],typeOfField variableForStorage) //nécessite de connaître le type à l'avance...
{
	//branches d'intérêt
	TBranch* b;

	// Set branch address
	tree->SetBranchAddress(field, &variableForStorage, &b);

	TH1D* h = new TH1D("h", field, 100, -400, 400);

	// Get number of hits in the TTree
	int nH = (int)tree->GetEntries();


	// Loop over hits
	for (int i = 0; i < nH; i++)
	{
		//get event i
		b->GetEntry(i);

		//fill histogram
		h->Fill(variableForStorage);
	}


	//Define canvas
	TCanvas* can = new TCanvas("can", "can", 1920, 1080);

	h->Draw();

	return can;
}
#endif
