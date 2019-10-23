#ifndef PETPhysicsList_h
#define PETPhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"

class G4Cerenkov;

class G4Scintillation;
class G4OpBoundaryProcess;

class PETPhysicsList : public G4VUserPhysicsList {
public:
  PETPhysicsList();

  virtual ~PETPhysicsList();

  virtual void ConstructParticle();

  virtual void ConstructProcess();

  virtual void ConstructOp();

  virtual void ConstructDecay();

  virtual void ConstructEM();

  virtual void SetCuts();

private:
  static G4ThreadLocal G4int fVerboseLevel;
  static G4ThreadLocal G4int fMaxNumPhotonStep;

  static G4ThreadLocal G4Cerenkov *fCerenkovProcess;
  static G4ThreadLocal G4Scintillation *fScintillationProcess;
  static G4ThreadLocal G4OpBoundaryProcess *fBoundaryProcess;
  static G4ThreadLocal G4OpAbsorption *fAbsorptionProcess;
  static G4ThreadLocal G4OpRayleigh *fRayleighScatteringProcess;
  static G4ThreadLocal G4OpMieHG *fMieHGScatteringProcess;
};

#endif
