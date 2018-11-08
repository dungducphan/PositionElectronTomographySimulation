#ifndef PETActionInitialization_h
#define PETActionInitialization_h 1

#include "PETPrimaryGeneratorAction.hh"
#include "PETEventAction.hh"
#include "PETRunAction.hh"

#include "G4VUserActionInitialization.hh"

class PETActionInitialization : public G4VUserActionInitialization {
public:
  PETActionInitialization();

  virtual ~PETActionInitialization();

  virtual void Build() const;
};

#endif
