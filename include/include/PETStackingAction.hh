#ifndef PETStackingaction_H
#define PETStackingAction_H 1

#include "G4UserStackingAction.hh"
#include "G4RunManager.hh"
#include "PETRunAction.hh"


class PETRunAction;

class PETStackingAction : public G4UserStackingAction
{
  public:
    PETStackingAction(PETRunAction *runAction);
    virtual ~PETStackingAction();

  public:
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();

  private:
  PETRunAction *fRunAction;

  G4int gammaCounter;
  G4int CerenCounter;

  G4int EventID;

};

#endif
