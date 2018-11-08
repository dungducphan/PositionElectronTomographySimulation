#include "PETPhysicsList.hh"

LSPhysicsList::LSPhysicsList() : G4VModularPhysicsList() {
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmStandardPhysics());
}

LSPhysicsList::~LSPhysicsList() {
}

void LSPhysicsList::SetCuts() {
  G4VUserPhysicsList::SetCuts();
}
