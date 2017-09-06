#include "mdomDetectorConstruction.hh"
#include "mdomPhysicsList.hh"
#include "mdomPrimaryGeneratorAction.hh"
#include "mdomRunAction.hh"
#include "mdomEventAction.hh"
#include "mdomTrackingAction.hh"
#include "mdomSteppingAction.hh"
#include "mdomSteppingVerbose.hh"
#include "mdomAnalysisManager.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4ThreeVector.hh"

#include "G4UItcsh.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"	//xxx

#include "argtable2.h"
#include <ctime>
#include <sys/time.h>

#include <sstream>
#include <iostream>
#include <fstream>
#include <string>

#include <cmath>	// for abs() of doubles
// #include "G4SystemOfUnits.hh"	// if compiling with Geant4.10

unsigned int	stats_buffer_max_size = 10;	// how many hits to keep in memory before purging to file in EndOfEventAction
unsigned int	stats_buffer_last_purge_at = 0;	// at what hits count was the hits file last written to
std::vector<G4int>	stats_PMT_hit;
std::vector<G4int>	stats_OM_hit;

G4double	gworldsize;
G4int		gsimevents;
G4String	ggunfilename;
G4String	ghitsfilename;
G4int		gPMT;
G4int		gEnvironment;
G4bool		gVisual;
G4bool		gInteractive;

// G4String	greffilename;

G4bool		gKillAll;
G4long		current_event_id;

struct timeval	gTime_Run_Start;
struct timeval	gTime_Run_End;
long randseed;

MdomAnalysisManager gAnalysisManager;

G4UImanager* UI;

void clearstats() {
	stats_PMT_hit.clear();
	stats_OM_hit.clear();
}

std::vector<G4String> explode(G4String s, char d) {
	std::vector<G4String> o;
	int i,j;
	i = s.find_first_of("#");
	if (i == 0) return o;
 	while (s.size() > 0) {
		i = s.find_first_of(d);
		j = s.find_last_of(d);
		o.push_back(s.substr(0, i));
		if (i == j) {
			o.push_back(s.substr(j+1));
			break;
		}
		s.erase(0,i+1);
 	}
	return o;// o beinhaltet s ohne d
}
std::vector<G4String> explode(char* cs, char d) {
	std::vector<G4String> o;
	G4String s = cs;
	return explode(s,d);
}
std::vector<double> readColumnDouble (G4String fn, int col) {
	std::vector<double>	values;
	unsigned int c;
	double	a;
	c = col;
	std::ifstream	infile;
	std::vector<G4String> n;
	char l[256];
	G4String l2;
	infile.open(fn);
	while (infile.good() && !infile.eof()) {
		infile.getline(l,255);
		l2 = l;
		n = explode(l2,'\t');
		if (n.size()>=c) {
			a = atof(n.at(c-1));
			values.push_back(a);
		}
	}
	infile.close();

	return values;//values enthält den c. Wert aus fn (aus jeder Spalte,welche  nach 255 zeichen oder durch \n beendet wird?)
}

int mdom() {
	struct timeval time_for_randy;
	gettimeofday(&time_for_randy, NULL);

	randseed = time_for_randy.tv_sec+4294*time_for_randy.tv_usec;

	CLHEP::HepRandom::setTheEngine(new CLHEP::RanluxEngine(randseed,3));
	std::stringstream command;

	G4RunManager* runManager = new G4RunManager;

	mdomDetectorConstruction* detector;
	detector = new mdomDetectorConstruction();
	runManager->SetUserInitialization(detector);

	G4VUserPhysicsList* physics = new mdomPhysicsList;
	runManager->SetUserInitialization(physics);

	#ifdef G4VIS_USE
 		G4VisManager* visManager = new G4VisExecutive;
 		visManager->Initialize();
 		visManager->SetVerboseLevel(0);
	#endif

	G4VUserPrimaryGeneratorAction* gen_action = new mdomPrimaryGeneratorAction();
	runManager->SetUserAction(gen_action);

	G4UserRunAction* run_action = new mdomRunAction();
	runManager->SetUserAction(run_action);

	G4UserEventAction* event_action = new mdomEventAction();
	runManager->SetUserAction(event_action);

 	G4UserTrackingAction* tracking_action = new mdomTrackingAction();
 	runManager->SetUserAction(tracking_action);

	G4UserSteppingAction* stepping_action = new mdomSteppingAction();
	runManager->SetUserAction(stepping_action);

	runManager->Initialize();

	UI = G4UImanager::GetUIpointer();

	command.str("");
	command << "/control/execute " << ggunfilename;
	UI->ApplyCommand(command.str());
	
	// Opens new user interface prompt after simulation was run
    int argumc = 1;
    char* argumv[] = {"dummy", NULL};

    G4UIExecutive* UIEx = new G4UIExecutive(argumc, argumv);
    //folgende zeile auskommentieren, dann keine visualisierung
    UI->ApplyCommand("/control/execute ../aux/init_vis.mac");
    UIEx->SessionStart();
    delete UIEx;

#ifdef G4VIS_USE
	delete visManager;
#endif

	delete runManager;
	return 0;
}

