#include "PETStackingAction.hh"
#include "PETRunAction.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"


PETStackingAction::PETStackingAction(PETRunAction *runAction)
  : fRunAction(runAction), gammaCounter(0)
{;}

PETStackingAction::~PETStackingAction()
{;}

G4ClassificationOfNewTrack
PETStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{


  if(aTrack->GetParentID()==0)return fUrgent;
  


  G4String name = aTrack->GetDefinition()->GetParticleName();

    
  if(aTrack->GetParentID()>0){
    if(name=="e-"){
      if(aTrack->GetCurrentStepNumber()==1){
	G4String process = aTrack->GetCreatorProcess()->GetProcessName();
	G4double Sec_energy = aTrack->GetKineticEnergy();
	G4double Sec_x  = aTrack->GetVertexPosition().x();
	G4double Sec_y  = aTrack->GetVertexPosition().y();
	G4double Sec_z  = aTrack->GetVertexPosition().z();
	//	std::cout<<name<<"\t" <<  Sec_energy << std::endl;
	fRunAction->FillSecondaryTree(Sec_x, Sec_y, Sec_z, Sec_energy);
      }
    }
  }
  //return fUrgent;

  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition())
  { // particle is optical photon
    //    std::cout<<"Um status of photon" << aTrack->GetTrackStatus()<<std::endl;
    //    return fKill;
    if(aTrack->GetParentID()>0)
    { // particle is secondary
      gammaCounter++;
    }
  }
  //return fKill;
  return fUrgent;
}

void PETStackingAction::NewStage()
{
  G4cout << "Number of gamma produces in this event : "
	 << gammaCounter << G4endl;
  fRunAction->FillGammaTree(gammaCounter);
  //std::ofstream myfile("10mmgammaCounter.txt", std::ios_base::app);
  //myfile << gammaCounter << std::endl;
  //myfile.close();
}

void PETStackingAction::PrepareNewEvent()
{ gammaCounter = 0; }
