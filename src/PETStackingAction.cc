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
  G4int EventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  G4String CPName  = aTrack->GetCreatorProcess()->GetProcessName();
  
  if(aTrack->GetParentID()>0){
    if(name=="e-"){
      if(aTrack->GetCurrentStepNumber()==1){
	G4String process = aTrack->GetCreatorProcess()->GetProcessName();
	G4double Sec_energy = aTrack->GetKineticEnergy();
	G4double Sec_x  = aTrack->GetVertexPosition().x();
	G4double Sec_y  = aTrack->GetVertexPosition().y();
	G4double Sec_z  = aTrack->GetVertexPosition().z();
	std::ofstream myfile("Sec_depth.txt", std::ios_base::app);
	myfile << Sec_x <<std::endl;
	myfile.close();
	// std::cout<<name<<"\t" <<  Sec_energy << std::endl;
	fRunAction->FillSecondaryTree(Sec_x, Sec_y, Sec_z, Sec_energy);
      }
    }
  }
  //return fUrgent;
  
  if(aTrack->GetDefinition()==G4OpticalPhoton::OpticalPhotonDefinition())
    { // particle is optical photon
      if(aTrack->GetParentID()>0)
	{ // particle is secondary
	  gammaCounter++;
	  //      return fKill;
	}
    }
  
  //return fKill;
  return fUrgent;
}

void PETStackingAction::NewStage()
{
  G4cout << "Number of gamma produces in this event : "
	 << gammaCounter << G4endl;
  std::ofstream myfile1("gammaCount.txt", std::ios_base::app);
  myfile1 << gammaCounter <<std::endl;
  myfile1.close();
  fRunAction->FillGammaTree(gammaCounter);
}

void PETStackingAction::PrepareNewEvent()
{ gammaCounter = 0; }
