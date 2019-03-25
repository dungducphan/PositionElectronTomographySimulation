#include "PETActionInitialization.hh"
#include "PETDetectorConstruction.hh"

#include "PETPrimaryGeneratorAction.hh"
#include "PETRunAction.hh"
#include "PETStackingAction.hh"
#include "PETEventAction.hh"
#include "PETSteppingAction.hh"



PETActionInitialization::PETActionInitialization(PETDetectorConstruction* det)
  : G4VUserActionInitialization(),
    fDetector(det) {
}

PETActionInitialization::~PETActionInitialization() {
}

void PETActionInitialization::BuildForMaster() const{
  SetUserAction(new PETRunAction());
}

void PETActionInitialization::Build() const {
  SetUserAction(new PETPrimaryGeneratorAction());

  PETRunAction* runAction = new PETRunAction();
  SetUserAction(runAction);

  PETEventAction* eventAction = new PETEventAction(runAction);
  SetUserAction(eventAction);

  SetUserAction(new PETStackingAction(runAction));

  SetUserAction(new PETSteppingAction(fDetector));
}
