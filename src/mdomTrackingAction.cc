#include "mdomEventAction.hh"
#include "mdomTrackingAction.hh"
#include "mdomRunAction.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4ThreeVector.hh"


mdomTrackingAction::mdomTrackingAction()
:G4UserTrackingAction()
{

}

mdomTrackingAction::~mdomTrackingAction()
{
}

void mdomTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{	
}

void mdomTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
}
