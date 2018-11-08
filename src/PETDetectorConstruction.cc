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

  G4Element *Oxigen = nist->FindOrBuildElement("O");
  G4Element *Lutetium = nist->FindOrBuildElement("Lu");
  G4Element *Yttrium = nist->FindOrBuildElement("Y");

  // Air
  fAir = nist->FindOrBuildMaterial("G4_AIR");

  // Lu2O3
  fLutetiumOxide = new G4Material("LutetiumOxide", 9.41 * g / cm3, 2);
  fLutetiumOxide->AddElement(Lutetium, 2);
  fLutetiumOxide->AddElement(Oxigen, 3);

  // SiO2
  fSiliconDioxide = nist->FindOrBuildMaterial("G4_SILICON_DIOXIDE");

  // Y2O3
  fYttriumOxide = new G4Material("YttriumOxide", 5.01 * g / cm3, 2);
  fYttriumOxide->AddElement(Yttrium, 2);
  fYttriumOxide->AddElement(Oxigen, 3);

  // LYSO
  fLYSO = new G4Material("LYSO", 7.1 * g / cm3, 3);
  fLYSO->AddMaterial(fLutetiumOxide, 81 * perCent);
  fLYSO->AddMaterial(fSiliconDioxide, 14 * perCent);
  fLYSO->AddMaterial(fYttriumOxide, 5 * perCent);

  // Optical Properties of Materials
  const G4int num = 21;
  G4double ene[num] = {1.79 * eV, 1.85 * eV, 1.91 * eV, 1.97 * eV,
                       2.04 * eV, 2.11 * eV, 2.19 * eV, 2.27 * eV,
                       2.36 * eV, 2.45 * eV, 2.56 * eV, 2.67 * eV,
                       2.80 * eV, 2.94 * eV, 3.09 * eV, 3.25 * eV,
                       3.44 * eV, 3.65 * eV, 3.89 * eV, 4.16 * eV,
                       4.43 * eV};

  G4double fast[num] = {0.01, 0.10, 0.20, 0.50,
                        0.90, 1.70, 2.90, 5.00,
                        8.30, 12.5, 17.0, 22.9,
                        26.4, 25.6, 16.8, 4.20,
                        0.30, 0.20, 0.10, 0.01,
                        0.00};

  G4double rLyso[num] = {1.81, 1.81, 1.81, 1.81,
                         1.81, 1.81, 1.81, 1.81,
                         1.81, 1.81, 1.81, 1.81,
                         1.81, 1.81, 1.81, 1.81,
                         1.81, 1.81, 1.81, 1.81,
                         1.81};
  G4double rAir[num] = {1.00, 1.00, 1.00, 1.00,
                        1.00, 1.00, 1.00, 1.00,
                        1.00, 1.00, 1.00, 1.00,
                        1.00, 1.00, 1.00, 1.00,
                        1.00, 1.00, 1.00, 1.00,
                        1.00};

  G4double abs[num] = {50 * cm, 50 * cm, 50 * cm, 50 * cm,
                       50 * cm, 50 * cm, 50 * cm, 50 * cm,
                       50 * cm, 50 * cm, 50 * cm, 50 * cm,
                       50 * cm, 50 * cm, 50 * cm, 50 * cm,
                       50 * cm, 50 * cm, 50 * cm, 50 * cm,
                       50 * cm};

  G4MaterialPropertiesTable *mpt1 = new G4MaterialPropertiesTable();
  G4MaterialPropertiesTable *mpt2 = new G4MaterialPropertiesTable();

  mpt1->AddProperty("FASTCOMPONENT", ene, fast, num);
  mpt1->AddProperty("RINDEX", ene, rLyso, num);
  mpt1->AddProperty("ABSLENGTH", ene, abs, num);
  mpt1->AddConstProperty("SCINTILLATIONYIELD", 33.2 / keV);
  mpt1->AddConstProperty("RESOLUTIONSCALE", 1);
  mpt1->AddConstProperty("FASTTIMECONSTANT", 43 * ns);

  mpt2->AddProperty("RINDEX", ene, rAir, num);

  fLYSO->SetMaterialPropertiesTable(mpt1);
  fAir->SetMaterialPropertiesTable(mpt2);
}

G4VPhysicalVolume * PETDetectorConstruction::Construct() {
  BuildMaterial();
  
  // Overlapping check
  G4bool checkOverlaps = true;

  // Clear old geometry if any
  if (worldPhy) {
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
  sCrystal_Log = new G4LogicalVolume(sCrystal_Mod, fLYSO, "SingleCrystalLogicalVolume", 0, 0, 0);

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

