#ifndef mdomPrimaryGeneratorAction_h
#define mdomPrimaryGeneratorAction_h 1
 
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
 
class G4GeneralParticleSource;
class G4Event;
class mdomPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
	mdomPrimaryGeneratorAction();
	~mdomPrimaryGeneratorAction();

public:
	void GeneratePrimaries(G4Event* anEvent);

private:
	G4GeneralParticleSource* particleSource;
};

#endif
