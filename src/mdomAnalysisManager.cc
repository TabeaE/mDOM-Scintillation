#include "mdomAnalysisManager.hh"
#include "G4ios.hh"
//since Geant4.10: include units manually
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
MdomAnalysisManager::MdomAnalysisManager(){
}

MdomAnalysisManager::~MdomAnalysisManager(){}


void MdomAnalysisManager::ResetEvent()
{	
  foundDecay = false;
  decayModus=4;
  NrScintillation=0;
  NrCerenkov=0;
  //energyDeposited=0;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ooOO0OOooo........oooOO0OOooo......
// -------------------------------------------WRITE FUNCTIONS----------------------------------------------------
void MdomAnalysisManager::WriteDetailPhotons()
{           for (int i = 0; i < (int) atPhotocathode.size(); i++) {
if (true) {

  //datafile << "# event#     Name Mother Nucleus    hit time/ns  flight time/ns  track length/m  energy/eV PMT#  event distance/m  photon position[m]: x,y,z  direction: x,y,z r[m] ParentId"<<G4endl;		 
  datafile << atPhotocathode.at(i).stats_event_id << "\t";
  //datafile << stats_mothername.at(i) << "\t";
  datafile << atPhotocathode.at(i).stats_hit_time << "\t";
  //datafile << stats_photon_flight_time.at(i) << "\t";
  //datafile << stats_photon_track_length.at(i) << "\t";
  //datafile << stats_photon_energy.at(i) << "\t";
  //datafile << photonAmplitude.at(i) << "\t";
    datafile << atPhotocathode.at(i).Amplitude << "\t";
    datafile << atPhotocathode.at(i).realHit << "\t";
  //datafile << stats_event_distance.at(i) << "\t";
  //datafile << stats_photon_Xposition.at(i)/m << "\t";
  //datafile << stats_photon_Yposition.at(i)/m << "\t";
  //datafile << stats_photon_Zposition.at(i)/m << "\t";
  //datafile << stats_photon_direction.at(i).x() << "\t";
  //datafile << stats_photon_direction.at(i).y() << "\t";
  //datafile << stats_photon_direction.at(i).z() << "\t";
  //datafile << stats_photon_position.at(i).mag() / m ;
  //datafile << photonParent.at(i) << "\t";
  //datafile << hitMotherName.at(i) << "\t";
   //for (int j=0; j < (int) photons.size(); j++){
   // if (atPhotocathode.at(i).hitPhotonID == photons.at(i).photonIds) {
      
  //    datafile <<  photons.at(j).creationProcess;
   //   break;
  //  }
 // }
  datafile << G4endl;
  }
}

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ooOO0OOooo........oooOO0OOooo......
void MdomAnalysisManager::WriteMotherDecay()
{
  //datafile << "# event#|	|MotherPos Theta	Phi	R|	|#Hits|	|#Cerenkov|	|#Scintillation|	|DecayModus|"<<G4endl;	
  for (int i = 0; i < (int) Decay.size(); i++) {
    
    datafileMother << Decay.at(i).NewMother_event_id << "\t";
    datafileMother << Decay.at(i).decayTheta << "\t" ;
    datafileMother << Decay.at(i).decayPhi << "\t" ;
    datafileMother << Decay.at(i).decayR << "\t" ;
    datafileMother << NrCerenkovVec.at(i) << "\t" ;
    datafileMother << NrScintillationVec.at(i);                //<< "\t" << decayModus;
    
    datafileMother << G4endl;
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ooOO0OOooo........oooOO0OOooo......
void MdomAnalysisManager::WriteAccept()
{
  int	pmthits[25] = {0};
  int sum = 0;
  int alone = 0;
  //datafile << "# test header" << G4endl;
  
  
  // repacking hits:
  for (int i = 0; i < (int) atPhotocathode.size(); i++) {

    if (atPhotocathode.at(i).realHit == 1 || atPhotocathode.at(i).realHit == 2) {
      pmthits[atPhotocathode.at(i).stats_PMT_hit] += 1;
    }
    if (atPhotocathode.at(i).realHit == -1) {
      alone += 1;
    }
  }
  // wrinting collective hits
  for (int j = 0; j < 24; j++) {
    //datafileTest << "\t" << pmthits[j];
    sum += pmthits[j];
    pmthits[j] = 0;
  }
  datafileTest << "\t" << sum <<"\t"  << alone <<"\t" << totalRC << "\t" << totalRS  ;
  datafileTest << G4endl;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......ooOO0OOooo........oooOO0OOooo......


// Erase all the data for the next event.
void MdomAnalysisManager::Reset()
{
  atPhotocathode.clear();
  Decay.clear();
  allParticles.clear();
  NrCerenkovVec.clear();
  NrScintillationVec.clear();
  photonIds.clear();
  creationProcess.clear();  
}
