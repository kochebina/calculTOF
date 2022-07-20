#ifndef TOFDEBUGDETECTOR2_HH
#define TOFDEBUGDETECTOR2_HH

#include "MetaDetector.hh"

class TOFDebugDetector2 : public MetaDetector //classe de base pour éviter de modifier directement construction.cc à chaque fois.
//On peut mimer les messengers avec des Get/set d'attributs et en créeant une classe qui hérite de celle-ci.
{
public :
	TOFDebugDetector2();
	~TOFDebugDetector2();

	virtual void Construct(G4LogicalVolume*logicWorld,G4Material* LYSO_GE, G4Material* worldMat);


private :
G4LogicalVolume *MD_scoringVolume;

};

#endif
