#ifndef LSPrimaryGeneratorAction_h
#define LSPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class G4Box;

class LSPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
  LSPrimaryGeneratorAction();
  virtual ~LSPrimaryGeneratorAction();

  // method from the base class
  virtual void GeneratePrimaries(G4Event*);

  // method to access particle gun
  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

private:
  G4ParticleGun*  fParticleGun; // pointer a to G4 gun class
};

#endif
