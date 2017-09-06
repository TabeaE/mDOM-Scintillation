#ifndef mdomSteppingAction_h
#define mdomSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"

#include <vector>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class SteppingMessenger;

class mdomSteppingAction : public G4UserSteppingAction
{
  public:
    mdomSteppingAction();
   ~mdomSteppingAction();

    void UserSteppingAction(const G4Step*);
    void SetFullChain(G4int i) { fullChain = i;};
    
  private:
    SteppingMessenger*  fStepMessenger;
    G4int   fullChain;


		
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
