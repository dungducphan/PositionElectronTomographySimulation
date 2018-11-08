#include "PETPrimaryGeneratorAction.hh"

PETPrimaryGeneratorAction::PETPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
  G4int n_particle = 1;

  fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "e-");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1., 0.2, 0.));
  fParticleGun->SetParticleEnergy(100. * MeV);
}

PETPrimaryGeneratorAction::~PETPrimaryGeneratorAction() {
  delete fParticleGun;
}

void PETPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  G4double x0 = 0 * m;
  G4double y0 = 0 * m;
  G4double z0 = 0 * m;

  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  fParticleGun->GeneratePrimaryVertex(anEvent);
}
