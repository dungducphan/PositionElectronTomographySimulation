//
// Created by Dung Phan on 3/13/18.
//

#ifndef LSSIM_LSTRACKERSD_HH
#define LSSIM_LSTRACKERSD_HH

#include <iostream>

#include "G4VSensitiveDetector.hh"

#include "LSTrackerHit.hh"

class G4Step;
class G4HCofThisEvent;

class LSTrackerSD : public G4VSensitiveDetector {
public:
  LSTrackerSD(const G4String);
  virtual ~LSTrackerSD();

  // methods from base class
  virtual void   Initialize(G4HCofThisEvent*);
  virtual G4bool ProcessHits(G4Step*, G4TouchableHistory*);
  G4bool ProcessHits_constStep(const G4Step*, G4TouchableHistory* );

private:
  LSTrackerHitsCollection* fPhotonDetHitCollection;
};


#endif //LSSIM_LSTRACKERSD_HH
