#include "mdomEventAction.hh"
#include "mdomRunAction.hh"
#include "mdomTrackingAction.hh"
#include "mdomPMT.hh"
#include "mdomAnalysisManager.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"
#include "G4RunManager.hh"
//#include "TH1.h"
extern G4int gDecayConditional;
extern MdomAnalysisManager gAnalysisManager;
extern mdomPMT gPMTAnalysis;

extern G4String	ghitsfilename;
extern G4String	gHittype;


mdomEventAction::mdomEventAction()
{}

mdomEventAction::~mdomEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ooOO0OOooo........oooOO0OOooo......
void mdomEventAction::BeginOfEventAction(const G4Event* evt)
{ gDecayConditional=0;
  gAnalysisManager.current_event_id = evt->GetEventID();
  gAnalysisManager.ResetEvent();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ooOO0OOooo........oooOO0OOooo......
void mdomEventAction::EndOfEventAction(const G4Event* evt)
{
  
  //   For debugging,  it saves all the events that met a certain condition (infiniteLoop true)
  //   if (gAnalysisManager.SaveThisEvent == true){
  //     G4cout << "Guardado" << G4endl;
  //     G4EventManager* em = G4EventManager::GetEventManager();
  //     em->KeepTheCurrentEvent();
  //     }
  
  
  //   These two vector were made in the times where the output was saved at the 
  //   end of the run and not at the end of each event.
  //   I left it like it is,  for making the things easier if we want to change 
  //   the writing-time again,  but right now they are just 1-element-vectors.
  gAnalysisManager.NrScintillationVec.push_back(gAnalysisManager.NrScintillation); 
  gAnalysisManager.NrCerenkovVec.push_back(gAnalysisManager.NrCerenkov);
  
  
  gPMTAnalysis.Analysis();
  
  
  if (gHittype == "onlyHits") {
    gAnalysisManager.WriteAccept();
  }
  
  if (gHittype == "detailed") {
    gAnalysisManager.WriteDetailPhotons(); 
    //gAnalysisManager.WriteMotherDecay();
    //gAnalysisManager.WriteAccept();
  }
  
  
  gAnalysisManager.Reset();
}
