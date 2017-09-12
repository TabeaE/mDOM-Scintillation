#include "mdomPMT.hh"
#include "G4ios.hh"
//since Geant4.10: include units manually
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "mdomAnalysisManager.hh"
#include <sstream>
#include <string>
#include <algorithm>
#include <math.h>  
#include <iterator>
#include <vector>
#include <iostream>

extern MdomAnalysisManager gAnalysisManager;
extern G4int gPMT;
extern G4int gQE;
extern G4int gDOM;
extern std::vector<double> readColumnDouble (G4String fn, int col);

bool sortByTime(const MdomAnalysisManager::photonHit & x, const MdomAnalysisManager::photonHit & y) {
  return x.stats_hit_time < y.stats_hit_time;
}
bool compareTime(const MdomAnalysisManager::photonHit & S_0 , const MdomAnalysisManager::photonHit & S_1) {
  return S_0.stats_hit_time < S_1.stats_hit_time;
}

bool compareTimeDouble(const G4double & S_0 , const G4double & S_1) {
  return S_0 < S_1;
}

mdomPMT::mdomPMT(){
  DecayMode = false;
  
}


mdomPMT::~mdomPMT(){
  
}

//#########################################################################################################################

void mdomPMT::loadThePMTInfo(){
  
  deadTime= 8*ns;
  OsciThreshold = 7; // in mV
  
  if (gDOM == 1){
    QEfile = "../QEFiles/DOM.cfg";
    G4cout << "Time Response and QE for Hamamatsu R7081-02 (DOM)" << G4endl;
    TransitTime = 62*ns;
    TTS = 3.4;
    detectionProbability = 0.0;                               // Probability that the photon will not be registered.
  }
  
  else if (gPMT == 0){
    QEfile = "../QEFiles/Hamamatsu12199-02.cfg";
    G4cout << "Time Response and QE for Hamamatsu 12199" << G4endl;
    TransitTime = 0.0*43*ns;
    TTS =  0.0*2.542;
    
    detectionProbability = 1.0;                               // Probability that the photon will not be registered.
  }
  else if(gPMT == 1){
    QEfile = "../QEFiles/ETEL.cfg"; 
    G4cout << "Time Response and QE for ETEL " << G4endl;
    TransitTime = 47*ns;
    TTS = 4.5;    
    detectionProbability = 0.1;                               // Probability that the photon will not be registered.
    
  }
  else{
    G4cout << ":::::::::::::::Warning: There is no Time response information for the selected PMT:::::::::::::::" << G4endl;
  }
  
  
  if (gQE==1) {
    PEfile = "../QEFiles/1PE.txt"; 
    QEwavelenght= readColumnDouble(QEfile, 1);
    QEprobability= readColumnDouble(QEfile, 2);  
    PEtime= readColumnDouble(PEfile, 1);
    PEamp= readColumnDouble(PEfile, 2);  
    
    for (unsigned int u = 0; u <QEwavelenght.size(); u++) {
      QEwavelenght[u] = QEwavelenght.at(u)*nm;
      QEprobability[u] = QEprobability.at(u)/100.;
    }
    for (unsigned int u = 0; u <PEtime.size(); u++) {
      PEtime[u] = PEtime.at(u)*ns;
      PEamp[u] = PEamp.at(u);
    }
  }
}



//#########################################################################################################################


// Cristian's QE function
bool mdomPMT::QEcheck(G4double lambda) {
  if ((gQE==1)) {
    if (!QEwavelenght.empty()) {
      probability = 0;
      if (lambda < QEwavelenght.at(0) || lambda > QEwavelenght.back()) {
	return false;
      } else {
	G4bool boolparameter = false;
	for (unsigned int u = 0; u <= (QEwavelenght.size()-1); u++) {
	  if (QEwavelenght.at(u) == lambda) {
	    probability = QEprobability.at(u);
	    boolparameter = true;
	  }
	  else if (QEwavelenght.at(u) > lambda) {
	    G4double slope = (QEprobability.at(u)-QEprobability.at(u-1))/(QEwavelenght.at(u)-QEwavelenght.at(u-1));
	    probability = (slope*(lambda-QEwavelenght.at(u-1))+QEprobability.at(u-1));
	    boolparameter = true;
	  }
	  if (boolparameter){
	    G4double rand = G4UniformRand();
	    //G4cout << "wavelenght -> " << lambda/nm << "  points -> " << QEwavelenght.at(u-1)/nm << " & " << QEwavelenght.at(u)/nm << "  points Prob-> " << QEprobability.at(u-1) << " & " << QEprobability.at(u) << "  probabiliy -> " <<probability << "  random "<< rand <<G4endl;  
	    if (rand< probability) {
	      //G4cout << "PARTY" << G4endl;
	      return true;
	    } else {
	      return false;
	    }
	  }
	}
      }
    } else {
      G4cout << "ERROR!!! -> Check Quantum efficiency function or data" << G4endl;
      return 0;
    }
  }
}




