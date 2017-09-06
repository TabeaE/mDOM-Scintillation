#ifndef SteppingMessenger_h
#define SteppingMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class mdomSteppingAction;
class G4UIcmdWithAnInteger;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingMessenger: public G4UImessenger
{
  public:
    SteppingMessenger(mdomSteppingAction*);
   ~SteppingMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
  private:
    mdomSteppingAction*   fmdomSteppingAction;    
    G4UIcmdWithAnInteger* fSteppingCmd;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
