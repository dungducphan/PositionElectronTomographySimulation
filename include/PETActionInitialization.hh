#ifndef PETActionInitialization_h
#define PETActionInitialization_h 1

#include "PETPrimaryGeneratorAction.hh"
#include "PETEventAction.hh"
#include "PETRunAction.hh"
#include "PETSteppingAction.hh"


#include "G4VUserActionInitialization.hh"


class PETDetectorConstruction;

class PETActionInitialization : public G4VUserActionInitialization {
public:
  PETActionInitialization(PETDetectorConstruction*);

  virtual ~PETActionInitialization();

  virtual void BuildForMaster() const;
  virtual void Build() const;

private:
  PETDetectorConstruction* fDetector;
};

#endif
