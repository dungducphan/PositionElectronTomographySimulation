#ifndef PETEventAction_h
#define PETEventAction_h 1

#include "PETRunAction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UserEventAction.hh"
#include "globals.hh"

class PETRunAction;

class PETEventAction : public G4UserEventAction {
public:
  PETEventAction(PETRunAction *runAction);

  virtual ~PETEventAction();

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

private:
  PETRunAction *fRunAction;

  G4int fMPPCCollID;
};

#endif
