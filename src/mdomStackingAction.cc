#include "mdomStackingAction.hh"
#include "mdomSteppingAction.hh"
#include "mdomAnalysisManager.hh"

#include "G4ios.hh"
#include "G4ParticleDefinition.hh"
#include "G4ParticleTypes.hh"
#include "G4Track.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4EventManager.hh"



extern MdomAnalysisManager gAnalysisManager;

mdomStackingAction::mdomStackingAction() {}


mdomStackingAction::~mdomStackingAction() {}



G4ClassificationOfNewTrack mdomStackingAction::ClassifyNewTrack(const G4Track * aTrack){
  
  //Count which process generated the optical photons
  if(aTrack->GetDefinition()->GetParticleName() == "opticalphoton"){
    // particle is optical photon
    if(aTrack->GetParentID()>0){
      // particle is secondary
      if(aTrack->GetCreatorProcess()->GetProcessName()=="Scintillation"){
	gAnalysisManager.NrScintillation++;
	gAnalysisManager.photonIds.push_back(aTrack->GetTrackID());
	gAnalysisManager.creationProcess.push_back("s");
      }
      else{ if(aTrack->GetCreatorProcess()->GetProcessName()=="Cerenkov"){
	gAnalysisManager.NrCerenkov++; 
	gAnalysisManager.photonIds.push_back(aTrack->GetTrackID());
	gAnalysisManager.creationProcess.push_back("c");
      }}
    }
  }
  
  
  //Makes three vectors with Name/ID/ParentID/ParticleType      
  if (( aTrack->GetDefinition()->GetParticleName() != "opticalphoton")) {
    gAnalysisManager.allParticles.push_back({
      aTrack->GetTrackID(), //particlesIDs
      aTrack->GetDefinition()->GetParticleName(), //particlesNames
      aTrack->GetDefinition()->GetParticleType(), // particlesType
      aTrack->GetParentID()});    //parentParticlesIDs
  }
  
  return fUrgent;
}



void mdomStackingAction::NewStage() {}



void mdomStackingAction::PrepareNewEvent() {}
