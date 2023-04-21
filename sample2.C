// sample.C
// sample code for overwrite histgram
 {

#include <iostream>
#include <fstream>

#include <TH1.h>
#include <TF1.h>
#include <TFile.h>
#include <TNtuple.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TROOT.h>


	TFile* f1 = TFile::Open("20220620171447_list.root"); //FG Molec
	TFile* f2 = TFile::Open("20220620180215_list.root"); //BG Molec
	TFile* f3 = TFile::Open("20220620122324_list.root"); //FG Hydr
	TFile* f4 = TFile::Open("20220620102327_list.root"); //BG hydr
	TFile* f5 = TFile::Open("20220620171447_list.root"); //FG Bare
	TFile* f6 = TFile::Open("20220620180215_list.root"); //BG Bare
	
	//Draws the UCNTOF or energy channels for the detectors. switch out the comments for what you're trying to find.
	//Scale is the inverse of KP, i.e. 1/KP.
	f1->cd();
	Six->Draw("e>>h1(120, 1000, 8000)", "ch == 3 && ucntof > 55 && ucntof < 85", "EH"); //Bare FG
	//Six->Draw("ucntof*e-3>>h5(120, 1000, 8000)", "ch == 3 && e > 1000", "EH");
	h1->Scale(1.658e-05);

	f2->cd();
	Six->Draw("e>>h2(120, 1000, 8000)", "ch == 3 && ucntof > 55 && ucntof < 85", "EH"); //Bare BG
	//Six->Draw("ucntof*e-3>>h5(120, 1000, 8000)", "ch == 3 && e > 1000", "EH");
	h2->SetLineColor(2);
	h2->Scale(1.684e-05);

	f3->cd();
	Six->Draw("e>>h3(120, 1000, 8000)", "ch == 3 && ucntof > 55 && ucntof < 85", "EH"); //Hydroxide FG
	//Six->Draw("ucntof*e-3>>h5(120, 1000, 8000)", "ch == 3 && e > 1000", "EH");
	h3->SetLineColor(3);
	h3->Scale(5.646e-06);

	f4->cd();
	Six->Draw("e>>h4(120, 1000, 8000)", "ch == 3 && ucntof > 55 && ucntof < 85", "EH"); //Hydrox BG
	//Six->Draw("ucntof*e-3>>h5(120, 1000, 8000)", "ch == 3 && e > 1000", "EH");
	h4->SetLineColor(2);
	h4->Scale(6.578e-06);

	f5->cd();
	Six->Draw("e>>h5(120, 1000, 8000)", "ch == 7 && ucntof > 55 && ucntof < 85", "EH"); //Bare FG
	//Six->Draw("ucntof*e-3>>h5(120, 1000, 8000)", "ch == 7 && e > 1000", "EH");
	h5->SetLineColor(5);
	h5->Scale(1.658e-05);

	f6->cd();
	Six->Draw("e>>h6(120, 1000, 8000)", "ch == 7 && ucntof > 55 && ucntof < 85", "EH"); //Bare BG
	//Six->Draw("ucntof*e-3>>h5(120, 1000, 8000)", "ch == 3 && e > 1000", "EH");
	h6->SetLineColor(3);
	h6->Scale(1.684e-05);

	f1->cd();
	h1->Draw();
	f2->cd();
	h2->Draw("same");
	f3->cd();
	h3->Draw("same");
	f4->cd();
	h4->Draw("same");
	f5->cd();
	h5->Draw("same");
	f6->cd();
	h6->Draw("same");


	//Draws the foreground - background for each detector
	
	TH1F* Molecularly_Bonded = new TH1F("Molecularly Bonded", "Molecularly Bonded", 120, 1000, 8000);
	TH1F* Bare = new TH1F("Bare Detector", "Bare Detector", 120, 1000, 8000);
	TH1F* Hydroxide = new TH1F("Hydroxide Bonded", "Hydroxide Bonded", 120, 1000, 8000);


	Molecularly_Bonded->Add(h1, h2, 1, -1);
	Hydroxide->Add(h3, h4, 1, -1);
	Bare->Add(h5, h6, 1, -1);

	Molecularly_Bonded->SetLineColor(1);
	Hydroxide->SetLineColor(2);
	Bare->SetLineColor(3);

	Molecularly_Bonded->Draw();
	Hydroxide->Draw("same");
	Bare->Draw("same");

	c1->SetGridx();
	c1->SetGridy();
	

	//Makes any ratios for efficiency comparisons
	/*
	TH1F* Ratio1 = new TH1F("Ratio", "Ratio, Bare/Dunia", 120, 55, 85);
	TH1F* Ratio2 = new TH1F("ZnSRatio", "Ratio, ZnS/Dunia", 120, 55, 85);
	TH1F* Ratio3 = new TH1F("ZnS/Bare efficiency ratio", "ZnS/Bare efficiency ratio", 120, 55, 85);



	/*Ratio1->Divide(Molecularly_Bonded, Dunia);
	Ratio1->SetMaximum(2);
	Ratio1->SetMinimum(0);
	Ratio1->GetYaxis()->SetTitle("Detector efficiency");
	Ratio1->GetXaxis()->SetTitle("UCN TOF");
	Ratio1->SetLineColor(1);

	Ratio2->Divide(ZnS, Dunia);
	Ratio2->SetMaximum(2);
	Ratio2->SetLineColor(2);

	Ratio3->Divide(Ratio2, Ratio1);
	Ratio3->SetMaximum(10);
	Ratio3->SetMinimum(0);
	Ratio3->GetYaxis()->SetTitle("Efficiency of ZnS/Bare");
	Ratio3->GetXaxis()->SetTitle("UCN TOF");

	leg->AddEntry(Ratio1, "Bare Efficiency", "l");
	leg->AddEntry(Ratio2, "ZnS efficiency", "l");

	Ratio1->Draw();
	leg->Draw("same");
	Ratio2->Draw("same");

	Ratio3->Draw();
	*/

	
	//Legend. Un-comment whatever you want to add to the legend
	TLegend* leg = new TLegend(0.5, 0.78, 0.75, 0.98);
	//leg->AddEntry(h1, "Molecularly Bonded Foreground", "l");
	//leg->AddEntry(h2, "Molecularly Bonded Background", "l");
	//leg->AddEntry(h3, "Hydroxide Bonded Foreground");
	//leg->AddEntry(h4, "Hydroxide Bonded Background");
	//leg->AddEntry(h5, "Bare Foreground", "l");
	//leg->AddEntry(h6, "Bare Background");
	//leg->AddEntry(Molecularly_Bonded, "Molecularly Bonded FG - BG", "l");
	//leg->AddEntry(Bare, "Bare Detector FG - BG", "l");
	//leg->AddEntry(Hydroxide, "Hydroxide Bonded FG - BG", "l");
	leg->Draw();

	
	//Change h1 to Molecularly_Bonded if you're doing FG-BG measurements or the code will crash
	h1->GetYaxis()->SetTitle("Entries/KP");
	//h1->GetXaxis()->SetTitle("UCNTOF (ms)");
	h1->GetXaxis()->SetTitle("Energy (ADC Channels)");
	
	
}