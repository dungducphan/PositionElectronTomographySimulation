//
// Created by Dung Phan on 3/13/18.
//

#include "PETTrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

#include "G4SDManager.hh"
#include "G4OpticalPhoton.hh"

PETTrackerSD::PETTrackerSD(const G4String name)
    : G4VSensitiveDetector(name), fPhotonDetHitCollection(0) {
  collectionName.insert("PhotonDetHitCollection");
}

PETTrackerSD::~PETTrackerSD() {
}

void PETTrackerSD::Initialize(G4HCofThisEvent* hce) {
  // Create hits collection
  fPhotonDetHitCollection = new PETTrackerHitsCollection(SensitiveDetectorName,collectionName[0]);

  // Add this collection in hce

  G4int hcID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fPhotonDetHitCollection );
}


G4bool PETTrackerSD::ProcessHits(G4Step* aStep, G4TouchableHistory*) {
  return false;
}

G4bool PETTrackerSD::ProcessHits_constStep(const G4Step * aStep, G4TouchableHistory *) {
//// Generates a hit and uses the postStepPoint; PostStepPoint because the hit
//// is generated manually when the photon hits the detector
  if (aStep == NULL) return false;
  G4Track* theTrack = aStep->GetTrack();

  // Need to know if this is an optical photon
  if(theTrack->GetDefinition()
     != G4OpticalPhoton::OpticalPhotonDefinition()) return false;

  // Find out information regarding the hit
  G4StepPoint* thePostPoint = aStep->GetPostStepPoint();

  G4TouchableHistory* theTouchable = (G4TouchableHistory*)(thePostPoint->GetTouchable());

  G4ThreeVector photonArrive = thePostPoint -> GetPosition();
  G4double      arrivalTime  = theTrack -> GetGlobalTime();
  G4double      energy       = theTrack -> GetTotalEnergy();


  // Convert the global coordinate for arriving photons into
  // the local coordinate of the detector


  //This is the thing you just did
  //photonArrive = theTouchable->GetHistory()->
  //GetTopTransform().TransformPoint(photonArrive);

  // Creating the hit and add it to the collection
  fPhotonDetHitCollection->insert(new PETTrackerHit(photonArrive, arrivalTime, energy));

  return true;
}
