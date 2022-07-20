#ifndef ACTION_HH
#define ACTION_HH

#include "G4VUserActionInitialization.hh"

#include "BackToBackGenerator.hh"
#include "generator.hh"
#include "run.hh"
#include "event.hh"
#include "stepping.hh"

class MyActionInitialization : public G4VUserActionInitialization
{
public :
	MyActionInitialization();
	virtual ~MyActionInitialization();

	virtual void Build() const;
	virtual void BuildForMaster() const;
};


#endif
