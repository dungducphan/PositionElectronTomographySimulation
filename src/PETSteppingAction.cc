//
// Created by Dung Phan on 3/19/18.
//

#include "G4Run.hh"
#include "G4Step.hh"
#include "G4Track.hh"
#include "G4StepPoint.hh"
#include "G4TrackStatus.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4ProcessManager.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"
#include "G4UImanager.hh"
#include "G4ThreeVector.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4TrackingManager.hh"
#include "G4Electron.hh"
#include "G4EventManager.hh"



#include "PETSteppingAction.hh"
#include "PETDetectorConstruction.hh"
#include "PETTrackerSD.hh"

#include <sstream>
#include <iostream>
#include <fstream>

PETSteppingAction::PETSteppingAction(PETDetectorConstruction * detector) : fDetector(detector) {
  fOpProcess = NULL;
}

PETSteppingAction::~PETSteppingAction() {}

void PETSteppingAction::UserSteppingAction(const G4Step * theStep) {
  G4Track* theTrack = theStep->GetTrack();

  G4int EventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();

  
  G4String CPName;
  G4double Secondary_Energy;
  G4double TotalSecondary_Energy;
  G4int gammaCounter=0;
  G4int TrackID;
  G4int ParentID;
  G4int Step_Number = theTrack->GetCurrentStepNumber();
  


  
  //  if(theTrack->GetCreatorProcess()!=0){
  if(theTrack->GetParentID()==0){
    //if(theTrack->GetCreatorProcess()->GetProcessName()=="compt"){
    //      if(Step_Number == 2){
    //CPName = theTrack->GetCreatorProcess()->GetProcessName();
    G4double Energy_Loss = theStep->GetDeltaEnergy();
    G4double Energy_Gamma = theTrack->GetTotalEnergy();
    //Secondary_Energy = theTrack->GetKineticEnergy()/keV;
    //ParentID = theTrack->GetParentID();
    G4double x  = theTrack->GetPosition().x();//GetVertexPosition().x();
    G4double y  = theTrack->GetPosition().y();//GetVertexPosition().y();
    G4double z  = theTrack->GetPosition().z();//GetVertexPosition().z();
    //if(Energy_Loss!=0){
    std::cout<<Step_Number<< "\t" <<Energy_Loss<< "\t" << Energy_Gamma <<std::endl;
    std::ofstream myfile("gamma_energy.txt", std::ios_base::app);
    myfile << Step_Number  << "," << Energy_Loss << "," << Energy_Gamma << "," << x << "," << y << "," << z <<std::endl;
    myfile.close();
    //}
    //}
  }
  //    }
  
  
  G4StepPoint* thePrePoint  = theStep->GetPreStepPoint();
  G4StepPoint* thePostPoint = theStep->GetPostStepPoint();

  G4VPhysicalVolume* thePrePV  = thePrePoint->GetPhysicalVolume();
  G4VPhysicalVolume* thePostPV = thePostPoint->GetPhysicalVolume();


  G4String thePrePVname  = " ";
  G4String thePostPVname = " ";

  if (thePostPV) {
    thePrePVname  = thePrePV->GetName();
    thePostPVname = thePostPV->GetName();
  }


  G4LogicalVolume* volume = theStep->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

  // // if (theTrack->GetParentID() == 0){
  // if (volume == sCrystal_Log){
  //     G4double edepStep = theStep->GetTotalEnergyDeposit();
  //     std::ofstream myfile("edep.txt", std::ios_base::app);
  //     myfile << edepStep << "\n";
  //     myfile.close();
  //     // }
  // }



  
  // //Recording data for start
  // if (theTrack->GetParentID()!=0) {
  //   if(theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition()) {
  //     //This is a primary track
  //     if ( theTrack->GetCurrentStepNumber() == 1 ) {
  // 	std::ofstream myfile("noclue.txt", std::ios_base::app);
  // 	G4double x  = theTrack->GetVertexPosition().x();
  // 	G4double y  = theTrack->GetVertexPosition().y();
  // 	G4double z  = theTrack->GetVertexPosition().z();
  // 	G4double pz = theTrack->GetVertexMomentumDirection().z();
  // 	G4double energy2 = theTrack->GetTotalEnergy();
  // 	G4double wavelength2 = 1242.38/ (energy2/eV);
  // 	//std::cout<<"#######################################"<< std::endl;
  // 	//std::cout<<wavelength2<<std::endl;
  // 	myfile << wavelength2 << "\t";
  // 	myfile.close();
  //     }
  //   }
  // }
  


  // Retrieve the status of the photon
  G4OpBoundaryProcessStatus theStatus = Undefined;
  G4ProcessManager* OpManager = G4OpticalPhoton::OpticalPhoton()->GetProcessManager();

  if (OpManager) {
    G4int MAXofPostStepLoops = OpManager->GetPostStepProcessVector()->entries();
    G4ProcessVector* fPostStepDoItVector = OpManager->GetPostStepProcessVector(typeDoIt);

    for ( G4int i=0; i<MAXofPostStepLoops; i++) {
      G4VProcess* fCurrentProcess = (*fPostStepDoItVector)[i];
      fOpProcess = dynamic_cast<G4OpBoundaryProcess*>(fCurrentProcess);
      if (fOpProcess) { theStatus = fOpProcess->GetStatus(); break;}
    }
  }

  switch (theStatus) {
    // Detected by a detector
    case Detection:
      // Check if the photon hits the detector and process the hit if it does
      if ( thePostPVname == "DetectorPhy" ) {
        //std::cout << "Photon is detected." << std::endl;
        G4SDManager* SDman = G4SDManager::GetSDMpointer();
        G4String SDname="TrackerChamberSD";
        PETTrackerSD* mppcSD = (PETTrackerSD*)SDman->FindSensitiveDetector(SDname);
        if (mppcSD) mppcSD->ProcessHits_constStep(theStep,NULL);
        // Stop Tracking when it hits the detector's surface
        theTrack->SetTrackStatus(fStopAndKill);
        return;
      }
      break;

    default: break;
  }

}