//#########################################################################################################################
//#########################################################################################################################
//#########################################################################################################################


void mdomPMT::addTTS(std::vector<MdomAnalysisManager::photonHit>& HitVector){
  for (int i = 0; i < (int) (HitVector.size()); i++) {
    G4double randomTime = TTS*sqrt(-2.*log(G4UniformRand()))*cos(2*3.14159265358979323846*(G4UniformRand()));
    HitVector.at(i).stats_hit_time =  HitVector.at(i).stats_hit_time+randomTime*ns+TransitTime;
    
  }
}

//#########################################################################################################################
void mdomPMT::eraseFirstTime(std::vector<MdomAnalysisManager::photonHit>& HitVector){
  G4bool iSawTheFirstTime = false;
  G4double smallestTime;
  for (int i = 0; i < (int) HitVector.size(); i++) {
    if(HitVector.at(i).realHit==1 && iSawTheFirstTime){
      HitVector.at(i).stats_hit_time = HitVector.at(i).stats_hit_time-smallestTime/ns;
    }
    if(HitVector.at(i).realHit==1 && !iSawTheFirstTime){
      smallestTime =  HitVector.at(i).stats_hit_time;
      HitVector.at(i).stats_hit_time = 0.*ns;
      iSawTheFirstTime = true;
    }
  }
}
//#########################################################################################################################

//This function makes a list of the mother nucleus that caused the hit.
void mdomPMT::MotherFinder(std::vector<MdomAnalysisManager::photonHit>& HitVector, std::vector<MdomAnalysisManager::particle>& particles)
{ G4int numberofHits = HitVector.size();
  
  if (DecayMode){
    for (int i = 0; i < (int) numberofHits; i++) {      
      for (int j = 0; j < (int) particles.size(); j++){
	if (HitVector.at(i).photonParent == particles.at(j).particlesIDs){
	  MOTHERFINDER2:
	  if((particles.at(j).particlesType != "nucleus")||(particles.at(j).particlesNames == "alpha"))
	  {
	    for(int k = 0; k < (int) particles.size(); k++) 
	    {
	      if (particles.at(k).particlesIDs == particles.at(j).parentParticlesIDs)
	      {
		j=k;
		goto MOTHERFINDER2;
	      }
	    }
	  }
	  else
	  {
	    HitVector.at(i).hitMotherName = particles.at(j).particlesNames;
	    break;
	  }
	  
	}
      }
    }   
  }
}

//#########################################################################################################################

