//
// Created by Dung Phan on 3/13/18.
//

#include "PETTrackerHit.hh"

#include "G4VVisManager.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include <iomanip>

G4ThreadLocal G4Allocator<PETTrackerHit>* PETTrackerHitAllocator=0;

PETTrackerHit::PETTrackerHit() {
  fArrivalTime = 0.;
  fPosArrive   = G4ThreeVector(0., 0., 0.);
}

PETTrackerHit::PETTrackerHit(G4ThreeVector pArrive, G4double pTime, G4double pEnergy) {
  fPosArrive   = pArrive;
  fArrivalTime = pTime;
  fEnergy      = pEnergy;
}

PETTrackerHit::~PETTrackerHit() {
}

PETTrackerHit::PETTrackerHit(const PETTrackerHit& right) : G4VHit() {
  *this = right;
}

const PETTrackerHit& PETTrackerHit::operator=(const PETTrackerHit& right) {
  fPosArrive   = right.fPosArrive;
  fArrivalTime = right.fArrivalTime;
  fEnergy      = right.fEnergy;

  return *this;
}

G4int PETTrackerHit::operator==(const PETTrackerHit& right) const {
  return fPosArrive   == right.fPosArrive  &&
         fArrivalTime == right.fArrivalTime &&
         fEnergy      == right.fEnergy;

}
