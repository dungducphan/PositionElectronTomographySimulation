#ifndef LSPhysicsList_h
#define LSPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"

class LSPhysicsList: public G4VModularPhysicsList {
public:
  LSPhysicsList();
  virtual ~LSPhysicsList();

  virtual void SetCuts();
};

#endif
