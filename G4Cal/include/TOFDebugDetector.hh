#ifndef TOFDEBUGDETECTOR_HH
#define TOFDEBUGDETECTOR_HH

#include "MetaDetector.hh"

class TOFDebugDetector : public MetaDetector //classe de base pour éviter de modifier directement construction.cc à chaque fois.
//On peut mimer les messengers avec des Get/set d'attributs et en créeant une classe qui hérite de celle-ci.
{
public :
	TOFDebugDetector();
	~TOFDebugDetector();

	virtual void Construct(G4LogicalVolume*logicWorld,G4Material* LYSO_GE);



private :
G4LogicalVolume *MD_scoringVolume;

};

#endif
