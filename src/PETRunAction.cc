#include "PETRunAction.hh"
#include "TFile.h"

PETRunAction::PETRunAction() : G4UserRunAction(), fSaveRndm(0), fAutoSeed(false) {
  G4RunManager::GetRunManager()->SetPrintProgress(1000);
}

PETRunAction::~PETRunAction() {
}

void PETRunAction::BeginOfRunAction(const G4Run *run) {

  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

  outputFilename = new TFile("first_try.root", "RECREATE");
  eventTree = new TTree("HitTree", "Hit Statistics");
  eventTree->Branch("eventHit", &eventHit, "eventHit/I");
  eventTree->Branch("wavelengthHit", &wavelengthHit, "wavelengthHit/D");
  eventTree->Branch("posx", &posx, "posx/D");
  eventTree->Branch("posy", &posy, "posy/D");
  eventTree->Branch("posz", &posz, "posz/D");

  //eventHit = 0;

  // G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  // G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

  // if (1) {
  //   // automatic (time-based) random seeds for each run
  //   G4cout << "*******************" << G4endl;
  //   G4cout << "*** AUTOSEED ON ***" << G4endl;
  //   G4cout << "*******************" << G4endl;
  //   long seeds[2];
  //   time_t systime = time(NULL);
  //   seeds[0] = (long) systime;
  //   seeds[1] = (long) (systime*G4UniformRand());
  //   G4Random::setTheSeeds(seeds);
  //   G4Random::showEngineStatus();
  // } else {
  //   G4Random::showEngineStatus();
  // }

  // if (fSaveRndm > 0) G4Random::saveEngineStatus("BeginOfRun.rndm");

  
}

void PETRunAction::EndOfRunAction(const G4Run *run) {

  eventTree->Write(0, TObject::kOverwrite);
  outputFilename->Write(0, TObject::kOverwrite);
  outputFilename->Close();

  // if (fSaveRndm == 1) {
  //   G4Random::showEngineStatus();
  //   G4Random::saveEngineStatus("endOfRun.rndm");
  // }
}

  void PETRunAction::FillEventHitTree(G4int NumberOfHitInAnEvent, G4double wavelengthOfPhotonHit, G4double Pos_x, G4double Pos_y, G4double Pos_z) {
  eventHit = NumberOfHitInAnEvent;
  wavelengthHit = wavelengthOfPhotonHit;
  posx = Pos_x;
  posy = Pos_y;
  posz = Pos_z;
  eventTree->Fill();
}



