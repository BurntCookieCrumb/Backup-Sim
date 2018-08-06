
// ~~ POSTPROCESSING ~~

// --------------------------------------------------------------------------------------
// Module for postprocessing results from a Simulation
//
// --------------------------------------------------------------------------------------

// == Includes ==

#include "postproc.h"

// == Namespace ==

using namespace std;

// --------------------------------------------------------------------------------------
// postprocessing
// --------------------------------------------------------------------------------------

void ppSim() {

    /** Reads in Files containing Data of different energies from Simulations with
        different tunes, extracts the corresponding histograms and uses these to derive
        Histograms meanpT <-> nCh, which will then be saved corresponding to energy.
    **/

    // ------ Define Program Specific Parameters ----------------------------------------

    Int_t order = 1; /// <-- MOMENT
    Char_t measure = 'Z'; /// M - moment, Z - central moment, K - cumulant

    TString generator = "PYTHIA8";

    const Int_t nTunes = 4;
    const Int_t nEnergies = 4;

    Double_t energies[nEnergies] = {5.02, 7., 8., 13.};
    Int_t tunes[nTunes] = {0, 6, 14, 21};

    // --------- Read in all Files ------------------------------------------------------

    TFile *inFiles[nEnergies][nTunes];
    TH1F *mult[nEnergies][nTunes];
    THnF *histograms[nEnergies][nTunes];
    Double_t nEvents[nEnergies][nTunes];

    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            inFiles[energy][tune] = new TFile(Form("./PYTHIA8/%dTeV/histo%d.root",
                                         (Int_t)energies[energy], tunes[tune]), "READ");

            // == Check if File was opened correctly ==

            if (!inFiles[energy][tune]) {
                    cout << "Error opening File "
                         << energies[energy] << " TeV histo" << tunes[tune] << endl;
                    exit(0);
                }


            // == Get Histograms ==

            mult[energy][tune] = (TH1F*) inFiles[energy][tune]->FindObjectAny("mult");

            if(!mult[energy][tune]){
                 cout << "Error opening histogram mult " << energies[energy]
                      << " TeV/ Tune " << tunes[tune]<< endl;
            }

            nEvents[energy][tune] = mult[energy][tune]->GetEntries();


            // histograms with
            //         0:mult 1:ID 2:pT(GeV) 3:eta
            histograms[energy][tune] = (THnF*) inFiles[energy][tune]->
                                                                    FindObjectAny("hn");

            if(!histograms[energy][tune]){
                 cout << "Error opening histogram hn " << energies[energy]
                      << " TeV/ Tune " << tunes[tune]<< endl;
            }

        }
    }

    // ---------- Set Ranges ------------------------------------------------------------

    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            // == eta ==

            histograms[energy][tune]->GetAxis(3)->SetRangeUser(-0.8,0.8);

            // == pT ==

            histograms[energy][tune]->GetAxis(2)->SetRangeUser(0.15, 9.99);

        }
    }

    // ------ Getting Mean Spectra ------------------------------------------------------

    TH1D* meanpT[nEnergies][nTunes];

    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            // == Initialize meanpT Histogram for each energy and each tune ==

            meanpT[energy][tune] = new TH1D(Form
              ("meanpT%d/%d", (int)energies[energy], tunes[tune]), "", 200, -0.5, 199.5);

            meanpT[energy][tune]->Sumw2();


            // == Fill said Histograms ==


            switch (measure) {

            case '0': GetMeanpT(meanpT[energy][tune], histograms[energy][tune], 2, 0, 200);
                      break;

            default : GetMomentHist(meanpT[energy][tune], histograms[energy][tune], 2, 0,
                                    measure, order, 0.15, 9.99);
                      break;

            }

        }
    }

    // ------------ Output --------------------------------------------------------------

    TFile* outSim = new TFile((TString(Form("Output/OutSim%c%d", measure, order))
                                      .Append(generator)).Append(".root"), "RECREATE");


    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            meanpT[energy][tune]->                           /// Set number of entries to
                         SetEntries(nEvents[energy][tune]);  /// number of events for
                                                             /// convenience

            meanpT[energy][tune]->Write(Form("meanpT%d/%d",
                                                  (int)energies[energy], tunes[tune]));

        }
    }

    // ----- Cleaning Up ----------------------------------------------------------------


    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            inFiles[energy][tune]->Close();

        }
    }

    outSim->Close();

}
























