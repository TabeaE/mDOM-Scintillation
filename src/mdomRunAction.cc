#include "mdomRunAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"

#include <ctime>
#include <sys/time.h>

#include "mdomAnalysisManager.hh"
#include <time.h>
#include <sys/time.h>


extern G4String	ghitsfilename;
extern G4String	gHittype;
extern MdomAnalysisManager gAnalysisManager;


mdomRunAction::mdomRunAction(){}
mdomRunAction::~mdomRunAction(){}

void mdomRunAction::BeginOfRunAction(const G4Run*) 
{	
  startingtime= clock() / CLOCKS_PER_SEC;
  // 	Open output data file
  // 	gAnalysisManager.datafile.open("../ana/data.txt".c_str(), std::ios::out|std::ios::out| std::ios_base::trunc);//out: zum schreiben, trunc: inhalt vor öffnen wird gelöscht
  
  if (gHittype == "onlyHits") {
    G4String name3= ghitsfilename;
    G4String name2= "Waves";
    gAnalysisManager.datafileTest.open(name3.c_str(), std::ios::out|std::ios::app);
    gAnalysisManager.datafileMother.open(name2.c_str(), std::ios::out|std::ios::app);
  }
  
  if (gHittype == "detailed") {
    G4String name1= ghitsfilename;
    G4String name2= "Waves";
    G4String name3= "Hits-"+ghitsfilename;
    gAnalysisManager.datafile.open(name1.c_str(), std::ios::out|std::ios::app);
    gAnalysisManager.datafileMother.open(name2.c_str(), std::ios::out|std::ios::app);
    gAnalysisManager.datafileTest.open(name3.c_str(), std::ios::out|std::ios::app);
  }
  
  
}

void mdomRunAction::EndOfRunAction(const G4Run*)
{
  
  // 	Close output data file
  if (gHittype == "detailed") {
  gAnalysisManager.datafile.close();
  //gAnalysisManager.datafileMother.close();
  gAnalysisManager.datafileTest.close();
  gAnalysisManager.datafileMother.close();
  }
  if (gHittype == "onlyHits") {
    gAnalysisManager.datafileTest.close();
     gAnalysisManager.datafileMother.close();
    }
  gAnalysisManager.Reset();
  
  
  //   G4cout <<  gAnalysisManager.OutOfWorldCounter << G4endl;
  double finishtime=clock() / CLOCKS_PER_SEC;
  G4cout << "Computation time: " << finishtime-startingtime << " seconds." << G4endl;
}

