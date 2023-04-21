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

#include "./MakeNiki/MakeNiki.h"

void WriteText(TH1F *h1, Char_t* name, Int_t kp){

  int n = h1->GetNbinsX();
  Char_t* filename = Form("hlambda_%s.txt",name);
  ofstream outFile(filename, ios::out);
  outFile << name << " :kp= " <<kp<<endl;
  outFile << h1->GetXaxis()->GetTitle() << " " << h1->GetYaxis()->GetTitle() << " Error"<<endl;
  for(int i=0; i<n+1; i++){
    outFile << h1->GetBinLowEdge(i) << " " << h1->GetBinContent(i) << " "<<h1->GetBinError(i)<<endl;
  }
  outFile.close();
  cout << "text file "<<filename<<" has been made."<<endl;
  return;
}

void Efficiency(){
  
  gROOT->SetStyle("Plain");
  gStyle->SetPalette(1);
  gStyle->SetOptStat(1001111);
  gStyle->SetOptFit(1111);

  TCut burstcut = "tof > 100 && tof < 39990";
  TCut chcut1 = "ch == 11";  //3He tube
  TCut ecut1 = "e > 400";  //3He tube
  TCut chcut2 = "ch == 4";  //3He
  TCut ecut2 = "e > 500";  //3He
  TCut thecut1 = chcut1 && ecut1 && burstcut;  //3He tube
  TCut thecut2 = chcut2 && ecut2 && burstcut;  //3He

  Double_t Distance1 = 16.50;  //3He tube, 16.17 m (MLF source - Port) + 0.33 m
  Double_t Distance2 = 16.41;  //3He, 16.17 m (MLF source - Port) + 0.24 m
  Double_t tzero = 0.008;  //time origin of the TOF in wavelength estimation
  Double_t Conversion = 395.6;  //Planck constant / neutron mass [meter * nanometer / second]

  //In this experiment, the condition of beam collimation is the same.
  //Therefore we does not consider the beam spot size in this macro. 

  Double_t ebin = 8192/16;  //histogram settings for degitized signal voltage
  Double_t emin = 0.;
  Double_t emax = 8192.;

  Double_t tbin = 40;  //histogram settings for time of flight
  Double_t tmin = 0.;
  Double_t tmax = 40.;

  Double_t wbin = 60;  //histogram settings for time of wavelength
  Double_t wmin = 0.;
  Double_t wmax = 1.2;

  //FG 
  TFile* file1 = TFile::Open("20220619182453_list.root");
  TFile* file2 = TFile::Open("20220617164522_list.root"); //FG 3He

  //BG
  TFile* fileb1 = TFile::Open("20220619154322_list.root"); //BG
  TFile* fileb2 = TFile::Open("20220617165607_list.root"); //3He

  

  TTree* six1=(TTree*)file1->Get("Six");
  TTree* six2=(TTree*)file2->Get("Six");
  TTree* sixb1=(TTree*)fileb1->Get("Six");
  TTree* sixb2=(TTree*)fileb2->Get("Six");

  six1->SetAlias("wavelength",Form("(fmod(tof*1e-6 + 0.04 - %f, 0.04) + %f) * %f/%f",tzero,tzero,Conversion,Distance1));
  six2->SetAlias("wavelength",Form("(fmod(tof*1e-6 + 0.04 - %f, 0.04) + %f) * %f/%f",tzero,tzero,Conversion,Distance2));
  sixb1->SetAlias("wavelengthb",Form("(fmod(tof*1e-6 + 0.04 - %f, 0.04) + %f) * %f/%f",tzero,tzero,Conversion,Distance1));
  sixb2->SetAlias("wavelengthb",Form("(fmod(tof*1e-6 + 0.04 - %f, 0.04) + %f) * %f/%f",tzero,tzero,Conversion,Distance2));

  int kp1=six1->GetMaximum("kp");
  int kp2=six2->GetMaximum("kp");
  int kpb1=sixb1->GetMaximum("kp");
  int kpb2=sixb2->GetMaximum("kp");

  cout <<"kp1=" << kp1 <<endl;
  cout <<"kp2=" << kp2 <<endl;
  cout <<"kpb1=" << kpb1 <<endl;
  cout <<"kpb2=" << kpb2 <<endl;


  TH1D* he1=new TH1D("he1","ZnS;pulse height[ch];count/bin",ebin,emin,emax);
  TH1D* he2=new TH1D("he2","3He;pulse height[ch];count/bin",ebin,emin,emax);
  TH1D* heb1=new TH1D("heb1","ZnS;pulse height[ch];count/bin",ebin,emin,emax);
  TH1D* heb2=new TH1D("heb2","3He;pulse height[ch];count/bin",ebin,emin,emax);

  TH1D* htof1=new TH1D("htof1","ZnS;Time of flight [ms];count rate at 760kW [1/bin/sec] ",tbin,tmin,tmax);
  TH1D* htof2=new TH1D("htof2","3He;Time of flight [ms];count rate at 760kW [1/bin/sec] ",tbin,tmin,tmax);
  TH1D* htofb1=new TH1D("htofb1","ZnS;Time of flight [ms];count rate at 760kW [1/bin/sec] ",tbin,tmin,tmax);
  TH1D* htofb2=new TH1D("htofb2","3He;Time of flight [ms];count rate at 760kW [1/bin/sec] ",tbin,tmin,tmax);

  TH1D* hw1=new TH1D("hw1","ZnS;wavelength [nm];count rate at 760kW [1/bin/sec] ",wbin,wmin,wmax);
  TH1D* hw2=new TH1D("hw2","3He;wavelength [nm];count rate at 760kW [1/bin/sec] ",wbin,wmin,wmax);
  TH1D* hwb1=new TH1D("hwb1","ZnS;wavelength [nm];count rate at 760kW [1/bin/sec] ",wbin,wmin,wmax);
  TH1D* hwb2=new TH1D("hwb2","3He;wavelength [nm];count rate at 760kW [1/bin/sec] ",wbin,wmin,wmax);

  six1->Draw("e>>he1",chcut1,"goff");
  he1->Sumw2();
  he1->Scale((25./kp1));
  six1->Draw("tof * 1e-3 >>htof1",thecut1,"goff");
  htof1->Sumw2();
  htof1->Scale((25./kp1));
  six1->Draw("wavelength>>hw1",thecut1,"goff");
  hw1->Sumw2();
  hw1->Scale((25./kp1));
  
  sixb1->Draw("e>>heb1",chcut1,"goff");
  heb1->Sumw2();
  heb1->Scale((25./kpb1));
  sixb1->Draw("tof * 1e-3 >>htofb1",thecut1,"goff");
  htofb1->Sumw2();
  htofb1->Scale((25./kpb1));
  sixb1->Draw("wavelengthb>>hwb1",thecut1,"goff");
  hwb1->Sumw2();
  hwb1->Scale((25./kpb1));


  six2->Draw("e>>he2",chcut2,"goff");
  he2->Sumw2();
  he2->Scale(25./kp2);
  six2->Draw("tof * 1e-3 >>htof2",thecut2,"goff");
  htof2->Sumw2();
  htof2->Scale(25./kp2);
  six2->Draw("wavelength>>hw2",thecut2,"goff");
  hw2->Sumw2();
  hw2->Scale(25./kp2);

  sixb2->Draw("e>>heb2",chcut2,"goff");
  heb2->Sumw2();
  heb2->Scale(25./kpb2);
  sixb2->Draw("tof * 1e-3 >>htofb2",thecut2,"goff");
  htofb2->Sumw2();
  htofb2->Scale(25./kpb2);
  sixb2->Draw("wavelengthb>>hwb2",thecut2,"goff");
  hwb2->Sumw2();
  hwb2->Scale(25./kpb2);


  TLegend *leg = new TLegend(0.5, 0.78, 0.75, 0.98);
  leg->AddEntry(hw1,"ZnS");
  leg->AddEntry(hw2,"3He");

  TCanvas *c1 = new TCanvas("c1","c1",1400,1000);
  c1->Divide(2,2);
  c1->cd(1);

  c1->cd(1); 
  gPad->SetGridx();
  gPad->SetGridy();

//  he1->Add(heb1,-1);
//  he2->Add(heb2,-1);

  he1->Draw("eh");
  heb1->Draw("ehsames");
  he2->Draw("ehsames");
  heb2->Draw("ehsames");
  gPad->SetLogy();
  he1->SetLineColor(1);
  he2->SetLineColor(2);
  heb1->SetLineColor(3);
  heb2->SetLineColor(4);

  TLegend *leg1 = new TLegend(0.5, 0.58, 0.75, 0.98);
  leg1->AddEntry(he1,"ZnS (FG)");
  leg1->AddEntry(heb1,"ZnS (BG)");
  leg1->AddEntry(he2,"3He (FG)");
  leg1->AddEntry(heb2,"3He (BG)");
  leg1->Draw();

  c1->cd(2); 
  gPad->SetGridx();
  gPad->SetGridy();

  gPad->SetLogy();
  hw1->Add(hwb1,-1);
  hw2->Add(hwb2,-1);

  hw1->SetLineColor(1);
  hw2->SetLineColor(2);
  hwb1->SetLineColor(3);
  hwb2->SetLineColor(4);
  hw1->Draw("eh"); 
  hw2->Draw("ehsames");
//  hwb1->Draw("ehsames"); 
//  hwb2->Draw("ehsames");
  hw1->GetYaxis()->SetRangeUser(1e-2,10 * (hw1->GetMaximum()));
  leg->Draw();

  c1->cd(3); 
  gPad->SetGridx();
  gPad->SetGridy();

  gPad->SetLogy();
  htof1->Add(htofb1,-1);
  htof2->Add(htofb2,-1);

  htof1->SetLineColor(1);
  htof2->SetLineColor(2);
  htofb1->SetLineColor(3);
  htofb2->SetLineColor(4);
  htof1->Draw("eh"); 
  htof2->Draw("ehsames");
//  htofb1->Draw("ehsames"); 
//  htofb2->Draw("ehsames");
  htof1->GetYaxis()->SetRangeUser(1e-2,10 * (htof1->GetMaximum()));
  leg->Draw();


  c1->cd(4);
  gPad->SetGridx();
  gPad->SetGridy();

  TH1D *hratioJPARC = (TH1D*)hw1->Clone("hratioJPARC");
  hratioJPARC->Divide(hw2);

  //Transmittance of Fe tube (0.5-mm thickness steel, but culculated by iron) and Al foil (0.1-mm thickness) of the detector is considered.
  TF1* func1 = new TF1("func1", "(exp(-9.072261e-4*x) * (1-exp(-[0]*x))/(exp(-6.03809e-2*x))*(1-exp(-[1]*x)))", 0.25, 0.9);
  func1->SetParameter(0,0.1);
  func1->SetParameter(1,17.);
  func1->SetParLimits(1,1.,17.0); // Limit of ZnS efficiency (0.97MPa)
  func1->SetParName(0,"3He efficiency");
  func1->SetParName(1,"ZnS efficiency");

  hratioJPARC->Draw("eh");

//  hratioJPARC->Fit(func1,"R");
  //hratioJPARC->Fit(func1,"","",0.45,0.9);
//  hratioJPARC->Fit(func1,"","",0.25,1.1);
 // func1->Draw("same");

  hratioJPARC->GetYaxis()->SetTitle("Ratio of 3He/ZnS");
  hratioJPARC->GetYaxis()->SetRangeUser(1e-4,1.2);
  //  hratioJPARC->Draw("eh");


//  WriteText(htof1, (Char_t*)"HeTube", kp1);
//  WriteText(htof2, (Char_t*)"3He", kp2);

//  c1->SaveAs("DuniaEfficiency_refill_TUCAN.png");
//  c1->SaveAs("DuniaEfficiency_refill_TUCAN.root");

  return;
}
