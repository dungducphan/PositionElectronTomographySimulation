#ifndef LSDetectorConstruction_h
#define LSDetectorConstruction_h 1

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
#include "globals.hh"

class PETDetectorMessenger;

class G4VPhysicalVolume;

class G4LogicalVolume;

class PETDetectorConstruction : public G4VUserDetectorConstruction {
public:
    PETDetectorConstruction();
    virtual ~PETDetectorConstruction();
    virtual G4VPhysicalVolume *Construct();
    virtual void BuildMaterial();

    // set methods
    void SetOpeningAngle(G4double newValue);
    void SetInnerDiameter(G4double newValue);
    void SetOuterDiameter(G4double newValue);
    void SetHeight(G4double newValue);

private:
    G4double fOpeningAngle;
    G4double fInnerDiameter;
    G4double fOuterDiameter;
    G4double fHeight;

    G4double world_hx;
    G4double world_hy;
    G4double world_hz;
    G4Box *worldBox;
    G4LogicalVolume *worldLog;
    G4VPhysicalVolume* worldPhy;

    G4Tubs *sCrystal_Mod;
    G4LogicalVolume *sCrystal_Log;

    G4Material *fAir;
    G4Material *fLAr;
    G4Material *fAl;
    G4MaterialPropertiesTable *fLAr_mt;

    PETDetectorMessenger *fMessenger;
};

#endif
