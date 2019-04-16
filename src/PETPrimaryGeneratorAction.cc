#include "PETPrimaryGeneratorAction.hh"

PETPrimaryGeneratorAction::PETPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
  G4int n_particle = 1;

  fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-1., 0.0, 0.0));
  fParticleGun->SetParticleEnergy(0.511 * MeV);

}

PETPrimaryGeneratorAction::~PETPrimaryGeneratorAction() {
  delete fParticleGun;
}

void PETPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  G4double x0 = 0 * mm;
  G4double y0 = 0 * m;
  G4double z0 = 0 * m;

  //this function is called at the begining of event
  //
  //distribution uniform in solid angle
  //
  // G4double cosTheta = 2*G4UniformRand() - 1., phi = CLHEP::twopi*G4UniformRand();
  // G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  // G4double ux = sinTheta*std::cos(phi),
  //   uy = sinTheta*std::sin(phi),
  //   uz = 0;//cosTheta;
  // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));

  
  fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));
  fParticleGun->GeneratePrimaryVertex(anEvent);

  
}
