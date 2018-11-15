
// ~~ POSTPROCESSING ~~

// --------------------------------------------------------------------------------------
// Create Plots for comparing the performance of different tunes with respect to the
// Data by using the Ratio as starting point
// --------------------------------------------------------------------------------------

// == Includes ==

#include "../Plotting.h"

#include <typeinfo>

// == Namespace ==

using namespace std;

// --------------------------------------------------------------------------------------
// postprocessing
// --------------------------------------------------------------------------------------

void comp(){

    TString gap = " ";

    // ------ Define Program Specific Parameters ----------------------------------------


    Char_t measure = 'M'; /// M - moment, Z - central moment, K - cumulant

    TString generator = "PYTHIA8";

    const Int_t nOrders = 1; /// <-- MOMENT

    const Int_t nTunes = 4;
    const Int_t nEnergies = 4;

    Double_t energies[nEnergies] = {5.02, 7., 8., 13.};
    Int_t tunes[nTunes] = {0, 6, 14, 21};


    // --------- Read in all Files ------------------------------------------------------

    TH1D* meanpT[nOrders][nEnergies][nTunes];
    TH1D* meanpT_data[nOrders][nEnergies];

    TFile *inSim[nOrders];
    TFile *inData[nOrders];

    Double_t nEvents[nOrders][nEnergies][nTunes];


    for (int order = 0; order < nOrders; order++) {

        // == Read Simulations ==

        inSim[order] = new TFile(TString(Form("../Output/OutSim%c%d", measure, order + 1)).
                                                Append(generator).Append(".root"), "READ");

        if (!inSim[order]) {
            cout << "Error opening File InSim" << endl;
            exit(0);
        }


        for (int energy = 0; energy < nEnergies; energy++){
            for (int tune = 0; tune < nTunes; tune++){


                meanpT[order][energy][tune] = (TH1D*) inSim[order]->
                                FindObjectAny(Form("meanpT%d/%d",
                                                   (int)energies[energy], tunes[tune]));

                if(!meanpT[order][energy][tune]){
                    cout << "Error opening histogram mean pT " << energies[energy]
                          << " TeV/ Tune " << tunes[tune]<< endl;
                    exit(0);
                }

                nEvents[order][energy][tune] = meanpT[order][energy][tune]->GetEntries();

                cout << nEvents[order][energy][tune] << endl;

            }
        }


        // == Read Data ==

        inData[order] = new TFile(TString(Form("../Output/OutData%c%d", measure, order + 1)).
                           Append(".root"), "READ");

        if (!inData[order]) {
            cout << "Error opening File OutData" << endl;
            exit(0);
        }

        for (int energy = 0; energy < nEnergies; energy++){

            meanpT_data[order][energy] = (TH1D*) inData[order]->
                                FindObjectAny(Form("meanpT_data%d", (int)energies[energy]));

            if(!meanpT[order][energy]){
                cout << "Error opening histogram mean pT data " << energies[energy] << endl;
                exit(0);
            }

        }

    }

    // ---------- Compute Mean Deviation and Mean Squared Deviation ---------------------

    Int_t binMin = 10;
    Int_t binMax = 50;

    TH1D* deviation[nOrders][nTunes];
    TH1D* deviation2[nOrders][nTunes];

    Double_t D, D2;
    Double_t sum, sum2;
    Double_t t_bin, d_bin;

    for (int order = 0; order < 1; order++){

        for (int tune = 0; tune < 3; tune++){
            for (int energy = 0; energy < nEnergies; energy++){
/*
                sum = sum2 = 0;

                deviation[order][tune]  = new TH1D(Form("D_O%d_T%d", order + 1, tunes[tune]),
                                                                "", 15, 0.5, 15.5);
                deviation2[order][tune] = new TH1D(Form("D2_O%d_T%d", order + 1, tunes[tune]),
                                                                "", 15, 0.5, 15.5);

                for (int bin = binMin; bin <= binMax; bin++){

                    t_bin = meanpT[order][energy][tune]->GetBinContent(bin);
                    d_bin = meanpT_data[order][energy] ->GetBinContent(bin);

                    sum  = TMath::Abs(t_bin - d_bin);
                    sum2 = TMath::Power(t_bin - d_bin, 2);

                }

                D  = sum  / (binMax - binMin);
                D2 = sum2 / (binMax - binMin);

                deviation[order][tune]->Fill(energy, D);
                deviation[order][tune]->Fill(energy, D2);
*/
                cout << order + 1 << gap << tunes[0] << gap << energies[energy] << endl;
//                                                        << gap << D << gap << D2 << endl;


           }
        }
    }

    deviation[1][0]->Draw();



    //meanpT[0][0][0]->Draw();

    // ------------ Output --------------------------------------------------------------

   /* TFile* outData = new TFile(TString(Form("Output/Ratios", measure, order))
                                                         .Append(".root"), "RECREATE");

    for (int energy = 0; energy < nEnergies; energy++){

        meanpT_data[energy]->Write(Form("meanpT_data%d", (int)energies[energy]));

    } */


    // ----- Cleaning Up ----------------------------------------------------------------

    for (int order = 0; order < 1; order++){

        inSim[order]->Close();
        inData[order]->Close();

    }

}
