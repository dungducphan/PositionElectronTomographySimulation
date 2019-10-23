#include "PETRunAction.hh"
#include "TFile.h"
#include <time.h>

PETRunAction::PETRunAction() : G4UserRunAction(), fSaveRndm(0), fAutoSeed(false) {
  G4RunManager::GetRunManager()->SetPrintProgress(1000);
}

PETRunAction::~PETRunAction() {//double eventID = analysisTree->GetEventID();
}

void PETRunAction::BeginOfRunAction(const G4Run *run) {

  //  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

  outputFilename = new TFile("TEST.root", "RECREATE");
  eventTree = new TTree("HitTree", "Hit Statistics");
  eventTree->Branch("eventHit", &eventHit, "eventHit/I");
  eventTree->Branch("energy", &energy, "energy/D");
  eventTree->Branch("wavelengthHit", &wavelengthHit, "wavelengthHit/D");
  eventTree->Branch("posx", &posx, "posx/D");
  eventTree->Branch("posy", &posy, "posy/D");
  eventTree->Branch("posz", &posz, "posz/D");
  eventTree->Branch("time1", &time1, "time/D");
  secTree = new TTree("SecondaryTree", "Event Statistics");
  secTree->Branch("sec_x", &sec_x, "sec_x/D");
  secTree->Branch("sec_y", &sec_y, "sec_y/D");
  secTree->Branch("sec_z", &sec_z, "sec_z/D");
  secTree->Branch("both_energy", &both_energy, "both_energy/D");
  gammaTree = new TTree("GammaTree", "Gamma Stats");
  gammaTree->Branch("GammaCount", &GammaCount, "GammaCount/I");
  CerenTree = new TTree("CerenTree", "CerenStats");
  CerenTree->Branch("CerenTime", &CerenTime, "CerenTime/D");

  //  eventHit = 0;

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  G4RunManager::GetRunManager()->SetRandomNumberStoreDir("random/");

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
  secTree->Write(0, TObject::kOverwrite);
  gammaTree->Write(0, TObject::kOverwrite);
  outputFilename->Write(0, TObject::kOverwrite);
  outputFilename->Close();

  // if (fSaveRndm == 1) {
  //   G4Random::showEngineStatus();
  //   G4Random::saveEngineStatus("endOfRun.rndm");
  // }
}

void PETRunAction::FillEventHitTree(G4int NumberOfHitInAnEvent, G4double EnergyOfPhoton,G4double wavelengthOfPhotonHit, G4double Pos_x, G4double Pos_y, G4double Pos_z, G4double Time) {
  eventHit = NumberOfHitInAnEvent;
  energy = EnergyOfPhoton;
  wavelengthHit = wavelengthOfPhotonHit;
  posx = Pos_x;
  posy = Pos_y;
  posz = Pos_z;
  time1 = Time;
  eventTree->Fill();
}


void PETRunAction::FillSecondaryTree(G4double Sec_x, G4double Sec_y, G4double Sec_z, G4double Sec_energy){
  sec_x = Sec_x;
  sec_y = Sec_y;
  sec_z = Sec_z;
  both_energy = Sec_energy;
  secTree->Fill();
}


void PETRunAction::FillGammaTree(G4int gammaCounter){
  GammaCount = gammaCounter;
  gammaTree->Fill();
}

void PETRunAction::FillCerenTree(G4double Time1){
}
