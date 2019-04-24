#include "PETPrimaryGeneratorAction.hh"

PETPrimaryGeneratorAction::PETPrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction(), fParticleGun(0) {
  G4int n_particle = 1;

  fParticleGun = new G4ParticleGun(n_particle);
  G4ParticleTable * particleTable = G4ParticleTable::GetParticleTable();
  G4String particleName;
  G4ParticleDefinition *particle = particleTable->FindParticle(particleName = "gamma");
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(3*0.995, 3*0.098, 0.0));
  fParticleGun->SetParticleEnergy(0.511 * MeV);
  //  fParticleGun->SetParticleTime(100);

/*  tParticleGun = new G4ParticleGun(n_particle);
  tParticleGun->SetParticleDefinition(particle);
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(-1., 0.0, 0.0));
  tParticleGun->SetParticleEnergy(0.511 * MeV); */
  //  fParticleGun->SetParticleTime(100);

}

PETPrimaryGeneratorAction::~PETPrimaryGeneratorAction() {
  delete fParticleGun;
}

void PETPrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent) {
  G4double x0 = 0 * mm;
  G4double y0 = 0 * m;
  G4double z0 = 0 * m;
  // this function is called at the begining of event

  //distribution uniform in solid angle
  //fParticleGun->SetParticlePosition(G4ThreeVector(x0, y0, z0));

  //G4double time = G4UniformRand()*100+G4UniformRand()*10;
  G4double Vector = 2*G4UniformRand() - 1., phi = CLHEP::twopi*G4UniformRand();
  G4double NoPoint = std::sqrt(1. - Vector*Vector);
  G4double ux = NoPoint*std::cos(phi);
  G4double uy = NoPoint*std::sin(phi);
  G4double uz = (2/3)*ux;//cosTheta;
  G4double time = G4UniformRand()*ns;
  //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
  fParticleGun->SetParticleTime(time);
  fParticleGun->GeneratePrimaryVertex(anEvent);

  /*tParticleGun->SetParticleMomentumDirection(G4ThreeVector(-ux,-uy,uz));
  tParticleGun->SetParticleTime(time);
  tParticleGun->GeneratePrimaryVertex(anEvent);*/

}
