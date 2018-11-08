#include "PETActionInitialization.hh"

PETActionInitialization::PETActionInitialization() : G4VUserActionInitialization() {
}

PETActionInitialization::~PETActionInitialization() {
}

void PETActionInitialization::Build() const {
  SetUserAction(new PETPrimaryGeneratorAction);

  PETRunAction *runAction = new PETRunAction();
  SetUserAction(runAction);

  PETEventAction *eventAction = new PETEventAction(runAction);
  SetUserAction(eventAction);
}
