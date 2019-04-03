#include "PETEventAction.hh"
#include "PETRunAction.hh"
#include "PETTrackerHit.hh"


#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"


PETEventAction::PETEventAction(PETRunAction *runAction) : G4UserEventAction(), fRunAction(runAction){
  fMPPCCollID = 0;
}

PETEventAction::~PETEventAction() {
}

void PETEventAction::BeginOfEventAction(const G4Event *event) {
}

void PETEventAction::EndOfEventAction(const G4Event *event) {

    if (fRunAction->GetRndmFreq() == 2) {
    std::ostringstream os;
    os<<"endOfEvent_"<<G4Threading::G4GetThreadId()<<".rndm";
    G4Random::saveEngineStatus(os.str().c_str());
  }

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  G4String colName = "PhotonDetHitCollection";
  fMPPCCollID = SDman->GetCollectionID(colName);



  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  PETTrackerHitsCollection* mppcHC = 0;



  // Get the hit collections
  if (HCE) {

    if (fMPPCCollID>=0) mppcHC = (PETTrackerHitsCollection*)(HCE->GetHC(fMPPCCollID));
   }
  
  std::cout<<mppcHC->entries()<<"mmpcHC entries" << std::endl;

  // Get hit information about photons that reached the detector in this event
  if (mppcHC) {
    G4int n_hit = mppcHC->entries();
    auto HitVector = mppcHC->GetVector();
    for (unsigned short int ihit = 0; ihit < n_hit; ihit++) {
      G4double energy = HitVector->at(ihit)->GetEnergy();
      G4ThreeVector photonArrive = HitVector->at(ihit)->GetArrivalPos();
      G4double wavelength = 1242.38 / (energy/eV);
      G4double xpos = photonArrive.x();
      G4double ypos = photonArrive.y();
      G4double zpos = photonArrive.z();
      fRunAction->FillEventHitTree(n_hit, energy, wavelength, xpos, ypos, zpos);
      std::cout<<"Number of Hits =  " <<n_hit<<std::endl;
    }
  }

}
