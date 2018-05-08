// main91.cc is a part of the PYTHIA event generator.
// Copyright (C) 2017 Torbjorn Sjostrand.
// PYTHIA is licenced under the GNU GPL version 2, see COPYING for details.
// Please respect the MCnet Guidelines, see GUIDELINES for details.

// This is a simple test program.
// It studies the charged multiplicity distribution at the LHC.
// Modified by Rene Brun, Axel Naumann and Bernhard Meirose
// to use ROOT for histogramming.

// Stdlib header file for input and output.
#include <iostream>

// Header file to access Pythia 8 program elements.
#include "Pythia8/Pythia.h"

// ROOT, for histogramming.
#include "TH1.h"
#include "TH3.h"
#include "THn.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"

// ROOT, for saving file.
#include "TFile.h"

#include "TStyle.h"

using namespace Pythia8;


int addToArray(int *pointy, int id){

	int i;

	for(i = 0; pointy[i] != 0; i++){
		if(pointy[i] == id) return 0;
	}

	//std::cout << i << std::endl;
	pointy[i++] = id;
	pointy[i] = 0;
	return 1;

}

void printArray(int *pointy){

	for(int i = 0; pointy[i] != 0; i++){
		std::cout << pointy[i] << " ";
	}

	std::cout << std::endl;

}

int getNewID(int id){

  switch (id) {

    case -2212: return -5;
    case  2212: return  5;
    case - 321: return -4;
    case + 321: return  4;
    case - 211: return -3;
    case   211: return  3;
    case -  13: return -2;
    case +  13: return  2;
    case -  11: return -1;
    case    11: return  1;

  }

return 0;

}

// -----------------------------------------------------------------------------------------------

int main(int argc, char* argv[]) {


  // Create Pythia instance and set it up to generate hard QCD processes
  // above pTHat = 20 GeV for pp collisions at 14 TeV.
  Pythia pythia;

  // settings
  pythia.readFile("sim.cmnd");

  // Extract settings to be used in the main program.
  int nEvent = pythia.mode("Main:numberOfEvents");

  /*
  pythia.readString("SoftQCD:inelastic = on"); //SoftQCD:inelastic = on
  pythia.readString("Beams:eCM = 14000.");

  pythia.readString("Random:setSeed = on");
  pythia.readString("Random:seed = 0");

  pythia.readString("Tune:pp = 14");

  */

  pythia.init();

  // Create file on which histogram(s) can be saved.
  TFile* outFile = new TFile(Form("%s.root", argv[1]), "RECREATE");

  // Book histogram.
  TH1F *mult = new TH1F("mult", "charged multiplicity", 100, -0.5, 799.5);
  mult->Sumw2();
  TH1F *dndpT = new TH1F("dndpT", "pT for charged particles", 100, 0., 10.);
  dndpT->Sumw2();

  TH1F *ID = new TH1F("ID", "particle IDs", 11, -5.5, 5.5);
  ID->Sumw2();

  (ID->GetXaxis())->SetBinLabel(1, "p");
  (ID->GetXaxis())->SetBinLabel(2, "#pi^{+}");

  /*
  TH1F *prtcls = new TH1F("prtcls", "particles", 8000, -20.5, 20.5 );
  int particles[100];
  */

  Double_t binsPtDefault[69]  ={0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.2,2.4,2.6,2.8,3.0,3.2,3.4,3.6,3.8,4.0,4.5,5.0,5.5,6.0,6.5,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0,18.0,20.0,22.0,24.0,26.0,28.0,30.0,32.0,34.0,36.0,40.0,45.0,50.0};
  Double_t binsEtaDefault[31] = {-1.5,-1.4,-1.3,-1.2,-1.1,-1.0,-0.9,-0.8,-0.7,-0.6,-0.5,-0.4,-0.3,-0.2,-0.1,0.,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5};
  Double_t binsZvDefault[13] = {-30.,-25.,-20.,-15.,-10.,-5.,0.,5.,10.,15.,20.,25.,30.};

  Int_t bins[4]   = {400, 11, 68, 30}; // mult ID pT eta
  Double_t min[4] = {-0.5, -5.5, 0., -1.5};
  Double_t max[4] = {399.5, 5.5, 50., 1.5};

  THnF *hn = new THnF("hn", "mult ID pT eta", 4, bins, min, max);
  hn->Sumw2();

  hn->SetBinEdges(2, binsPtDefault);
  hn->SetBinEdges(3, binsEtaDefault);

  hn->GetAxis(0)->SetTitle("multiplicity");
  hn->GetAxis(1)->SetTitle("ID");
  hn->GetAxis(2)->SetTitle("pT");
  hn->GetAxis(3)->SetTitle("#eta");

  hn->GetAxis(1)->SetBinLabel(1, "p");
  hn->GetAxis(1)->SetBinLabel(2, "#pi^{+}");

  int nCharged;

  // Begin event loop. Generate event; skip if generation aborted.
  for (int iEvent = 0; iEvent < nEvent; ++iEvent) {
    if (!pythia.next()) continue;

    nCharged = 0;


    // multiplicity for each event

    for(int i = 0; i < pythia.event.size(); ++i){
        //if (pythia.event[i].eta() > TMath::Abs(0.8))
        if (pythia.event[i].isFinal() && pythia.event[i].isCharged()) {

	    ++nCharged; // add to multiplicity

        }
    }


    for (int i = 0; i < pythia.event.size(); ++i)
      if (pythia.event[i].isFinal() && pythia.event[i].isCharged()){

        /* ~~ get particles ~~
	prtcls->Fill(pythia.event[i].id());
	addToArray(particles, pythia.event[i].id());
	*/

	Double_t test[4] = {nCharged, getNewID(pythia.event[i].id()), pythia.event[i].pT(), pythia.event[i].eta()};
	hn->Fill(test);
	ID->Fill(getNewID(pythia.event[i].id()));

	}

	mult->Fill(nCharged);

  }

  // Statistics on event generation.
  pythia.stat();

  // Show histogram. Possibility to close it.
  //mult->Draw();
  //dndpT->Draw();
  //ID->Draw();

 /*
  gStyle->SetOptStat(111111);
  prtcls->SetFillColor(kRed);
  prtcls->Draw();
 */

  //printArray(particles);

  // Save histogram on file and close file.
  hn->Write();
  mult->Write();
  delete outFile;

  // Done.
  return 0;
}

/* ~~**  **~~

	// Fill transverse momentum in event loop
        dndpT->Fill( pythia.event[i].pT() );

        // Fill charged multiplicity in histogram. End event loop.
        mult->Fill( nCharged );

	// h3->Fill(nCharged, getNewID(pythia.event[i].id()), pythia.event[i].pT());

*/


