#ifndef PETDetectorConstruction_h
#define PETDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4MaterialTable.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4VisAttributes.hh"
#include "G4AssemblyVolume.hh"
#include "globals.hh"

#include "PETTrackerSD.hh"

class PETDetectorMessenger;

class G4VPhysicalVolume;

class G4LogicalVolume;

class PETDetectorConstruction : public G4VUserDetectorConstruction {
public:
    PETDetectorConstruction();

    virtual ~PETDetectorConstruction();

    virtual G4VPhysicalVolume *Construct();

    virtual void BuildMaterial();

    virtual void ConstructSDandField();

    // set methods
    void SetOpeningAngle(G4double newValue);

    void SetInnerDiameter(G4double newValue);

    void SetOuterDiameter(G4double newValue);

    void SetHeight(G4double newValue);

    // Geometry Calculation methods
    G4int CalculateNReplicaInARing();

private:
    // Single Crystal Variables
    G4double fOpeningAngle;
    G4double fInnerDiameter;
    G4double fOuterDiameter;
    G4double fHeight;

    G4Tubs *sCrystal_Mod;
    G4Tubs *sDet_Mod;
    G4Tubs *sCrystalDet_Mod;
    G4LogicalVolume *sCrystal_Log;
    G4LogicalVolume *sDet_Log;
    G4LogicalVolume *sCrystalDet_Log;
//    G4VPhysicalVolume *sCrystal_Phy;
//    G4VPhysicalVolume *sDet_Phy;
//    G4VPhysicalVolume *sCrystalDet_Phy;

    PETTrackerSD *aTrackerSD;

    // Ring of Crystal Modules Variables
    G4double fRingInnerDiameterBuffer;
    G4double fRingOuterDiameterBuffer;
    G4double fRingThicknessBuffer;

    G4Tubs            *crystalRing_Mod;
    G4LogicalVolume   *crystalRing_Log;
//    G4VPhysicalVolume *crystalRing_Phy;

    // General Variables
    G4double world_hx;
    G4double world_hy;
    G4double world_hz;
    G4Box *worldBox;
    G4LogicalVolume *worldLog;
    G4VPhysicalVolume *worldPhy;

    G4Material *fAir;
    G4Material *fLYSO;
    G4Material *fLutetiumOxide;
    G4Material *fYttriumOxide;
    G4Material *fSiliconDioxide;
    G4MaterialPropertiesTable *fLYSO_MPT;
    G4MaterialPropertiesTable *fAir_MPT;

    PETDetectorMessenger *fMessenger;
};

#endif
