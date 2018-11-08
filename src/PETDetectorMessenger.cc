//
// Created by dphan on 11/8/18.
//

#include "PETDetectorMessenger.hh"
#include "PETDetectorConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

PETDetectorMessenger::PETDetectorMessenger(PETDetectorConstruction* detectorConstruction)
: G4UImessenger(),
  fDetectorConstruction(detectorConstruction),
  fDirectory(NULL),
  fSetOpeningAngleCmd(NULL) {
    fDirectory = new G4UIdirectory("/customize/geometry/");
    fDirectory->SetGuidance("Customize Single Crystal geometry");

    fSetOpeningAngleCmd = new G4UIcmdWithADoubleAndUnit("/customize/geometry/setOpeningAngle", this);
    fSetOpeningAngleCmd->SetGuidance("Set Opening Angle for a single crystal module.");
    fSetOpeningAngleCmd->SetParameterName("Opening Angle", false);
    fSetOpeningAngleCmd->SetDefaultUnit("radian");
    fSetOpeningAngleCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fSetOpeningAngleCmd->SetToBeBroadcasted(false);

    fSetInnerDiameterCmd = new G4UIcmdWithADoubleAndUnit("/customize/geometry/setInnerDiameter", this);
    fSetInnerDiameterCmd->SetGuidance("Set Inner Diameter for a single crystal module.");
    fSetInnerDiameterCmd->SetParameterName("Inner Diameter", false);
    fSetInnerDiameterCmd->SetDefaultUnit("cm");
    fSetInnerDiameterCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fSetInnerDiameterCmd->SetToBeBroadcasted(false);

    fSetOuterDiameterCmd = new G4UIcmdWithADoubleAndUnit("/customize/geometry/setOuterDiameter", this);
    fSetOuterDiameterCmd->SetGuidance("Set Outer Diameter for a single crystal module.");
    fSetOuterDiameterCmd->SetParameterName("Outer Diameter", false);
    fSetOuterDiameterCmd->SetDefaultUnit("cm");
    fSetOuterDiameterCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fSetOuterDiameterCmd->SetToBeBroadcasted(false);

    fSetHeightCmd = new G4UIcmdWithADoubleAndUnit("/customize/geometry/setHeight", this);
    fSetHeightCmd->SetGuidance("Set Height for a single crystal module.");
    fSetHeightCmd->SetParameterName("Height", false);
    fSetHeightCmd->SetDefaultUnit("cm");
    fSetHeightCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
    fSetHeightCmd->SetToBeBroadcasted(false);
}

PETDetectorMessenger::~PETDetectorMessenger() {
    delete fDirectory;
    delete fSetOpeningAngleCmd;
    delete fSetInnerDiameterCmd;
    delete fSetOuterDiameterCmd;
    delete fSetHeightCmd;
}

void PETDetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
    if (command == fSetOpeningAngleCmd) {
        fDetectorConstruction->SetOpeningAngle(fSetOpeningAngleCmd->GetNewDoubleValue(newValue));
    }
    if (command == fSetInnerDiameterCmd) {
        fDetectorConstruction->SetInnerDiameter(fSetInnerDiameterCmd->GetNewDoubleValue(newValue));
    }
    if (command == fSetOuterDiameterCmd) {
        fDetectorConstruction->SetOuterDiameter(fSetOuterDiameterCmd->GetNewDoubleValue(newValue));
    }
    if (command == fSetHeightCmd) {
        fDetectorConstruction->SetHeight(fSetHeightCmd->GetNewDoubleValue(newValue));
    }
}
