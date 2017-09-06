#ifndef mdomAnalysisManager_h
#define mdomAnalysisManager_h 1

#include "G4Types.hh"
#include "G4String.hh"
#include "G4ThreeVector.hh"
#include <algorithm>
#include <vector>
#include <fstream>

class MdomAnalysisManager
{
public:
  MdomAnalysisManager();
  ~MdomAnalysisManager();
  
  void Reset();
  void WriteDetailPhotons();
  void WriteMotherDecay();
  void WriteAccept();
  void ResetEvent();
  
  
  // run quantities
  G4String outputFilename;
  std::fstream datafile;

  G4long current_event_id;
  
  std::fstream datafileMother;
  std::fstream datafileTest;
  
  G4bool foundDecay;
  G4String MotherName;
  
  
  G4int NrScintillation;
  G4int NrCerenkov;
  G4int decayModus;
  
  
  G4int totalC;
  G4int totalS;
  G4int totalRC;
  G4int totalRS;
  G4int NrAlone;
  
  
  G4bool SaveThisEvent;
  G4int OutOfWorldCounter;
  
  G4double energyDeposited;
  
  struct photonHit {
    G4long stats_event_id;
    G4int hitPhotonID;
    G4int photonParent;
    G4int realHit;
    G4double	stats_hit_time;
    G4double	stats_photon_flight_time;
    G4double	stats_photon_energy;
    G4int	stats_PMT_hit;
    G4ThreeVector	stats_photon_direction;
    G4ThreeVector	stats_photon_position;
    G4String	hitMotherName;
    G4int	Amplitude;
  };
  
  struct particle{
    G4int particlesIDs;
    G4String particlesNames;
    G4String particlesType;
    G4int parentParticlesIDs;
    //G4long	parents_event_id;
  };
  
  struct decayIsotope{
    G4long	NewMother_event_id;
    G4double decayTheta;
    G4double decayPhi;
    G4double decayR;
  };
  
  
  //std::find() did not work with a struct with all the photons...
  // it would have only two members anyway, so it is not a big problem
  std::vector<G4int> photonIds;
  std::vector<G4String> creationProcess;  
  
  std::vector<G4int> NrScintillationVec;
  std::vector<G4int> NrCerenkovVec;
  
  //struc vectors:
  std::vector<particle> allParticles;
  std::vector<photonHit> atPhotocathode;
  std::vector<decayIsotope> Decay;
  
private:
  
};

#endif
