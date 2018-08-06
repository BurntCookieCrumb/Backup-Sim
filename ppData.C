
// ~~ POSTPROCESSING ~~

// --------------------------------------------------------------------------------------
// Module for postprocessing results from Data
//
// --------------------------------------------------------------------------------------

// == Includes ==

#include "postproc.h"

// == Namespace ==

using namespace std;

// --------------------------------------------------------------------------------------
// postprocessing
// --------------------------------------------------------------------------------------

void ppData() {

    /** Reads in Files containing Data of different energies from Experiments,
        extracts the corresponding histograms and uses these to derive
        Histograms meanpT <-> nCh, which will then be saved corresponding to energy.
    **/

    // ------ Define Program Specific Parameters ----------------------------------------

    Int_t order = 1; /// <-- ORDER
    Char_t measure = 'Z'; /// M - moment, Z - central moment, K - cumulant

    const Int_t nEnergies = 4;
    Double_t energies[nEnergies] = {5.02, 7., 8., 13.};

    // --------- Read in all Files ------------------------------------------------------

    TFile *inData;
    TH2D *data[nEnergies];

    inData = new TFile("./Daten/ptSpectraUnfolded.root", "READ");

    for (int energy = 0; energy < nEnergies; energy++){

        data[energy] = (TH2D*) inData->
            FindObjectAny(Form("multPtUnfolded_pp_%dTeV_eta08",(int)energies[energy]));

        if(!data[energy]) cout << "Error opening histogram data " << energies[energy]
                                                                      << " TeV"<< endl;

    }

    // ---------- Set Ranges ------------------------------------------------------------

    for (int energy = 0; energy < nEnergies; energy++){

        data[energy]->GetYaxis()->SetRangeUser(0.15, 9.99);

    }

    // ------ Getting Mean Spectra ------------------------------------------------------

    TH1D* meanpT_data[nEnergies];

    for (int energy = 0; energy < nEnergies; energy++){

        // == Initialize meanpT Histogram for each energy and each tune ==

        meanpT_data[energy] = new TH1D(Form("meanpT_data%d", (int)energies[energy]),
                                                                "", 200, -0.5, 199.5);

        meanpT_data[energy]->Sumw2();

        // == Fill said Histograms ==

        switch (measure) {

            case '0': GetMeanpT(meanpT_data[energy], data[energy], 200);
                      break;
            default : GetMomentHist(meanpT_data[energy], data[energy], measure, order,
                                    0.15, 9.99);
                      break;


        }

    }

    // ------------ Output --------------------------------------------------------------

    TFile* outData = new TFile(TString(Form("Output/OutData%c%d", measure, order))
                                                         .Append(".root"), "RECREATE");

    for (int energy = 0; energy < nEnergies; energy++){

        meanpT_data[energy]->Write(Form("meanpT_data%d", (int)energies[energy]));

    }

    // --------- Cleaning Up ------------------------------------------------------------

    inData->Close();
    outData->Close();

}































