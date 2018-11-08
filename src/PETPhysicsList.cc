#include "PETPhysicsList.hh"

#include "G4ProcessManager.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4Decay.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"


#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

G4ThreadLocal G4int PETPhysicsList::fVerboseLevel = 0;
G4ThreadLocal G4int PETPhysicsList::fMaxNumPhotonStep = 1;
G4ThreadLocal G4Cerenkov *PETPhysicsList::fCerenkovProcess = 0;
G4ThreadLocal G4OpBoundaryProcess *PETPhysicsList::fBoundaryProcess = 0;

PETPhysicsList::PETPhysicsList() : G4VUserPhysicsList() {
  SetVerboseLevel(1);
}

PETPhysicsList::~PETPhysicsList() {
}

void PETPhysicsList::ConstructParticle() {
  G4BosonConstructor bConstructor;
  bConstructor.ConstructParticle();

  G4LeptonConstructor lConstructor;
  lConstructor.ConstructParticle();

  G4MesonConstructor mConstructor;
  mConstructor.ConstructParticle();

  G4BaryonConstructor rConstructor;
  rConstructor.ConstructParticle();

  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle();
}

void PETPhysicsList::ConstructProcess() {
  AddTransportation();
  ConstructOp();
  ConstructDecay();
  ConstructEM();
}

void PETPhysicsList::ConstructOp() {
  fCerenkovProcess = new G4Cerenkov("Cerenkov");
  fCerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
  fCerenkovProcess->SetMaxBetaChangePerStep(1.0);
  fCerenkovProcess->SetTrackSecondariesFirst(true);

  fBoundaryProcess = new G4OpBoundaryProcess();

  fCerenkovProcess->SetVerboseLevel(fVerboseLevel);
  fBoundaryProcess->SetVerboseLevel(fVerboseLevel);

  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)()) {
    G4ParticleDefinition *particle = particleIterator->value();
    G4ProcessManager *pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (fCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(fCerenkovProcess);
      pmanager->SetProcessOrdering(fCerenkovProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(fBoundaryProcess);
    }
  }
}

void PETPhysicsList::ConstructEM() {
  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)()) {
    G4ParticleDefinition *particle = particleIterator->value();
    G4ProcessManager *pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
      // Construct processes for gamma
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());

    } else if (particleName == "e-") {
      // Construct processes for electron
      pmanager->AddProcess(new G4eMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(), -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(), -1, 3, 3);

    } else if (particleName == "e+") {
      // Construct processes for positron
      pmanager->AddProcess(new G4eMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(), -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(), -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation(), 0, -1, 4);

    } else if (particleName == "mu+" ||
               particleName == "mu-") {
      // Construct processes for muon
      pmanager->AddProcess(new G4MuMultipleScattering(), -1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation(), -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung(), -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction(), -1, 4, 4);

    } else {
      if ((particle->GetPDGCharge() != 0.0) &&
          (particle->GetParticleName() != "chargedgeantino") &&
          !particle->IsShortLived()) {
        // all others charged particles except geantino
        pmanager->AddProcess(new G4hMultipleScattering(), -1, 1, 1);
        pmanager->AddProcess(new G4hIonisation(), -1, 2, 2);
      }
    }
  }
}

void PETPhysicsList::ConstructDecay() {
  // Add Decay Process
  G4Decay *theDecayProcess = new G4Decay();
  auto particleIterator = GetParticleIterator();
  particleIterator->reset();
  while ((*particleIterator)()) {
    G4ParticleDefinition *particle = particleIterator->value();
    G4ProcessManager *pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}

void PETPhysicsList::SetCuts() {
  SetCutsWithDefault();
  if (verboseLevel > 0) DumpCutValuesTable();
}
