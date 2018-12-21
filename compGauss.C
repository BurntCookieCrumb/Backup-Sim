
// ~~ TEST OF COMP FOR STATISTICAL DISTRIBUTIONS ~~

// --------------------------------------------------------------------------------------
// test the deviation measures for statistical distributions                              //
// to look at the influence of statistical fluctuations                                   //
// --------------------------------------------------------------------------------------

// == Includes ==

#include "../../Plotting.h"

// == Namespace ==

using namespace std;

// --------------------------------------------------------------------------------------
// test
// --------------------------------------------------------------------------------------

Double_t Gauss(Double_t *var, Double_t *par){

    /** Normalized to [2] **/

    Double_t x = var[0];
    return (1/(TMath::Sqrt(TMath::TwoPi()*par[0]))) *
                                            exp(-0.5*TMath::Power((x-par[1]),2)/par[0]);

}

void compGauss(){

    // ------ Define Program Specific Parameters ----------------------------------------

    Double_t nEvents = 1.e7;
    Int_t nNchBins = 70;
    Int_t nGaussBins = 40;

    Int_t nFills = nEvents * nGaussBins / nNchBins;

    Double_t sigma = 5;
    Double_t mu = 0;

    Int_t binMin = 1;
    Int_t binMax = nGaussBins;

    // ------ Define necessary Histogramms & Stuff --------------------------------------

    // Function Gauss True

    TF1 *gaussTrue = new TF1("gaussTrue", &Gauss, -4, 4, 2);
    gaussTrue->SetParameters(sigma, mu);

    // Histogramm Gauss Rand (random filled)

    TH1D *gaussRand = new TH1D("gaussRand", "", nGaussBins, -4, 4);
    gaussRand->FillRandom("gaussTrue", nFills);
    gaussRand->Sumw2();

    gaussRand->Scale(gaussTrue->Integral(-4,4)/gaussRand->Integral(binMin, binMax, "width"));

    // Vergleichsgroesse

    Double_t D, D2, D_error, D2_error;
    Double_t sum, sum2, sum_error_square, diff_square, sum_error, sum2_error;
    Double_t tg_bin, rg_bin, tg_bin_error, rg_bin_error;

    sum = sum2 = sum_error = sum2_error = 0;

    for (int bin = binMin; bin <= binMax; bin++){

        tg_bin = gaussTrue->Eval(gaussRand->GetBinCenter(bin));
        rg_bin = gaussRand->GetBinContent(bin);

        tg_bin_error = 0;
        rg_bin_error = gaussRand->GetBinError(bin);

        sum  += TMath::Abs(rg_bin - tg_bin);
        sum2 += TMath::Power(rg_bin - tg_bin, 2);

        sum_error_square = TMath::Power(rg_bin_error, 2) +
                                                TMath::Power(tg_bin_error, 2);
        diff_square = TMath::Power(tg_bin - rg_bin, 2);


        sum_error += sum_error_square;
        sum2_error += diff_square * sum_error_square;


    }

    D  = sum  / (binMax - binMin);
    D2 = sum2 / (binMax - binMin);

    D_error  = TMath::Sqrt(sum_error) / (binMax - binMin);
    D2_error = 2*TMath::Sqrt(sum2_error) / (binMax - binMin);


    // ------------ Plotting ------------------------------------------------------------

    TObjArray *array = new TObjArray();

    SetHistogramProperties(gaussRand, "x", "gaussian", 3);
    gaussRand->GetXaxis()->SetRangeUser(-4,4);
    gaussRand->GetYaxis()->SetRangeUser(0,0.2);


    array->Add(gaussRand);
    array->Add(gaussTrue);

    TCanvas* output = makeCanvas(array, 0x0, "NoTime", 0, 0);
    output->SaveAs("testGauss.png");

    // ------------ Output -------------------------------------------------------------

    cout << "Deviation"  << gap << D  << gap << "Error:" << gap << D_error << endl;
    cout << "Deviation2" << gap << D2 << gap << "Error:" << gap << D2_error << endl;

    cout << gaussRand->Integral("width") << " || " <<  gaussTrue->Integral(-4, 4) << endl;





}
