#include "myHit.hh"

#include <iostream>
#include <cstdarg> //fonctions variadiques

#include "g4analysis.hh"

G4ThreadLocal G4Allocator<myHit>* myHitAllocator = 0;

myHit::myHit()
{
//G4cout << "myHit::myHit" << G4endl;
}
myHit::~myHit()
{
//G4cout << "myHit::~myHit" << G4endl;
}


void myHit::fillHitInfo(G4int myhitID,G4int myhitNumberTrack1, G4int myhitNumberTrack2, G4int myhitTrackID,G4int myhitEventID,G4int myhitParentID,
	G4ThreeVector myhitPos,
	G4double myhitEdep, G4double myhitTime,
	G4int myhitLayerID,
	G4int myhitCrystalID,
	G4int myhitSubmoduleID,
	G4int myhitModuleID,
	G4int myhitRsectorID,
	G4String myhitParticleName)


{
hitID = myhitID;
hitNumberTrack1 = myhitNumberTrack1;
hitNumberTrack2 = hitNumberTrack2;
hitTrackID = myhitTrackID;
hitEventID = myhitEventID;
hitParentID = myhitParentID;

hitPos = myhitPos;

hitEdep = myhitEdep;
hitTime = myhitTime;

hitLayerID = myhitLayerID;
hitCrystalID = myhitCrystalID;
hitSubmoduleID = myhitSubmoduleID;
hitModuleID = myhitModuleID;
hitRsectorID = myhitRsectorID;

hitParticleName = myhitParticleName;

 //G4cout <<"myHit::fillHitInfo"<< G4endl;
}

void myHit::Print()
{

  G4cout << "test" << G4endl;

}

void myHit::FillNTuple(char listOfTypes[],int NTupleID ,...) //variadic function, see https://en.cppreference.com/w/cpp/utility/variadic for more
{
		G4AnalysisManager* man = G4AnalysisManager::Instance();
		int i =0;

		man->FillNtupleIColumn(NTupleID, i, this->gethitID());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitNumberTrack1());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitNumberTrack2());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitTrackID());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitEventID());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitParentID());
		i++;
		man->FillNtupleDColumn(NTupleID, i, this->gethitPos()[0]);
		i++;
		man->FillNtupleDColumn(NTupleID, i, this->gethitPos()[1]);
		i++;
		man->FillNtupleDColumn(NTupleID, i, this->gethitPos()[2]);
		i++;
		man->FillNtupleDColumn(NTupleID, i, this->gethitTime());
		i++;
		man->FillNtupleDColumn(NTupleID, i, this->gethitEdep());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitLayerID());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitCrystalID());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitSubmoduleID());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitModuleID());
		i++;
		man->FillNtupleIColumn(NTupleID, i, this->gethitRsectorID());
		i++;
		man->FillNtupleSColumn(NTupleID, i, this->gethitParticleName());
		i++;

		int j=0;
		int vali;
		double vald;
		G4String vals;

		if (listOfTypes[j] != '\0')
		{
			va_list args;  // 	holds the information needed by va_start, va_arg, va_end, and va_copy
			va_start(args, listOfTypes);  //enables access to variadic function arguments
			char currentType;
			currentType = listOfTypes[j];
			while(currentType != '\0')
			{
				switch (currentType)
				{
					case 'i' :
					vali = va_arg(args, int);
					man->FillNtupleIColumn(NTupleID, i, vali);
						break;
					case 'd' :
					vald = va_arg(args, double);
					man->FillNtupleDColumn(NTupleID, i, vald);
						break;
					case 's' :
					vals = va_arg(args, G4String);
					man->FillNtupleSColumn(NTupleID, i, vals);
						break;
				}
				 i++;  //compteur de NTuple
				 j++;  //compteur de type
				 currentType = listOfTypes[j];
			 }
			 va_end(args); //ends traversal of the variadic function arguments
	 	}
		man->AddNtupleRow(NTupleID);

}
