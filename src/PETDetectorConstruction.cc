#include "PETDetectorConstruction.hh"
#include "PETDetectorMessenger.hh"

#include "G4SDManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4GeometryManager.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4PVReplica.hh"

#include "TMath.h"

#include <iostream>
#include <fstream>
#include <vector>


PETDetectorConstruction::PETDetectorConstruction()
        : G4VUserDetectorConstruction(),
          world_hx(1.0 * m),
          world_hy(1.0 * m),
          world_hz(1.0 * m),
          fMessenger(NULL),
          fLYSO_MPT(NULL),
          fAir_MPT(NULL),
          fOpeningAngle(1.5),
          fInnerDiameter(50 * cm),
          fOuterDiameter(60 * cm),
          fHeight(10 * cm),
          worldLog(NULL),
          worldPhy(NULL),
          sCrystal_Log(NULL) {
    fMessenger = new PETDetectorMessenger(this);
}

PETDetectorConstruction::~PETDetectorConstruction() {
}

void PETDetectorConstruction::BuildMaterial() {
    G4NistManager *nist = G4NistManager::Instance();

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

    fLYSO_MPT = new G4MaterialPropertiesTable();
    fAir_MPT = new G4MaterialPropertiesTable();

    fLYSO_MPT->AddProperty("FASTCOMPONENT", ene, fast, num);
    fLYSO_MPT->AddProperty("RINDEX", ene, rLyso, num);
    fLYSO_MPT->AddProperty("ABSLENGTH", ene, abs, num);
    fLYSO_MPT->AddConstProperty("SCINTILLATIONYIELD", 0.5 / keV);
    fLYSO_MPT->AddConstProperty("RESOLUTIONSCALE", 1);
    fLYSO_MPT->AddConstProperty("FASTTIMECONSTANT", 43 * ns);

    fAir_MPT->AddProperty("RINDEX", ene, rAir, num);

    fLYSO->SetMaterialPropertiesTable(fLYSO_MPT);
    fAir->SetMaterialPropertiesTable(fAir_MPT);
}

G4VPhysicalVolume *PETDetectorConstruction::Construct() {
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
            fHeight * 0.5,
            0,
            fOpeningAngle);

    sDet_Mod = new G4Tubs(
            "SiPMModel",
            fOuterDiameter,
            fOuterDiameter + 0.2,
            fHeight * 0.5,
            0,
            fOpeningAngle);

    sCrystalDet_Mod = new G4Tubs(
            "SingleCrystalWithSiPMModel",
            fInnerDiameter,
            fOuterDiameter + 0.2,
            fHeight * 0.5,
            0,
            fOpeningAngle);

    crystalRing_Mod = new G4Tubs(
            "SingleCrystalModel",
            fInnerDiameter - fRingInnerDiameterBuffer,
            fOuterDiameter + fRingOuterDiameterBuffer,
            (fHeight + fRingThicknessBuffer) * 0.5,
            0,
            2 * TMath::Pi());

    worldBox = new G4Box("WorldBox", world_hx, world_hy, world_hz);

    sDet_Log = new G4LogicalVolume(sDet_Mod, fLYSO, "DetectorLogicalVolume", 0, 0, 0);
    sCrystal_Log = new G4LogicalVolume(sCrystal_Mod, fLYSO, "SingleCrystalLogicalVolume", 0, 0, 0);
    sCrystalDet_Log = new G4LogicalVolume(sCrystalDet_Mod, fAir, "SingleCrystalWithSiPMLogicalVolume", 0, 0, 0);
    crystalRing_Log = new G4LogicalVolume(crystalRing_Mod, fAir, "CrystalRingLogicalVolume", 0, 0, 0);
    worldLog = new G4LogicalVolume(worldBox, fAir, "WorldLog");

    // Placement
    new G4PVPlacement(0, G4ThreeVector(), sCrystal_Log, "", sCrystalDet_Log, false, 0);
    new G4PVPlacement(0, G4ThreeVector(), sDet_Log, "", sCrystalDet_Log, false, 0);

    // Module assembly
    G4int nCrystalInARing = TMath::Floor((2*TMath::Pi())/fOpeningAngle);
    G4double widthOfSingleRep = (2*TMath::Pi())/(G4double)nCrystalInARing;
    new G4PVReplica("Ring", sCrystalDet_Log, crystalRing_Log, kPhi, nCrystalInARing, widthOfSingleRep, 0);

    /* TODO
     *  - Give the right geometry.
     *  - But in runtime, the program crashes. My guess is that I need to
     *  rewrite the TrackerSD. Also, change name of class TrackSD. SiPM is not a tracker.
     */
    new G4PVReplica("Tube", crystalRing_Log, worldLog, kZAxis, 10, fHeight + 1*mm, 0);

    // Return physical volumes
    worldPhy = new G4PVPlacement(0, G4ThreeVector(), worldLog, "WorldPhy", 0, false, 0, checkOverlaps);


    // Vis touch-up
    G4VisAttributes *crystVisAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
    G4VisAttributes *SiPMVisAttributes = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    sCrystal_Log->SetVisAttributes(crystVisAttributes);
    sDet_Log->SetVisAttributes(SiPMVisAttributes);

    // Surface properties of the detector
    G4OpticalSurface *opDetectorSurface = new G4OpticalSurface("DetectorSurface");
    opDetectorSurface->SetType(dielectric_metal);
    opDetectorSurface->SetFinish(ground);
    opDetectorSurface->SetModel(glisur);
    G4LogicalSkinSurface *detectorSurface = new G4LogicalSkinSurface("DetectorSurface", sDet_Log, opDetectorSurface);

    G4double ephoton_pmt[4] = {0.0001 * eV, 1 * eV, 10 * eV, 100 * eV};
    G4double reflectivity_pmt[4] = {0, 0, 0, 0};
    G4double efficiency_pmt[4] = {1.0, 1.0, 1.0, 1.0};

    G4MaterialPropertiesTable *detectorMPT = new G4MaterialPropertiesTable();
    detectorMPT->AddProperty("REFLECTIVITY", ephoton_pmt, reflectivity_pmt, 4);
    detectorMPT->AddProperty("EFFICIENCY", ephoton_pmt, efficiency_pmt, 4);
    opDetectorSurface->SetMaterialPropertiesTable(detectorMPT);

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

void PETDetectorConstruction::ConstructSDandField() {
    // Sensitive detectors

    aTrackerSD = new PETTrackerSD("TrackerChamberSD");
    G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);

    // Setting aTrackerSD to all logical volumes with the same name of "Chamber_LV".
    SetSensitiveDetector("DetectorLogicalVolume", aTrackerSD, true);
}

G4int PETDetectorConstruction::CalculateNReplicaInARing() {
    return TMath::Floor((2 * TMath::Pi()) / this->fOpeningAngle);
}

