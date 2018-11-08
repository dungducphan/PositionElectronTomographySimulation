//
// Created by dphan on 11/8/18.
//

#ifndef PETDETECTORMESSENGER_HH
#define PETDETECTORMESSENGER_HH

#include "G4UImessenger.hh"
#include "globals.hh"

class PETDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWith3VectorAndUnit;
class G4UIcmdWithADouble;
class G4UIcmdWithADoubleAndUnit;

class PETDetectorMessenger: public G4UImessenger
{
public:
    PETDetectorMessenger(PETDetectorConstruction* );
    ~PETDetectorMessenger();

    virtual void SetNewValue(G4UIcommand* command, G4String newValue);

private:
    PETDetectorConstruction*      fDetectorConstruction;
    G4UIdirectory*                fDirectory;
    G4UIcmdWithADoubleAndUnit*           fSetOpeningAngleCmd;
    G4UIcmdWithADoubleAndUnit*           fSetInnerDiameterCmd;
    G4UIcmdWithADoubleAndUnit*           fSetOuterDiameterCmd;
    G4UIcmdWithADoubleAndUnit*           fSetHeightCmd;
};

#endif //PETSIM_PETDETECTORMESSENGER_HH
