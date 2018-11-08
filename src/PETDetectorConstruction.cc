#include "PETDetectorConstruction.hh"
#include "PETDetectorMessenger.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

PETDetectorConstruction::PETDetectorConstruction()
: G4VUserDetectorConstruction(),
  world_hx(5.0 * m),
  world_hy(5.0 * m),
  world_hz(5.0 * m),
  fMessenger(NULL),
  fOpeningAngle(1.5),
  fInnerDiameter(50*cm),
  fOuterDiameter(60*cm),
  fHeight(10*cm),
  worldLog(NULL),
  worldPhy(NULL),
  sCrystal_Log(NULL) {
    fMessenger = new PETDetectorMessenger(this);
}

PETDetectorConstruction::~PETDetectorConstruction() {
}

void PETDetectorConstruction::BuildMaterial() {
  G4NistManager * nist = G4NistManager::Instance();
  G4double z, a, density;

  // Air
  fAir = nist->FindOrBuildMaterial("G4_AIR");

  // Liquid Argon (Properties data taken from LArSOFT)
  fLAr = new G4Material("LiquidArgon", z = 18., a = 39.948 * g / mole, density = 1.3954 * g / cm3);
}

G4VPhysicalVolume * PETDetectorConstruction::Construct() {
  BuildMaterial();
  
  // Overlapping check
  G4bool checkOverlaps = true;

  // Clear old geometry if any
  if (worldPhy != NULL) {
      G4GeometryManager::GetInstance()->OpenGeometry();
      G4PhysicalVolumeStore::GetInstance()->Clean();
      G4LogicalVolumeStore::GetInstance()->Clean();
      G4SolidStore::GetInstance()->Clean();
  }

  sCrystal_Mod = new G4Tubs(
    "SingleCrystalModel",
    fInnerDiameter,
    fOuterDiameter,
    fHeight*0.5,
    0,
    fOpeningAngle);
  sCrystal_Log = new G4LogicalVolume(sCrystal_Mod, fLAr,"SingleCrystalLogicalVolume", 0, 0, 0);

  worldBox = new G4Box("WorldBox", world_hx, world_hy, world_hz);
  worldLog = new G4LogicalVolume(worldBox, fAir, "WorldLog");


  sCrystal_Phy = new G4PVPlacement(0, G4ThreeVector(), sCrystal_Log, "SingleCrystalPhysicalVolume", worldLog, false, 0);
  worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);

  return worldPhy;
}

void PETDetectorConstruction::SetOpeningAngle(G4double newValue) {
    this->fOpeningAngle = newValue;
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void PETDetectorConstruction::SetInnerDiameter(G4double newValue) {
    this->fInnerDiameter = newValue;
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void PETDetectorConstruction::SetOuterDiameter(G4double newValue) {
    this->fOuterDiameter = newValue;
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

void PETDetectorConstruction::SetHeight(G4double newValue) {
    this->fHeight = newValue;
    G4RunManager::GetRunManager()->DefineWorldVolume(Construct());
}

