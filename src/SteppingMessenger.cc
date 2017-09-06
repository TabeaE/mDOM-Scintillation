
#include "SteppingMessenger.hh"
#include "mdomSteppingAction.hh"

#include "G4UIcmdWithAnInteger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingMessenger::SteppingMessenger(mdomSteppingAction* trackA)
:fmdomSteppingAction(trackA)
{
  fSteppingCmd = new G4UIcmdWithAnInteger("/mdomDECAYcmd/fullChain",this);
  fSteppingCmd->SetGuidance("1:Full Chain \n 2: Single Decay \n 3: Time restricted");
  fSteppingCmd->SetParameterName("flag",true);
  fSteppingCmd->SetDefaultValue(1);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingMessenger::~SteppingMessenger()
{
  delete fSteppingCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ 
  if (command == fSteppingCmd)
    { fmdomSteppingAction->SetFullChain(fSteppingCmd->GetNewIntValue(newValue));}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
