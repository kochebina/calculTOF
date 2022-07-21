#include "action.hh"

MyActionInitialization::MyActionInitialization(): G4VUserActionInitialization()
{
G4cout<< "MyActionInitialization" << G4endl;
}

MyActionInitialization::~MyActionInitialization()
{
	G4cout<< "~MyActionInitialization" << G4endl;
}

void MyActionInitialization::Build() const
{
	MyPrimaryGenerator *generator = new MyPrimaryGenerator();
	SetUserAction(generator);

	MyRunAction *runAction = new MyRunAction();
	SetUserAction(runAction);

	MyEventAction *eventAction = new MyEventAction(runAction);
	SetUserAction(eventAction);

	MySteppingAction *steppingAction = new MySteppingAction(eventAction);
	SetUserAction(steppingAction);

	G4cout<< "MyActionInitialization::Build" << G4endl;
}
void MyActionInitialization::BuildForMaster() const
{
	MyRunAction *runAction = new MyRunAction();
	SetUserAction(runAction);
}
