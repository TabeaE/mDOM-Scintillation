#ifndef mdomPMT_h
#define mdomPMT_h 1

#include "G4Types.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include <algorithm>
#include <vector>
#include <fstream>
#include "mdomAnalysisManager.hh"

class mdomPMT
{
public:
  mdomPMT();
  ~mdomPMT();
  bool QEcheck(G4double lambda);
  void Analysis();
  void loadThePMTInfo();
private:
  void quicksort(std::vector<MdomAnalysisManager::photonHit>& array, int start, int end);
  int divide(std::vector<MdomAnalysisManager::photonHit>& array, int start, int end) ;
  void HitKiller(std::vector<MdomAnalysisManager::photonHit>& allHits);
  bool triggerAcceptanceKiller();
  void MotherFinder(std::vector<MdomAnalysisManager::photonHit>& HitVector, std::vector<MdomAnalysisManager::particle>& particles);
  void HitsProcessCounter(G4int& CerenkovCounter, G4int& ScintCounter, std::vector<G4int>& , std::vector<G4String>& creationProcess , std::vector<MdomAnalysisManager::photonHit>& allHits);
  void addTTS(std::vector<MdomAnalysisManager::photonHit>& HitVector);
  void eraseFirstTime(std::vector<MdomAnalysisManager::photonHit>& HitVector);
  void WaveAmplitudeHitKiller(std::vector<MdomAnalysisManager::photonHit>& allHits);

  G4String QEfile;
  G4String PEfile;
  G4double probability;
  G4double deadTime;
  
  G4double detectionProbability;
  G4double TTS;
  G4double TTSmean;
  G4double TTSsigma;
  G4double TransitTime;
  G4bool flag;
  std::vector<double> QEwavelenght;
  std::vector<double> QEprobability;
  std::vector<double> PEtime;
  std::vector<double> PEamp;
  G4bool DecayMode;
  G4double OsciThreshold;

  
  
  
};
  bool sortByTime( const MdomAnalysisManager::photonHit& x, const MdomAnalysisManager::photonHit& y);
  bool compareTime(const MdomAnalysisManager::photonHit& S_0 , const MdomAnalysisManager::photonHit& S_1); 
  bool compareTimeDouble(const G4double & S_0 , const G4double & S_1);

#endif