int main(int argc,char *argv[])
{
	struct arg_dbl  *worldsize	= arg_dbl0("wW", "world","<n>","\t\tradius of world sphere in m");
	struct arg_int  *events		= arg_int0("nN", "numevents,nevents","<n>","\tnumber of decays per run");
	struct arg_file *gunfile	= arg_file0("gG","gun","<file.txt>","\t\tfile containing GPS parameters");
	struct arg_int  *pmt		= arg_int0("pP", "pmt,PMT","<n>","\t\tPMT type [12199S, etel, 12199e]");
	struct arg_int  *environment= arg_int0("eE", "environment","<n>","\t\tmedium in which the setup is emmersed [AIR, ice, spice]");
	struct arg_file *outputfile	= arg_file0("oO","output","<file.txt>","\t\tfilename for hits data");
	struct arg_lit	*interactive = arg_lit0("iI","interact","\t\topens user interface after run");
	struct arg_lit	*visual		= arg_lit0("vV","visual","\t\tshows visualization of module after run");
	struct arg_lit	*help		= arg_lit0(NULL,"help","\t\tprint this help and exit");
	struct arg_end  *end		= arg_end(20);
	
	void* argtable[] = {worldsize, events, gunfile, pmt, environment, outputfile, interactive, visual, help, end};
	const char* progname = "mdom";
	int nerrors;
	int exitcode=0;

	// verify the argtable[] entries were allocated sucessfully
	if (arg_nullcheck(argtable) != 0) {
		/* NULL entries were detected, some allocations must have failed */
		printf("%s: insufficient memory\n",progname);
		exitcode=1;
		arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
		return exitcode;
	}

	// set any command line default values prior to parsing
	worldsize->dval[0] = 10.0;
	events->ival[0] = 0;
	gunfile->filename[0] = "mdom.gps";
	pmt->ival[0] = 0;	// use new R12199 as default
	environment->ival[0] = 0;	// use air as default
	outputfile->filename[0] = "mdom_testoutput.txt";

	/* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
	{
        printf("\nGEANT4 simulation of the mDOM: K40 decays\n");
        printf("\nUsage: %s", progname);
        arg_print_syntax(stdout,argtable,"\n");
        arg_print_glossary(stdout,argtable,"  %-25s %s\n");
        printf("\n");
        exitcode=0;
        goto hell;
	}

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
	{
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout,end,progname);
        printf("Try '%s --help' for more information.\n",progname);
        exitcode=1;
        goto hell;
	}

    /* special case: uname with no command line options induces brief help */
    if (argc==1)
	{
        printf("Try '%s --help' for more information.\n",progname);
        exitcode=0;
        goto hell;
	}

//	assign command-line arguments to variables:
	gworldsize = worldsize->dval[0];
	gsimevents = events->ival[0];
	ggunfilename = gunfile->filename[0];
	gPMT = pmt->ival[0];
	gEnvironment = environment->ival[0];
	ghitsfilename = outputfile->filename[0];
	if (interactive->count > 0) gInteractive = true; else gInteractive = false;
	if (visual->count > 0) gVisual = true; else gVisual = false;
// 	greffilename = waterref->filename[0];

	//	check params for sanity
	mdom();

hell:
    /* deallocate each non-null entry in argtable[] */
	arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
	//	return exitcode;

}

