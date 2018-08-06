
// ~~ POSTPROCESSING ~~

// --------------------------------------------------------------------------------------
// Create Plots for meanpT spectra from Simulations and Data
//
// --------------------------------------------------------------------------------------

// == Includes ==

#include "Plotting.h"

// == Namespace ==

using namespace std;

// --------------------------------------------------------------------------------------
// postprocessing
// --------------------------------------------------------------------------------------

void ppResults(){

    // ------ Define Program Specific Parameters ----------------------------------------

    Int_t order = 1; /// <-- MOMENT
    Char_t measure = 'Z'; /// M - moment, Z - central moment, K - cumulant

    TString generator = "PYTHIA8";

    const Int_t nTunes = 4;
    const Int_t nEnergies = 4;

    Double_t energies[nEnergies] = {5.02, 7., 8., 13.};
    Int_t tunes[nTunes] = {0, 6, 14, 21};
    TString nameTune[nTunes] = {"default", "4Cx", "Monash 2013", "ATLAS A14, NNPDF2.3LO"};


    // --------- Read in all Files ------------------------------------------------------

    TH1D* meanpT[nEnergies][nTunes];
    TH1D* meanpT_data[nEnergies];

    TFile *inSim, *inData;

    Double_t nEvents[nEnergies][nTunes];

    // == Read Simulations ==

    inSim = new TFile(TString(Form("Output/OutSim%c%d", measure, order)).
                                            Append(generator).Append(".root"), "READ");

    if (!inSim) {
        cout << "Error opening File OutSim" << endl;
        exit(0);
    }


    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){


            meanpT[energy][tune] = (TH1D*) inSim->
                            FindObjectAny(Form("meanpT%d/%d",
                                               (int)energies[energy], tunes[tune]));

            if(!meanpT[energy][tune]){
                cout << "Error opening histogram mean pT " << energies[energy]
                      << " TeV/ Tune " << tunes[tune]<< endl;
                exit(0);
            }

            nEvents[energy][tune] = meanpT[energy][tune]->GetEntries();

        }
    }

    // == Read Data ==

    inData = new TFile(TString(Form("Output/OutData%c%d", measure, order)).Append(".root"), "READ");

    if (!inData) {
        cout << "Error opening File OutData" << endl;
        exit(0);
    }

    for (int energy = 0; energy < nEnergies; energy++){

        meanpT_data[energy] = (TH1D*) inData->
                            FindObjectAny(Form("meanpT_data%d", (int)energies[energy]));

        if(!meanpT[energy]){
            cout << "Error opening histogram mean pT data " << energies[energy] << endl;
            exit(0);
        }

    }


    // ------------ Ratios --------------------------------------------------------------

    TH1D *ratios[nEnergies][nTunes];

    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            ratios[energy][tune] = (TH1D*)meanpT_data[energy]->
                                    Clone(Form("ratio%d/%d",
                                               (int)energies[energy], tunes[tune]));

            ratios[energy][tune]->Divide(meanpT[energy][tune]);

        }
    }

    // ------ Set Histogram Properties for Plotting -------------------------------------

     for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            // == Sim ==

            SetHistogramProperties(meanpT[energy][tune], "",
                                                "<#it{p}_{T}> (GeV/#it{c})", tune + 1);

            switch (order * measure) {

                case 1 * 'M': meanpT[energy][tune]->GetYaxis()->SetRangeUser(0.24, 0.86);
                              break;
                case 2 * 'M': meanpT[energy][tune]->GetYaxis()->SetRangeUser(-0.14, 1.3);
                              break;
                case 3 * 'M': meanpT[energy][tune]->GetYaxis()->SetRangeUser(-0.84, 3.5);
                              break;

                case 1 * 'Z': meanpT[energy][tune]->GetYaxis()->SetRangeUser(-0.95, 0.95);
                              break;
                case 2 * 'Z': meanpT[energy][tune]->GetYaxis()->SetRangeUser(-0.096, 0.54);
                              break;
                case 3 * 'Z': meanpT[energy][tune]->GetYaxis()->SetRangeUser(-0.54, 1.54);
                              break;

                case 1 * 'K': meanpT[energy][tune]->GetYaxis()->SetRangeUser(0.24, 0.86);
                              break;
                case 2 * 'K': meanpT[energy][tune]->GetYaxis()->SetRangeUser(-0.096, 0.54);
                              break;
                case 3 * 'K': meanpT[energy][tune]->GetYaxis()->SetRangeUser(-0.54, 1.54);
                              break;

            }

            // == Ratios ==
            SetHistogramProperties(ratios[energy][tune],
                                        "multiplicity #it{N}_{ch}", "Ratio", tune + 1);

            switch (order * measure) {

                case 1 * 'M': ratios[energy][tune]->GetYaxis()->SetRangeUser(0.85, 1.16);
                               break;
                case 2 * 'M': ratios[energy][tune]->GetYaxis()->SetRangeUser(0.85, 1.35);
                              break;
                case 3 * 'M': ratios[energy][tune]->GetYaxis()->SetRangeUser(0.85, 1.7);
                              break;

                case 1 * 'Z': ratios[energy][tune]->GetYaxis()->SetRangeUser(0.85, 1.16);
                              break;
                case 2 * 'Z': ratios[energy][tune]->GetYaxis()->SetRangeUser(0.95, 1.55);
                              break;
                case 3 * 'Z': ratios[energy][tune]->GetYaxis()->SetRangeUser(0.85, 1.7);
                              break;

                case 1 * 'K': ratios[energy][tune]->GetYaxis()->SetRangeUser(0.85, 1.16);
                              break;
                case 2 * 'K': ratios[energy][tune]->GetYaxis()->SetRangeUser(0.95, 1.55);
                              break;
                case 3 * 'K': ratios[energy][tune]->GetYaxis()->SetRangeUser(0.85, 1.7);
                              break;

            }


        }

        // == Data ==
        SetHistogramProperties(meanpT_data[energy], "", "<#it{p}_{T}> (GeV/#it{c})", 0);
        meanpT_data[energy]->SetMarkerStyle(kFullDiamond);

    }


    // ------------ Plotting ------------------------------------------------------------

    for (int energy = 0; energy < nEnergies; energy++){

        // == Dashed Line for Ratio at 1 ==

        TLine *one = new TLine(0., 1., 50., 1);
        one->SetLineStyle(9);
        one->SetLineWidth(2);
        one->SetLineColor(kBlack);

        // == Legends ==

        TLegend *lInfo = new TLegend(0.123762 , 0.673071, 0.363861, 0.854834);

        lInfo->SetTextFont(42);
        lInfo->SetTextSize(0.03);
        lInfo->SetBorderSize(0);

        lInfo->AddEntry((TObject*)0x0, "PYTHIA 8.23", "");
        lInfo->AddEntry((TObject*)0x0,
               Form("pp collisions at #sqrt{s} = %.2lf TeV", energies[energy]), "");
        lInfo->AddEntry((TObject*)0x0, "charged particles", "");
        lInfo->AddEntry((TObject*)0x0, "-0.8 < #eta < 0.8", "");
        lInfo->AddEntry((TObject*)0x0, "0.15 < #it{p}_{T} (GeV/#it{c}) < 10", "");


        TLegend *lmeanpT = new TLegend(0.167079, 0.031412, 0.365099, 0.224154);

        lmeanpT->SetTextFont(42);
        lmeanpT->SetTextSize(0.03);
        lmeanpT->SetBorderSize(0);

        lmeanpT->AddEntry(meanpT_data[energy], "data", "lp");

        // == TObject Arrays ==

        TObjArray* hist = new TObjArray();
        TObjArray* ratio = new TObjArray();


        for (int tune = 0; tune < nTunes; tune++){

            lmeanpT->AddEntry(meanpT[energy][tune],
                              legendEntry(nameTune[tune], nEvents[energy][tune]), "lp");

            hist->Add(meanpT[energy][tune]);
            ratio->Add(ratios[energy][tune]);

        }

        hist->Add(meanpT_data[energy]);

        hist->Add(lInfo);
        hist->Add(lmeanpT);

        ratio->Add(one);

        TCanvas *output = makeCanvas(hist, ratio, "NoTime", 0, 0);

        output->Update();
        output->SaveAs(Form("Results/%c%d/%c%d_vs_nCh_vglmitDaten_%dTeV.png",
                            measure, order, measure, order, (int)energies[energy]));
        output->SaveAs(Form("Results/%c%d/%c%d_vs_nCh_vglmitDaten_%dTeV.root",
                            measure, order, measure, order, (int)energies[energy]));

        }

    // ----- Cleaning Up ----------------------------------------------------------------

    inSim->Close();
    inData->Close();

}

























