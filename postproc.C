
// ~~ TITLE ~~

#include "TH1.h"
#include "TH2.h"
#include "THn.h"
#include "TFile.h"
#include <iostream>


void postproc(){

  TCanvas *c1 = new TCanvas("c1", "Canvas Title", 1000, 500);

  TFile* inFile = new TFile("/u/kschmitt/post_processing/histo.root", "READ");

  if (!inFile) {
      std::cout << "Error opening file" << std::endl;
      exit(0);
   }

  // ------------- Get Histograms  -------------------------

  TH1F *mult = (TH1F*) inFile->FindObjectAny("mult");
  if(!mult) cout << "es gibt ein problem..." << endl;
  Double_t dNumberOfEvents = mult->GetEntries();

  // 0:mult 1:ID 2:pT(GeV) 3:eta
  THnF *hn = (THnF*) inFile->FindObjectAny("hn");
  if(!hn) cout << "es gibt ein problem..." << endl;


  // --------------- Set pT Range ---------------------------

  //hn->GetAxis(2)->SetRangeUser();


  // --------------- Acceptance ----------------------------

  // == Full Range ==

  TH1D *projectionFull = hn->Projection(2);

  // == Limited Range ==

  hn->GetAxis(3)->SetRangeUser(-0.8,0.8);

  TH1D *projection = hn->Projection(2);

  // == Acceptance ==

  TH1D *acceptance = (TH1D*)projection->Clone("acceptance");

  acceptance->Divide(projectionFull);
  //acceptance->Draw();

  // ---------- Mean Transverse Momentum per Multiplicity ----------------------------

  // == meanpT Histogram ==

  TH1D *meanpT = new TH1D("meanpT", "mean transverse momentum per multiplicity", 400, -0.5, 399.5);

  meanpT->Sumw2();
  meanpT->SetTitle("");

  meanpT->GetXaxis()->SetTitle("multiplicity #it{N}_{ch}");
  meanpT->GetYaxis()->SetTitle("<#it{p}_{T}> GeV/#it{c}");
  meanpT->GetYaxis()->SetRangeUser(0.301, 0.899);

  // == Legend ==

  TLegend *lmeanpT = new TLegend(0.139279, 0.554622, 0.378758, 0.871849);

  lmeanpT->SetTextFont(42);
  lmeanpT->SetTextSize(0.04);
  lmeanpT->SetBorderSize(0);

  lmeanpT->AddEntry((TObject*)0x0, "PYTHIA 8.23 (Monash 2013)", "");
  lmeanpT->AddEntry((TObject*)0x0, "pp collisions at #sqrt{s} = 14 TeV", "");
  lmeanpT->AddEntry((TObject*)0x0, "charged particles", "");
  lmeanpT->AddEntry((TObject*)0x0, "-0.8 < #eta < 0.8", "");
  lmeanpT->AddEntry((TObject*)0x0, Form("%3.2e events", dNumberOfEvents), "");
  lmeanpT->AddEntry(meanpT, "mean transverse momentum", "lp");

  TLatex *lTmeanpT = new TLatex();

  lTmeanpT->SetTextSize(0.04);
  lTmeanpT->SetTextFont(42);


  // == Filling meanpT ==

  TH1D *singleMultpT;

  for(int i = 1; i <= 400; i++){

      hn->GetAxis(0)->SetRange(i,i);
      singleMultpT = hn->Projection(2);

      meanpT->SetBinContent(i, singleMultpT->GetMean());
      meanpT->SetBinError(i, singleMultpT->GetMeanError());

      delete singleMultpT;

  }

  // == Draw & Output ==

  meanpT->SetMarkerStyle(8);
  meanpT->SetStats(0);
  meanpT->Draw();

  lmeanpT->Draw("SAME");

  /*
  char information1[] = {"#splitline{PYTHIA 8.23 (Monash 2013)}{pp collisions at 14 TeV}"};
  char information2[] = {"#splitline{-0.8 < #eta < 0.8}{charged particles}"};
  lTmeanpT->DrawLatexNDC(0.178357, 0.789916, information1);
  lTmeanpT->DrawLatexNDC(0.178357, 0.689076, information2);
  lTmeanpT->DrawLatexNDC(0.178357, 0.589076, Form("%3.2e events", dNumberOfEvents));
  */

  c1->SaveAs("meanpT.pdf");
  c1->SaveAs("meanpT.root");

}

