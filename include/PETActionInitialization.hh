#ifndef LSActionInitialization_h
#define LSActionInitialization_h 1

#include "PETPrimaryGeneratorAction.hh"
#include "PETEventAction.hh"
#include "PETRunAction.hh"

#include "G4VUserActionInitialization.hh"

class LSActionInitialization : public G4VUserActionInitialization {
public:
  LSActionInitialization();
  virtual ~LSActionInitialization();

  virtual void Build() const;
};

#endif
