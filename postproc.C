
// ~~ TITLE ~~

#include "TH1.h"
#include "TH2.h"
#include "THn.h"
#include "TFile.h"
#include <iostream>


void postproc(){

  // ------------- Preparatory Steps ------------------

  TCanvas *c1 = new TCanvas("c1", "Canvas Title", 1000, 800);

  // create TFiles for all Tunes

  TFile* inFile0  = new TFile("/u/kschmitt/post_processing/histo0.root", "READ");
  TFile* inFile6  = new TFile("/u/kschmitt/post_processing/histo6.root", "READ");
  TFile* inFile14 = new TFile("/u/kschmitt/post_processing/histo.root", "READ");
  TFile* inFile21 = new TFile("/u/kschmitt/post_processing/histo21.root", "READ");

  // check if opening was successful

  if (!inFile0) {
      std::cout << "Error opening file for tune 0" << std::endl;
      exit(0);
   }
  if (!inFile6) {
      std::cout << "Error opening file for tune 6" << std::endl;
      exit(0);
   }
  if (!inFile14) {
      std::cout << "Error opening file for tune 14" << std::endl;
      exit(0);
   }
  if (!inFile21) {
      std::cout << "Error opening file for tune 21" << std::endl;
      exit(0);
   }

  // ------------- Get Histograms  -------------------------

  // multiplicities
  TH1F *mult0 = (TH1F*) inFile0->FindObjectAny("mult");
  if(!mult0) cout << "Error opening histogram mult 0" << endl;
  Double_t dNumberOfEvents0 = mult0->GetEntries();

  TH1F *mult6 = (TH1F*) inFile6->FindObjectAny("mult");
  if(!mult6) cout << "Error opening histogram mult 6" << endl;
  Double_t dNumberOfEvents6 = mult6->GetEntries();

  TH1F *mult14 = (TH1F*) inFile14->FindObjectAny("mult");
  if(!mult14) cout << "Error opening histogram mult 14" << endl;
  Double_t dNumberOfEvents14 = mult14->GetEntries();

  TH1F *mult21 = (TH1F*) inFile21->FindObjectAny("mult");
  if(!mult21) cout << "Error opening histogram mult 21" << endl;
  Double_t dNumberOfEvents21 = mult21->GetEntries();

  // histograms with
  //         0:mult 1:ID 2:pT(GeV) 3:eta

  THnF *hn0 = (THnF*) inFile0->FindObjectAny("hn");
  if(!hn0) cout << "Error opening histogram hn 0" << endl;

  THnF *hn6 = (THnF*) inFile6->FindObjectAny("hn");
  if(!hn6) cout << "Error opening histogram hn 6" << endl;

  THnF *hn14 = (THnF*) inFile14->FindObjectAny("hn");
  if(!hn14) cout << "Error opening histogram hn 14" << endl;

  THnF *hn21 = (THnF*) inFile21->FindObjectAny("hn");
  if(!hn21) cout << "Error opening histogram hn 21" << endl;

  /*
  // --------------- Acceptance ----------------------------

  // == Full Range ==

  TH1D *projectionFull = hn->Projection(2);

  // == Limited Range ==

  hn14->GetAxis(3)->SetRangeUser(-0.8,0.8);

  TH1D *projection = hn14->Projection(2);

  // == Acceptance ==

  TH1D *acceptance = (TH1D*)projection->Clone("acceptance");

  acceptance->Divide(projectionFull);
  //acceptance->Draw();

  */

  // --------------- Set Ranges ---------------------------

  // eta

  hn0 ->GetAxis(3)->SetRangeUser(-0.8,0.8);
  hn6 ->GetAxis(3)->SetRangeUser(-0.8,0.8);
  hn14->GetAxis(3)->SetRangeUser(-0.8,0.8);
  hn21->GetAxis(3)->SetRangeUser(-0.8,0.8);

  // pT

  //hn0 ->GetAxis(2)->SetRangeUser();
  //hn6 ->GetAxis(2)->SetRangeUser();
  //hn14->GetAxis(2)->SetRangeUser();
  //hn21->GetAxis(2)->SetRangeUser();



  // ---------- Mean Transverse Momentum per Multiplicity ----------------------------

  // histogram array

  THnF *hn[4] = {hn0, hn6, hn14, hn21};

  // == meanpT Histograms ==

  TH1D *meanpT0  = new TH1D("meanpT0",  "mean transverse momentum per multiplicity", 400, -0.5, 399.5);
  TH1D *meanpT6  = new TH1D("meanpT6",  "mean transverse momentum per multiplicity", 400, -0.5, 399.5);
  TH1D *meanpT14 = new TH1D("meanpT14", "mean transverse momentum per multiplicity", 400, -0.5, 399.5);
  TH1D *meanpT21 = new TH1D("meanpT21", "mean transverse momentum per multiplicity", 400, -0.5, 399.5);

  TH1D *meanpT[4] = {meanpT0, meanpT6, meanpT14, meanpT21};

  for (int i = 0; i < 4; i++){

      meanpT[i]->Sumw2();
      meanpT[i]->SetTitle("");

      meanpT[i]->GetXaxis()->SetTitle("multiplicity #it{N}_{ch}");
      meanpT[i]->GetYaxis()->SetTitle("<#it{p}_{T}> GeV/#it{c}");
      meanpT[i]->GetYaxis()->SetRangeUser(0.301, 0.899);

  }

  // == Legend ==

  // General Information

  TLegend *lInfo = new TLegend(0.135271, 0.612113, 0.374749, 0.83634);

  lInfo->SetTextFont(42);
  lInfo->SetTextSize(0.04);
  lInfo->SetBorderSize(0);

  lInfo->AddEntry((TObject*)0x0, "PYTHIA 8.23", "");
  lInfo->AddEntry((TObject*)0x0, "pp collisions at #sqrt{s} = 14 TeV", "");
  lInfo->AddEntry((TObject*)0x0, "charged particles", "");
  lInfo->AddEntry((TObject*)0x0, "-0.8 < #eta < 0.8", "");
  //lmeanpT->AddEntry((TObject*)0x0, "-0.8 < #eta < 0.8", ""); //pT

  // Legend

  TLegend *lmeanpT = new TLegend(0.255511, 0.145619, 0.453908, 0.340206);
  lmeanpT->SetTextFont(42);
  lmeanpT->SetTextSize(0.04);
  lmeanpT->SetBorderSize(0);

  lmeanpT->AddEntry(meanpT[0], Form("default (%3.2e events)", dNumberOfEvents0), "lp");
  lmeanpT->AddEntry(meanpT[1], Form("4Cx (%3.2e events)", dNumberOfEvents6), "lp");
  lmeanpT->AddEntry(meanpT[2], Form("Monash 2013 (%3.2e events)", dNumberOfEvents14), "lp");
  lmeanpT->AddEntry(meanpT[3], Form("ATLAS A14, NNPDF2.3LO (%3.2e events)", dNumberOfEvents21), "lp");


  // == Filling meanpT ==

  TH1D *singleMultpT;

  for (int k = 0; k < 4; k++){

      for(int i = 1; i <= 400; i++){

          hn[k]->GetAxis(0)->SetRange(i,i);
          singleMultpT = hn[k]->Projection(2);

          meanpT[k]->SetBinContent(i, singleMultpT->GetMean());
          meanpT[k]->SetBinError(i, singleMultpT->GetMeanError());

          delete singleMultpT;

      }

  }

  // == Draw & Output ==

  meanpT[0]->SetMarkerColor(kRed);
  meanpT[0]->SetLineColor(kRed);

  meanpT[1]->SetLineColor(kBlue);
  meanpT[1]->SetMarkerColor(kBlue);

  meanpT[2]->SetLineColor(kBlack);

  meanpT[3]->SetMarkerColor(kGreen+3);
  meanpT[3]->SetLineColor(kGreen+3);

  for (int k = 0; k < 4; k++){

      meanpT[k]->SetMarkerStyle(8);
      meanpT[k]->SetMarkerSize(0.7);
      meanpT[k]->SetStats(0);
      meanpT[k]->Draw("SAME");

  }

  lInfo->Draw("SAME");
  lmeanpT->Draw("SAME");

  c1->SaveAs("meanpT2.png");
  c1->SaveAs("meanpT2.root");

  // -------------------- Cleaning Up ---------------------

}

