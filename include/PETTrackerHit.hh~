//
// Created by Dung Phan on 3/13/18.
//

#ifndef LSSIM_LSTRACKERHIT_HH
#define LSSIM_LSTRACKERHIT_HH

#include "G4VHit.hh"
#include "G4ThreeVector.hh"
#include "G4THitsCollection.hh"

class LSTrackerHit : public G4VHit {
public:
  LSTrackerHit();
  LSTrackerHit(G4ThreeVector pArrive, G4double pTime, G4double pEnergy);
  virtual ~LSTrackerHit();

  LSTrackerHit(const LSTrackerHit &right);
  const LSTrackerHit& operator=(const LSTrackerHit& right);

  G4int operator==(const LSTrackerHit& right) const;

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

typedef G4THitsCollection<LSTrackerHit> LSTrackerHitsCollection;
extern G4ThreadLocal G4Allocator<LSTrackerHit>* LSTrackerHitAllocator;

//--------------------------------------------------
// Operator Overloads
//--------------------------------------------------

inline void* LSTrackerHit::operator new(size_t) {
  if(!LSTrackerHitAllocator)
    LSTrackerHitAllocator = new G4Allocator<LSTrackerHit>;
  return (void *) LSTrackerHitAllocator->MallocSingle();
}

inline void LSTrackerHit::operator delete(void *hit) {
  LSTrackerHitAllocator->FreeSingle((LSTrackerHit*) hit);
}

#endif //LSSIM_LSTRACKERHIT_HH
