#include "PETStackingAction.hh"
#include "PETRunAction.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"


PETStackingAction::PETStackingAction(PETRunAction *runAction)
  : gammaCounter(0), fRunAction(runAction)
{;}

PETStackingAction::~PETStackingAction()
{;}

G4ClassificationOfNewTrack
PETStackingAction::ClassifyNewTrack(const G4Track * aTrack)
{


  if(aTrack->GetParentID()==0) return fUrgent;

  G4String name = aTrack->GetDefinition()->GetParticleName();
  if(aTrack->GetTrackID()>0){
    if(name == "e-"){
    //if(aTrack->GetCreatorProcess()->GetProcessName()=="compt"){
      //if(aTrack->GetCurrentStepNumber()==1){
	  G4double Sec_energy = aTrack->GetKineticEnergy();
	  G4double Sec_x  = aTrack->GetVertexPosition().x();
	  G4double Sec_y  = aTrack->GetVertexPosition().y();
	  G4double Sec_z  = aTrack->GetVertexPosition().z();
	  std::cout<<Sec_energy<<std::endl;
	  fRunAction->Gamma(Sec_x, Sec_y, Sec_z, Sec_energy);
	  // std::ofstream myfile("compt_energy.txt", std::ios_base::app);
	  // myfile <<secondary_energy << std::endl;
	  // myfile.close();
	  // std::cout<<name<<" "<<secondary_energy<<std::endl;
	  // }
	  //}
	  return fUrgent;
    }
  }

  //   G4double secondary_energy = aTrack->GetKineticEnergy();
  //   G4String name = aTrack->GetDefinition()->GetParticleName();
  //   if(name == "e-"){
  //   }
  // }

  
  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition())
  { // particle is optical photon
    if(aTrack->GetParentID()>0)
    { // particle is secondary
      gammaCounter++;
      //std::cout<<gammaCounter<<std::endl;
    }
  }
  //fRunAction->Gamma(gammaCounter);
  return fUrgent;
}

void PETStackingAction::NewStage()
{
  G4cout << "Number of gamma produces in this event : "
         << gammaCounter << G4endl;
  //fRunAction->Gamma(gammaCounter);

}

void PETStackingAction::PrepareNewEvent()
{ gammaCounter = 0; }
