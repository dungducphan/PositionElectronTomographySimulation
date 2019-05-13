#ifndef PETDetectorConstruction_h
#define PETDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4MaterialTable.hh"
#include "G4Material.hh"
#include "G4Tubs.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
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

  virtual void ConstructSDandField();

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
  G4VPhysicalVolume *worldPhy;

  G4Box *solidCryst;
  G4LogicalVolume *logicCryst;

  G4Box *solidDet_outter;
  G4Box *solidDet_inner;
  G4LogicalVolume *logicDet_outter;
  G4LogicalVolume *logicDet_inner;



  G4Trap *solidGel;
  //  G4Tubs *solidGel;
  G4LogicalVolume *logicGel;
 
  
  G4Material *fAir;
  G4Material *fNaI;
  G4Material *fLYSO;
  G4Material *fTeflon;
  G4Material *fLutetiumOxide;
  G4Material *fYttriumOxide;
  G4Material *fSiliconDioxide;
  G4Material *fGel;
  G4MaterialPropertiesTable *fLYSO_MPT;
  G4MaterialPropertiesTable *fAir_MPT;
  G4MaterialPropertiesTable *fGel_MPT;

  
  PETDetectorMessenger *fMessenger;
};

#endif
