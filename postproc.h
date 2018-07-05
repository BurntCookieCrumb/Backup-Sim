
// Includes

#include <string>
#include "TString.h"

// ------------------------------------------------------------------------------------
// helpful functions
// ------------------------------------------------------------------------------------

Double_t xGetMean(TH1 *hist, Double_t pTMin, Double_t pTMax){

    /** function to get the mean of a one dimensional Histogram in range pTMin to pTMax
        containing the yield, which has to be normalized to bin width,
    **/

    if(!hist) return 0;
    //if(hist->GetEntries()<=0) return 0;
    //if(pTMin < hist->GetXaxis()->GetBinLowEdge(1) || pTMax > hist->GetXaxis()->GetBinCenter(hist->GetNbinsX())) return 0;

    Double_t sumWeighted = 0, sum = 0;
    Double_t binCenter, binWidth;
    Double_t yield;

    for(int bin = hist->FindBin(pTMin); bin <= hist->FindBin(pTMax); bin++){

        binCenter = hist->GetBinCenter(bin);
        binWidth = hist->GetBinWidth(bin);
        yield = hist->GetBinContent(bin);

        sumWeighted += yield * binCenter * binWidth;
        sum += yield * binWidth;
    }


    //return sumWeighted/sum;
    if (sum > 0) return sumWeighted/sum;
    else return 0.;


}

Double_t xGetMeanError(TH1 *hist, Double_t pTMin, Double_t pTMax){

    /** function to get the mean error of a one dimensional Histogram in range pTMin to pTMax
        containing the yield, which has to be normalized to bin width,
    **/

    if(!hist) return 0;

    Double_t mean = xGetMean(hist, pTMin, pTMax);

    Double_t errSumWeighted = 0, sum = 0;
    Double_t binCenter, binWidth;

    Double_t yield, yieldError;

    for(int bin = hist->FindBin(pTMin); bin <= hist->FindBin(pTMax); bin++){

        binCenter = hist->GetBinCenter(bin);
        binWidth = hist->GetBinWidth(bin);

        yield = hist->GetBinContent(bin);
        yieldError = hist->GetBinError(bin); //* binCenter;

        errSumWeighted += TMath::Power((binCenter - mean) * yieldError * binWidth, 2);
        sum += yield * binWidth;

    }

    if (sum > 0) return TMath::Sqrt(errSumWeighted) / sum;
    else return 0.;

}


void GetMeanpT(TH1D *meanpT, THnF *source, int pTAxis, int multAxis, int multBins = 200){

    /** Fill a one dimensional Histogram \meanpT with the mean transverse momentum of each
        multiplicity bin of a n dimensional Histogram \source, which should not be normalized
    **/

    TH1D *singleMultpT;

    for(int i = 1; i <= multBins; i++){

          source->GetAxis(multAxis)->SetRange(i,i);
          singleMultpT = source->Projection(pTAxis);

          singleMultpT->Scale(1, "width");

          //meanpT->SetBinContent(i, singleMultpT->GetMean());
          meanpT->SetBinContent(i, xGetMean(singleMultpT, 0.15, 9.99));

          //meanpT->SetBinError(i, singleMultpT->GetMeanError());
          meanpT->SetBinError(i, xGetMeanError(singleMultpT, 0.15, 9.99));

          delete singleMultpT;

      }

}

void GetMeanpT(TH1D *meanpT, TH2D *source, int multBins = 200){

    /** Fill a one dimensional Histogram \meanpT with the mean transverse momentum of each
        multiplicity bin of a 2 dimensional Histogram \source
    **/

    TH1D *singleMultpT;

    for(int i = 1; i <= multBins; i++){

          singleMultpT = source->ProjectionY("singleMultpT", i, i);

          for (int k = 1; k <= singleMultpT->GetXaxis()->GetLast(); k++){

                singleMultpT->SetBinContent(k, (singleMultpT->GetBinContent(k))*(singleMultpT->GetBinCenter(k)));

          }

          //meanpT->SetBinContent(i, singleMultpT->GetMean());
          meanpT->SetBinContent(i, xGetMean(singleMultpT, 0.15, 9.99));

          //meanpT->SetBinError(i, singleMultpT->GetMeanError());
          meanpT->SetBinError(i, xGetMeanError(singleMultpT, 0.15, 9.99));

          delete singleMultpT;

      }

}

TString niceExp(double x){

    /** Receives a floating point number \x and returns a TString containing a nicer exponential
        representation of \x that can be used in a TLegend
    **/

    TString nString = Form("%3.2e", x);

    nString.ReplaceAll("e", " #upoint 10^{");
    nString.ReplaceAll("+", "");
    nString.ReplaceAll("{0", "{");
    nString.ReplaceAll("{-0", "{-");
    nString.Append("}");

    return nString;

}

TString legendEntry(TString tune, double nEvents){

    /** Formats a nice TString containing the used tune \tune and the number of events \nEvents
        that can be used in a TLegend
    **/

    tune.Append("(");
    tune.Append(niceExp(nEvents));
    tune.Append(" events)");

    return tune;

}

