//
// Created by Dung Phan on 3/13/18.
//

#ifndef PETSIM_PETTRACKERHIT_HH
#define PETSIM_PETTRACKERHIT_HH

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"

class PETTrackerHit : public G4VHit {
public:
  PETTrackerHit();
  PETTrackerHit(G4ThreeVector pArrive, G4double pTime, G4double pEnergy);
  virtual ~PETTrackerHit();

  PETTrackerHit(const PETTrackerHit &right);
  const PETTrackerHit& operator=(const PETTrackerHit& right);

  G4int operator==(const PETTrackerHit& right) const;

  inline void* operator new(size_t);
  inline void  operator delete(void*);

  inline void SetArrivalPos(G4ThreeVector xyz) { fPosArrive = xyz; }
  inline void SetArrivalTime(G4double t) { fArrivalTime = t; }
  inline void SetEnergy(G4double e) { fEnergy = e; }

  inline G4ThreeVector GetArrivalPos() { return fPosArrive; }
  inline G4double GetArrivalTime() { return fArrivalTime; }
  inline G4double GetEnergy() { return fEnergy; }

private:

  // the arrival time of the photon
  G4double      fArrivalTime;
  // where the photon hit the detector (detector's coordinate)
  G4ThreeVector fPosArrive;
  G4double      fEnergy;
};

//--------------------------------------------------
// Type Definitions
//--------------------------------------------------

typedef G4THitsCollection<PETTrackerHit> PETTrackerHitsCollection;
extern G4ThreadLocal G4Allocator<PETTrackerHit>* PETTrackerHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* PETTrackerHit::operator new(size_t) {
  if(!PETTrackerHitAllocator)
    PETTrackerHitAllocator = new G4Allocator<PETTrackerHit>;
  return (void *) PETTrackerHitAllocator->MallocSingle();
}

inline void PETTrackerHit::operator delete(void *hit) {
  PETTrackerHitAllocator->FreeSingle((PETTrackerHit*) hit);
}

#endif //PETSIM_PETTRACKERHIT_HH
