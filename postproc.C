
// ~~ POSTPROCESSING ~~

// --------------------------------------------------------------------------------------
// Information or so..
//
// --------------------------------------------------------------------------------------

// == Includes ==

#include "Plotting.h"
#include "postproc.h"


// == Namespace ==

using namespace std;

// ------------------------------------------------------------------------------------
// postprocessing
// ------------------------------------------------------------------------------------

void postproc() {

    /** Reads in Files containing Data of different energies from Simulations with different tunes
        and from Experiments, extracts the corresponding histograms and uses these to derive
        Histograms meanpT <-> nCh, which will then be plotted corresponding to energy.
    **/

    // --------- Read in all Files ---------

    const Int_t nTunes = 4;
    const Int_t nEnergies = 4;

    Double_t energies[nEnergies] = {5.02, 7., 8., 13.};
    Int_t tunes[nTunes] = {0, 6, 14, 21};
    TString nameTune[nTunes] = {"default", "4Cx", "Monash 2013", "ATLAS A14, NNPDF2.3LO"};

    TFile *inFiles[nEnergies][nTunes];
    TH1F *mult[nEnergies][nTunes];
    THnF *histograms[nEnergies][nTunes];
    Double_t nEvents[nEnergies][nTunes];


    TFile *inData;
    TH2D *data[nEnergies];

    // Read Simulations

    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            inFiles[energy][tune] = new TFile(Form("./PYTHIA8/%dTeV/histo%d.root",
                                                  (Int_t)energies[energy], tunes[tune]), "READ");

            // Check if File was opened correctly
            if (!inFiles[energy][tune]) {
                    cout << "Error opening File " << energies[energy]
                                                  << " TeV histo" << tunes[tune] << endl;
                    exit(0);
                }

            // Get Histograms
            mult[energy][tune] = (TH1F*) inFiles[energy][tune]->FindObjectAny("mult");
            if(!mult[energy][tune]){
                 cout << "Error opening histogram mult " << energies[energy]
                      << " TeV/ Tune " << tunes[tune]<< endl;
            }

            nEvents[energy][tune] = mult[energy][tune]->GetEntries();

            // histograms with
            //         0:mult 1:ID 2:pT(GeV) 3:eta
            histograms[energy][tune] = (THnF*) inFiles[energy][tune]->FindObjectAny("hn");
            if(!histograms[energy][tune]){
                 cout << "Error opening histogram hn " << energies[energy]
                      << " TeV/ Tune " << tunes[tune]<< endl;
            }

        }
    }

    // Read Data

    inData = new TFile("./Daten/ptSpectraUnfolded.root", "READ");

    for (int energy = 0; energy < nEnergies; energy++){

        data[energy] = (TH2D*) inData->FindObjectAny(Form("multPtUnfolded_pp_%dTeV_eta08", (int)energies[energy]));

        if(!data[energy]) cout << "Error opening histogram data " << energies[energy] << " TeV"<< endl;

    }


    // ---------- Set Ranges --------------

    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            // eta

            histograms[energy][tune]->GetAxis(3)->SetRangeUser(-0.8,0.8);

            // pT

            histograms[energy][tune]->GetAxis(2)->SetRangeUser(0.15, 9.99);

        }

        // data

        data[energy]->GetYaxis()->SetRangeUser(0.15, 9.99);

    }

    // ------ Getting Mean Spectra --------

    TH1D* meanpT[nEnergies][nTunes];

    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            // Initialize meanpT Histogram for each energy and each tune

            meanpT[energy][tune] = new TH1D(Form("meanpT%d/%d", (int)energies[energy], tunes[tune]), "", 200, -0.5, 199.5);

            meanpT[energy][tune]->Sumw2();


            // Fill said Histograms

            GetMeanpT(meanpT[energy][tune], histograms[energy][tune], 2, 0, 200);

            SetHistogramProperties(meanpT[energy][tune], "", "<#it{p}_{T}> (GeV/#it{c})", tune + 1);
            meanpT[energy][tune]->GetYaxis()->SetRangeUser(0.24, 0.86);

        }
    }

    TH1D* meanpT_data[nEnergies];

    for (int energy = 0; energy < nEnergies; energy++){

        // Initialize meanpT Histogram for each energy and each tune

        meanpT_data[energy] = new TH1D(Form("meanpT_data%d", (int)energies[energy]), "", 200, -0.5, 199.5);

        meanpT_data[energy]->Sumw2();

        // Fill said Histograms

        GetMeanpT(meanpT_data[energy], data[energy], 200);

        SetHistogramProperties(meanpT_data[energy], "", "<#it{p}_{T}> (GeV/#it{c})", 0);
        meanpT_data[energy]->SetMarkerStyle(kFullDiamond);

    }

    // ------------ Ratios --------------

    TH1D *ratios[nEnergies][nTunes];

    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            ratios[energy][tune] = (TH1D*)meanpT_data[energy]->Clone(Form("ratio%d/%d", (int)energies[energy], tunes[tune]));

            ratios[energy][tune]->Divide(meanpT[energy][tune]);

            SetHistogramProperties(ratios[energy][tune], "multiplicity #it{N}_{ch}", "Ratio", tune + 1);
            ratios[energy][tune]->GetYaxis()->SetRangeUser(0.85, 1.16);

        }
    }




    // ------------ Plotting --------------

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
        lInfo->AddEntry((TObject*)0x0, Form("pp collisions at #sqrt{s} = %.2lf TeV", energies[energy]), "");
        lInfo->AddEntry((TObject*)0x0, "charged particles", "");
        lInfo->AddEntry((TObject*)0x0, "-0.8 < #eta < 0.8", "");
        lInfo->AddEntry((TObject*)0x0, "0.15 GeV/#it{c} < #it{p}_{T}", ""); //pT


        TLegend *lmeanpT = new TLegend(0.167079, 0.031412, 0.365099, 0.224154);

        lmeanpT->SetTextFont(42);
        lmeanpT->SetTextSize(0.03);
        lmeanpT->SetBorderSize(0);

        lmeanpT->AddEntry(meanpT_data[energy], "data", "lp");

        // == TObject Arrays ==

        TObjArray* hist = new TObjArray();
        TObjArray* ratio = new TObjArray();


        for (int tune = 0; tune < nTunes; tune++){

            lmeanpT->AddEntry(meanpT[energy][tune], legendEntry(nameTune[tune], nEvents[energy][tune]), "lp");

            hist->Add(meanpT[energy][tune]);
            ratio->Add(ratios[energy][tune]);



        }

        hist->Add(meanpT_data[energy]);

        hist->Add(lInfo);
        hist->Add(lmeanpT);

        ratio->Add(one);


        TCanvas *output = makeCanvas(hist, ratio, "NoTime", 0, 0);

        output->Update();
        //output->SaveAs("test.png");
        output->SaveAs(Form("Results/meanpT_vs_nCh_vglmitDaten_%dTeV.png", (int)energies[energy]));
        output->SaveAs(Form("Results/meanpT_vs_nCh_vglmitDaten_%dTeV.root", (int)energies[energy]));

        delete lInfo;
        delete lmeanpT;

        delete hist;
        delete ratio;

        delete output;

    }


    // ----- Cleaning Up ------------------


    for (int energy = 0; energy < nEnergies; energy++){
        for (int tune = 0; tune < nTunes; tune++){

            inFiles[energy][tune]->Close();

            delete meanpT[energy][tune];

        }

        delete meanpT_data[energy];

    }

    inData->Close();

}