// The function HitKiller will erase all hits that arrived to the PMT in the pulse-pileup-time given by the variable deadTime. 
// It also erase a certain percentage (given by the G4double "detectionProbability") of the individual hits (hits that do not have a "temporal neighbour") using the function triggerAcceptanceKiller.
void mdomPMT::HitKiller(std::vector<MdomAnalysisManager::photonHit>& allHits)
{  gAnalysisManager.NrAlone=0;
  
  for (int i = 0; i < (int) (allHits.size()-1); i++) {
    for (int j = i+1; j < (int) allHits.size(); j++){
      //if ((allHits.at(i).stats_PMT_hit == allHits.at(j).stats_PMT_hit) && (allHits.at(i).hitMotherName == allHits.at(j).hitMotherName) && ((allHits.at(j).stats_hit_time-(allHits.at(i).stats_hit_time))<deadTime)){	
      if ((allHits.at(j).stats_hit_time-(allHits.at(i).stats_hit_time))<deadTime){
	allHits.at(j).realHit=0;
      }
    }
  }
  
  for (int i = 0; i < (int) (allHits.size()-1); i++) {
    G4int AccumulatedPhotons = 1;
    if ((allHits.at(i).realHit == 1)&&(allHits.at(i+1).realHit == 0)){
      
      for (int j = i+1; j < (int) allHits.size(); j++){
	
	if (allHits.at(j).realHit == 0){
	  AccumulatedPhotons++;
	  if (j==allHits.size()-1){
	    allHits.at(i).Amplitude = AccumulatedPhotons;
	  }
	}
	else {
	  
	  allHits.at(i).Amplitude = AccumulatedPhotons;
	  break;
	}
	
	
      }
    }
    else if ((allHits.at(i).realHit == 1) ){
      if ((triggerAcceptanceKiller())){
	allHits.at(i).Amplitude = 1;
      }
      else{
	allHits.at(i).Amplitude = 0;
	allHits.at(i).realHit = -1;
      }
    }
  }
  
  
  if (allHits.size()>1){
    if ((allHits.at(allHits.size()-1).realHit == 1) && (triggerAcceptanceKiller())) {
      allHits.at(allHits.size()-1).Amplitude = 1;
    }
    else if (allHits.at(allHits.size()-1).realHit == 1){
      allHits.at(allHits.size()-1).Amplitude = 0;
      allHits.at(allHits.size()-1).realHit = -1;
    }
    else {
      allHits.at(allHits.size()-1).Amplitude = 0;
    }
  }
}

//#########################################################################################################################
// This function adds up the hits,  separating them by their creationProcess.
void mdomPMT::HitsProcessCounter(G4int& CerenkovCounter, G4int& ScintCounter, std::vector<G4int>& photonIds, std::vector<G4String>& creationProcess , std::vector<MdomAnalysisManager::photonHit>& allHits )
{ for (int i = 0; i < (int) allHits.size(); i++) {
  if(allHits.at(i).realHit ==1 || allHits.at(i).realHit ==2){
    G4int myindex = find(photonIds.begin(), photonIds.end(),allHits.at(i).hitPhotonID)- photonIds.begin(); 
    if (creationProcess.at(myindex) =="c"){
      CerenkovCounter++;}
      if (creationProcess.at(myindex) =="s"){
	ScintCounter++;}
  }
}  
}

//#########################################################################################################################

