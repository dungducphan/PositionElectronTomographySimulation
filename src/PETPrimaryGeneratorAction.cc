#include "PETPrimaryGeneratorAction.hh"

PETPrimaryGeneratorAction::PETPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
  G4int n_particle = 1;

  fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.9, 0.1, 0.0));
  fParticleGun->SetParticleEnergy(0.511 * MeV);

  // tParticleGun = new G4ParticleGun(n_particle);
  // G4ParticleTable * tparticleTable = G4ParticleTable::GetParticleTable();
  // G4String tparticleName;
  // G4ParticleDefinition *tparticle = tparticleTable->FindParticle(tparticleName = "gamma");
  // tParticleGun->SetParticleDefinition(tparticle);
  // tParticleGun->SetParticleMomentumDirection(G4ThreeVector(-0.25, 0.75, 0.0));
  // tParticleGun->SetParticleEnergy(0.511 * MeV);
  

}

PETPrimaryGeneratorAction::~PETPrimaryGeneratorAction() {
  delete fParticleGun;
  delete tParticleGun;
}

void PETPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  G4double x0 = 0 * m;
  G4double y0 = 0 * m;
  G4double z0 = 0 * m;

  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  fParticleGun->GeneratePrimaryVertex(anEvent);

  
  // tParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  // tParticleGun->GeneratePrimaryVertex(anEvent);

  
}
