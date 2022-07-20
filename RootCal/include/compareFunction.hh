#include <TCanvas.h>


#ifndef compareFunction_hh
#define compareFunction_hh
TCanvas * compareGateG4XYZ();
TCanvas * compareGateG4time();
TCanvas * compareGateG4timeT0(TString rootFileGate, TString rootFileG4,TString datatype);
TCanvas * compareTwoTrees(TString champ, TString fileGate, TString fileG4,TString datatype);
TCanvas * compareTwoTrees(TString champ1,TString champ2, TString file1, TString file2, TString datatype);
TCanvas * compareTwoTrees(TString champ1,TString champ2, TString file1, TString file2, TString datatype, Double_t xmin,Double_t xmax, Int_t nBins);
TCanvas * compareDt_rand(TString fileGate, TString fileG4);

#endif
