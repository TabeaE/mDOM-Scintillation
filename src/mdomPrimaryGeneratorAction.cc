#include "mdomPrimaryGeneratorAction.hh"
#include "mdomDetectorConstruction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTypes.hh"

extern double gworldsize;


mdomPrimaryGeneratorAction::mdomPrimaryGeneratorAction()
{
	particleSource = new G4GeneralParticleSource ();
	particleSource->SetParticleDefinition(G4GenericIon::GenericIonDefinition());
}

mdomPrimaryGeneratorAction::~mdomPrimaryGeneratorAction()
{
	delete particleSource;
}

void mdomPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	particleSource->GeneratePrimaryVertex(anEvent);
}
