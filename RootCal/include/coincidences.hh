#ifndef COINCIDENCES_HH
#define COINCIDENCES_HH

#include "TTree.h"
#include "TString.h"
#include "TFile.h"

class coincidences{
private:
TTree * Coincidences = new TTree("coincidences","coincidences");




public:

coincidences();
~coincidences();


TTree * getTreeCoincidences() {return Coincidences;}
void fillTreeCoincidences (TTree * Singles,TFile* outputFile);
void writeTree(TTree* treeToWrite,TString filename);
Double_t distance(Double_t x1,Double_t x2,Double_t y1,Double_t y2,Double_t z1,Double_t z2);


};
#endif
