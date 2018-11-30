//
// Created by Dung Phan on 3/13/18.
//

#ifndef PETSIM_PETTRACKERSD_HH
#define PETSIM_PETTRACKERSD_HH

#include <iostream>

#include "G4VSensitiveDetector.hh"

#include "PETTrackerHit.hh"

class G4Step;
class G4HCofThisEvent;

class PETTrackerSD : public G4VSensitiveDetector {
public:
  PETTrackerSD(const G4String);
  virtual ~PETTrackerSD();

  // methods from base class
  virtual void   Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  G4bool ProcessHits_constStep(const G4Step*, G4TouchableHistory* );

private:
  PETTrackerHitsCollection* fPhotonDetHitCollection;
};


#endif //PETSIM_PETTRACKERSD_HH
