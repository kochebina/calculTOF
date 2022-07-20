#include "TTree.h"
#include "TFile.h"



class single{

/*
    typedef struct { Float_t x, y, z; } POINT;
private:

    POINT point;
	TTree* TSingle("T","test");
    TSingle.Branch("point", &point);
    TSingle.Branch("energy", &energy);
    TSingle.Branch("time", &time);

    TTree* triParID(TTree* Hits) const;

public:

Single(TTree* Hits);
~Single();


void SetPositionToFunction(Double_t x,Double_t y,Double_t z,Double_t e, (&fun)(Double_t,Double_t,Double_t,Double_t));
static void CentroidPosition(TTree &Hits) const;//static : fonction normale mais dans une classe
//static void WinnerTakeAll(Double_t x,Double_t y,Double_t z,Double_t e);
*/


private:
	TTree* Single = new TTree("Singles","Singles");
	int maVariable = 4;
	TString policy = "centroid";

void fillTreeSingle(TTree* Hits);
void fillTreeSingle_b2b_WTA(TTree* Hits, TFile* outputFile);
void fillTreeSingle_b2b_v2(TTree* Hits, TString filename);
void fillTreeSingle_positron(TTree* Hits, TString filename);
void fillTreeSingle_Gate(TTree* Hits);

public:
	single(TTree* Hits);
	~single();

TTree * getSingleTree() {return Single;}
void setMaVariable(int maValeur) {maVariable = maValeur;}
void copyPosxyz(TTree* Hits);

void createTreeSingle(TTree* Hits, TFile* outputFile);

void blurMyBranch(TTree* tree, Double_t sigma, TString branchName);

void writeTree(TTree* arbreAEcrire,TString filename);

void setPolicyToCentroid() {policy = "centroid";}
void setPolicyToWinnerTakeAll() {policy = "winnerTakeAll";}
TString getPolicy() {return policy;}


};
