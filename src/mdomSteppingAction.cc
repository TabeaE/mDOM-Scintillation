#include "mdomSteppingAction.hh"

#include "G4RunManager.hh"
#include "G4SteppingManager.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
//since Geant4.10: include units manually
#include "G4SystemOfUnits.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "mdomAnalysisManager.hh"
#include "SteppingMessenger.hh"
#include "mdomPMT.hh"



extern MdomAnalysisManager gAnalysisManager;
extern G4int gDecayConditional;
extern mdomPMT gPMTAnalysis;
extern G4int gQE;

mdomSteppingAction::mdomSteppingAction()
:G4UserSteppingAction()
{  
  
  fullChain = 1;
  fStepMessenger = new SteppingMessenger(this);   
}

mdomSteppingAction::~mdomSteppingAction()
{
  delete fStepMessenger;
}


void mdomSteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4double Ekin;
  G4double t1, t2;
  G4double t3;
  G4double killTime=10.*s;                                   //one week
  G4Track* aTrack = aStep->GetTrack();
  
  
  std::vector<G4String> n;
  extern std::vector<G4String> explode (G4String s, char d);
  //G4ThreeVector deltapos;
  
  
  gAnalysisManager.decayModus = fullChain;
  //
  //------------------------------------------------------------------
  
  
  
  // Find position of decay
  if ( ! gAnalysisManager.foundDecay ) {
    if ( aTrack->GetCreatorProcess() ) {
      if ( aTrack->GetCreatorProcess()->GetProcessName() == "RadioactiveDecay" ) {
	gAnalysisManager.foundDecay = true;
	
	gAnalysisManager.Decay.push_back({
	  gAnalysisManager.current_event_id, // NewMother_event_id
	  aTrack->GetVertexPosition().getTheta(), //decayTheta
	  aTrack->GetVertexPosition().getPhi(), // decayPhi
	  aTrack->GetVertexPosition().getR()}); // decayR
      }
    }
  }
  
  
  /*
   *  // Energie die in das Glass deponiert wird.
   *  if (( aStep->GetTotalEnergyDeposit() )&&((aStep-> GetPostStepPoint()->GetMaterial()->GetName() == "Vessel Glass"))) {
   *  G4double energyloss = aStep->GetTotalEnergyDeposit()/MeV;
   *  gAnalysisManager.energyDeposited=gAnalysisManager.energyDeposited+energyloss;  
}
*/ 
  
  
  
  
  //force 'single' decay, kills every ion that does not have the ID 1 (your starting nucleus).
  if ((fullChain==2)&&(aTrack->GetDefinition()->GetParticleType() == "nucleus")&&(aTrack->GetDefinition()->GetParticleName()!="alpha")&&(aTrack->GetTrackID() > 1)) {
    if ( aTrack->GetTrackStatus() != fStopAndKill ) {
      aTrack->SetTrackStatus(fStopAndKill);
    }
  }
  
  
  //Time restricted Decay
  if (fullChain==3){
    if ((gDecayConditional==0)&&(aTrack->GetDefinition()->GetParticleType() == "nucleus")&&(aTrack->GetDefinition()->GetParticleName()!="alpha")&&(aTrack->GetTrackID() > 1)&&(aTrack->GetDefinition()->GetPDGLifeTime() != 0.)) {
      t3=aTrack->GetGlobalTime()/s;
      gDecayConditional=1;
    }
    
    if (( aTrack->GetTrackStatus() != fStopAndKill )&&(gDecayConditional>0)&&(aTrack->GetGlobalTime()>(t3+killTime))&&(aTrack->GetDefinition()->GetParticleType() == "nucleus")&&(aTrack->GetDefinition()->GetParticleName()!="alpha")&&(aTrack->GetTrackID() > 1)) {
      aTrack->SetTrackStatus(fStopAndKill);    
    }
  }
  
  
  
  
  if ( (aTrack->GetDefinition()->GetParticleName() == "gamma") || (aTrack->GetDefinition()->GetParticleName() == "e-") ) {
    if ( aTrack->GetTrackStatus() != fStopAndKill ) {
      if ( (aStep->GetPostStepPoint()->GetMaterial()->GetName() == "Photocathode")&&(aTrack->GetTrackStatus() != fStopAndKill ) ) {
	aTrack->SetTrackStatus(fStopAndKill);
      }
    }
  }
  
  
  if ( aTrack-> GetCurrentStepNumber() > 50000) {
    //G4cout << "Al infinito y mas alla!!!!!" << G4endl;
    // gAnalysisManager.infiniteLoop = true;
    if ( aTrack->GetTrackStatus() != fStopAndKill ) {
      aTrack->SetTrackStatus(fStopAndKill);
    }
  }
  
  
  
  
  //	Check if optical photon is about to hit a photocathode, if so, destroy it and save the hit
  if ( aTrack->GetDefinition()->GetParticleName() == "opticalphoton" ) {
    //     if (aTrack->GetNextVolume()==0) {
    //   		    gAnalysisManager.OutOfWorldCounter++;
    //   		  }
    if ( aTrack->GetTrackStatus() != fStopAndKill ) {
      if ( (aStep->GetPostStepPoint()->GetMaterial()->GetName() == "Photocathode")&&(aTrack->GetTrackStatus() != fStopAndKill ) ) {
	
	G4double h = 4.136E-15*eV*s;
	G4double c = 2.99792458E17*nm/s;
	G4double lambda;
	
	Ekin = aTrack->GetKineticEnergy();
	lambda = h*c/Ekin;
	
	//_------------------------------
	// 	if ( lambda/nm >= 740 ) {
	//    G4cout << "Al infinito y mas alla!!!!!" << G4endl;
	//    gAnalysisManager.SaveThisEvent = true;
	//    }
	//-------------------------------------------------
	
	if( (gQE==0) || ((gPMTAnalysis.QEcheck(lambda)) && (gQE==1))) {
	  t1 = aTrack->GetGlobalTime();
	  t2 = aTrack->GetLocalTime();	
	  n = explode(aStep->GetPreStepPoint()->GetPhysicalVolume()->GetName(),'_');  
	  gAnalysisManager.atPhotocathode.push_back({
	    gAnalysisManager.current_event_id, // stats_event_id
	    aTrack->GetTrackID(), //hitPhotonID
	    aTrack->GetParentID(), // photonParent
	    1, // realHit
	    t1, // stats_hit_time
	    t2, // stats_photon_flight_time
	    Ekin/eV, // stats_photon_energy
	    atoi(n.at(1)), // stats_PMT_hit
	    aTrack->GetMomentumDirection(),//stats_photon_direction
	    aTrack->GetPosition(),//stats_photon_position
	    "default", //Mother Particle Name yet not known.
	    1}); // Amplitude 
	  
	  aTrack->SetTrackStatus(fStopAndKill); // kills counted photon to prevent scattering and double-counting   
	}
      }
    }
  }
}
