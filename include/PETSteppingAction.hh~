//
// Created by Dung Phan on 3/19/18.
//

#ifndef LSSIM_LSSTEPPINGACTION_HH
#define LSSIM_LSSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"

class G4Track;
class G4StepPoint;
class G4OpBoundaryProcess;

class LSDetectorConstruction;

class LSSteppingAction : public G4UserSteppingAction {
public:
  LSSteppingAction(LSDetectorConstruction*);
  virtual ~LSSteppingAction();

  virtual void UserSteppingAction(const G4Step*);

private:
  G4OpBoundaryProcess* fOpProcess;
  LSDetectorConstruction* fDetector;
};


#endif //LSSIM_LSSTEPPINGACTION_HH
