#ifndef DEBUG_DETECTOR_HH
#define DEBUG_DETECTOR_HH

#include "MetaDetector.hh"

class DebugDetector: public MetaDetector //classe de base pour éviter de modifier directement construction.cc à chaque fois.
//On peut mimer les messengers avec des Get/set d'attributs et en créeant une classe qui hérite de celle-ci.
{
public :
	DebugDetector();
	~DebugDetector();

	virtual void Construct(G4LogicalVolume*logicWorld, G4Material* LSO);

private :
G4LogicalVolume *MD_scoringVolume;

};

#endif