//Evaluate the photons as waveform and check if they are measured as a real hits.
void mdomPMT::WaveAmplitudeHitKiller( std::vector<MdomAnalysisManager::photonHit>& allHits){ 
  HitKiller(allHits); //Evaluate if the hits are real, not detected because of the threshold acceptance or ot detected because of the ToT.

  if (allHits.size()>1){ //gToT==1 //If you have more than one hit
    for ( int i = 0; i < (allHits.size()-1); i++) { //for every possible hit
      if (allHits.at(i).realHit == 1 && allHits.at(i).Amplitude > 1){// If the Hit is real and it has some photons inside the dead time (neighbour events)
	
	std::vector<G4double> WaveTime;
	std::vector<G4double> HitTimes;
	std::vector<G4double> WaveAmp;
	std::vector<G4double> AmplitudeArray;
	G4int OriginalAmplitude = allHits.at(i).Amplitude;
	G4bool thishappenedonce = false;
	
	
	for (int j = i; j < allHits.at(i).Amplitude +i ; j++){ //for every neighbour
	  HitTimes.push_back(allHits.at(j).stats_hit_time);
	  for (int item = 0; item < PEtime.size(); item++){
	    WaveTime.push_back(PEtime.at(item)+allHits.at(j).stats_hit_time); //Make 1 PE peak in hit time
	    WaveAmp.push_back(PEamp.at(item));
	    
	  }
	} 
	
	G4double time_min =  *min_element(WaveTime.begin(),WaveTime.end()); // search for the smallest time value
	G4double time_max = *max_element(WaveTime.begin(),WaveTime.end()); //searches for the largest time value
	G4double spacing = 0.2; // The 1 PE data file has time values with 0.2 ns spacing...change it if you use another file! (Maybe improve the code, so it searches for the spacing at the beginning of the run)
	
	for (int k = 0; k <= round((time_max-time_min)/0.2); k++){
	  AmplitudeArray.push_back(0);                             // it makes a vector with the needed dimension
	}
	
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
	//:::::::::::. Sum every 1 PE peak into a single array (kinda like an Histogram):::::::::
	for ( int k = 0; k < WaveTime.size(); k++){   
	  G4int index = round((WaveTime.at(k)-time_min)/spacing);
	  G4double old = AmplitudeArray.at(index); 
	  AmplitudeArray.at(index) = old + WaveAmp.at(k); 
	}  
	//::::::::::::::::::::::::::: Here you have already a Waveform, do wathever you want with it. You can make the time array with k*spacing*ns+time_min, where k is an index that goes from 0 to AmplitudeArray.size():::::::::::::::::::
	//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::


	G4bool UnderThreshold = true; // The Waveform start at 0, so the signal is under the threshold
	G4bool firstTime = true; // First hit is always real
	
	for ( int k = 0; k < (int) AmplitudeArray.size(); k++){ //Go through the Waveform     
	  
	  if (AmplitudeArray.at(k) < OsciThreshold && UnderThreshold == false ){ //The signal is lower than the threshold level, and it is not the beginning of the waveform
	    UnderThreshold = true;
	  }
	  if (AmplitudeArray.at(k) > OsciThreshold && UnderThreshold == true && firstTime == false){//The signal was under the threshold, but now again over it.

	    MdomAnalysisManager::photonHit dummy;
	    G4double doubledummy;
	    dummy.stats_hit_time = k*spacing*ns+time_min;
	    doubledummy= k*spacing*ns+time_min;
	    
	    G4int myindex = lower_bound(allHits.begin(), allHits.end(), dummy, compareTime)- allHits.begin(); // Get the index of the first hit that we have after the signal was over the threshold
	    G4int myindex2 = lower_bound(HitTimes.begin(), HitTimes.end(), doubledummy, compareTimeDouble)- HitTimes.begin(); 
	    
	    allHits.at(myindex).realHit = 2; // This is a real hit now!
	    allHits.at(myindex).Amplitude = OriginalAmplitude - myindex2+1;
	    

	    
	    if (!thishappenedonce){ 
	      allHits.at(i).Amplitude = myindex2;
	    }
	    
	    
	    thishappenedonce = true;
	    UnderThreshold = false; 
	  }
	  if (AmplitudeArray.at(k) > OsciThreshold && UnderThreshold == true && firstTime == true){ // Change the conditional after the first part of the signal is over the threshold
	    firstTime = false;
	    UnderThreshold = false;
	  }
	  
	}
	

	for (int j = i+1; j < OriginalAmplitude +i-1 ; j++){ 	  
	  if (allHits.at(j).realHit == 2){

	    for (int k = j+1; k < OriginalAmplitude +i ; k++){

	      if (allHits.at(k).realHit == 2){
		allHits.at(j).Amplitude = allHits.at(j).Amplitude -allHits.at(k).Amplitude;
		break;
	      }
	      
	    }
	  }
	}

	if (allHits.at(OriginalAmplitude +i-1).realHit == 2){	  
	  allHits.at(OriginalAmplitude+i-1).Amplitude =1;
	}

      }
    }
  }

}


bool mdomPMT::triggerAcceptanceKiller()
{ 
  G4double rand = G4UniformRand();
  if (rand < detectionProbability){
    return false;
  }
  else{
    return true;
  }
}

//#########################################################################################################################

// This function runs all the functions above. 
void mdomPMT::Analysis() {
  
  
  gAnalysisManager.totalC=0;
  gAnalysisManager.totalS=0;	
  gAnalysisManager.totalRC=0;
  gAnalysisManager.totalRS=0;	
  
  addTTS(gAnalysisManager.atPhotocathode);
  
  sort(gAnalysisManager.atPhotocathode.begin(), gAnalysisManager.atPhotocathode.end(), sortByTime);
  
  eraseFirstTime(gAnalysisManager.atPhotocathode);
  
  MotherFinder(gAnalysisManager.atPhotocathode, gAnalysisManager.allParticles);
  
  HitsProcessCounter(gAnalysisManager.totalC, gAnalysisManager.totalS, gAnalysisManager.photonIds, gAnalysisManager.creationProcess , gAnalysisManager.atPhotocathode);

  WaveAmplitudeHitKiller(gAnalysisManager.atPhotocathode);

  
  HitsProcessCounter(gAnalysisManager.totalRC, gAnalysisManager.totalRS, gAnalysisManager.photonIds, gAnalysisManager.creationProcess , gAnalysisManager.atPhotocathode);
  
}
//#########################################################################################################################
//#########################################################################################################################

