#include <TApplication.h>
#include "MyMainFrame.h"

// Plot()
#include <TF1.h>

// pippim()
#include <RooAbsPdf.h>
#include <RooRealVar.h>
#include <RooBreitWigner.h>
#include <RooGaussian.h>
#include <RooVoigtian.h>
#include <RooFFTConvPdf.h>
#include <RooChebychev.h>
#include <RooDataSet.h>
#include <RooDataHist.h>
#include <RooPlot.h>
#include <RooAddPdf.h>
#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include <TPaveText.h>
#include <TCanvas.h>
#include <TROOT.h>
#include <TSystem.h>
#include <TLegend.h>
#include <TGuiFactory.h>
#include <TBrowser.h>
#include <TRint.h>
#include <RooFitResult.h>
#include <sstream>
#include <string>
#include <iomanip>

#include "bes3plotstyle.h"
using namespace std;
using namespace RooFit;

std::vector<TObject *> pippim()
{
    gROOT->SetStyle("Plain");
    gSystem->Load("libRooFit");
    TCanvas *c = new TCanvas("BESIII", "BESIII Plot", 1000, 800);
    c->SetBatch();

    Double_t y, yy;
    Int_t bin1(200);
    Double_t x1_min(0.4), x1_max(3);
    Double_t ee(1);

    stringstream oss;
    oss << (x1_max - x1_min) / bin1;
    TString bb = oss.str();

    TString tree1("fit5c"), branch1("mrh0");
    TString namex("m(#pi^{+}#pi^{-}) / GeV/c^{2}"), namey("Entries / " + bb + "GeV/c^{2}"), namet("Invariant Mass of #pi^{+}#pi^{-} Distribution");

    TFile *mfile1 = new TFile("../data/Rhopi_inclusive.root");
    TFile *mfile2 = new TFile("../data/Rhopi_signal.root");

    TTree *mytree1 = (TTree *)mfile1->Get(tree1);
    TTree *mytree2 = (TTree *)mfile2->Get(tree1);

    mytree1->SetBranchAddress(branch1, &y);
    mytree2->SetBranchAddress(branch1, &yy);

    TH1D *hist1 = new TH1D("htemp", branch1, bin1, x1_min, x1_max);
    TH1D *hist2 = new TH1D("htemp2", branch1, bin1, x1_min, x1_max);

    Int_t nev1 = (Int_t)mytree1->GetEntries();
    Int_t nev2 = (Int_t)mytree2->GetEntries();

    for (int i = 0; i < nev1 / ee; i++)
    {
        mytree1->GetEntry(i);
        hist1->Fill(y);
    }

    for (int i = 0; i < nev2; i++)
    {
        mytree2->GetEntry(i);
        hist2->Fill(yy);
    }

    RooRealVar x("x", namex, x1_min, x1_max);
    RooRealVar mean("mean", "mean", 0.774, 0.77, 0.78);
    RooRealVar sigma("sigma", "sigma", 0.1478, 0.1475, 0.1480);
    RooRealVar sigmap("sigmap", "sigmap", 0.001, 0., 0.2);
    RooVoigtian sig("sig", "signal p.d.f.", x, mean, sigma, sigmap);

    RooRealVar c0("c0", "coefficient #0", 1.0, -1.5, 1.5);
    RooRealVar c1("c1", "coefficient #1", 0.1, -1.5, 1.5);
    RooRealVar c2("c2", "coefficient #2", -0.1, -1.5, 1.5);
    RooChebychev bkg("bkg", "background p.d.f.", x, RooArgList(c0, c1, c2));

    RooRealVar nsig("nsig", "signal fraction", 11220 / ee, 0, 60000 / ee);
    RooRealVar nbkg("nbkg", "background fraction", 10000 / ee, 0, 60000 / ee);
    RooAddPdf model("model", "model", RooArgList(sig, bkg), RooArgList(nsig, nbkg));

    RooPlot *frame = x.frame(Title(namet));
    RooDataHist data("data", "dataset with x", x, hist1);
    model.fitTo(data, Extended());
    data.plotOn(frame, MarkerStyle(8), MarkerSize(0.8));
    model.plotOn(frame);
    model.plotOn(frame, Components(bkg), LineStyle(kDashed));

    frame->GetXaxis()->CenterTitle(kTRUE);
    frame->GetXaxis()->SetLabelFont(42);
    frame->SetXTitle(namex);
    frame->SetYTitle(namey);
    frame->Draw();
    FormatMC1(hist2);
    hist2->Draw("SAME");
    hist1->SetLineColor(kBlue);

    TLegend *legend = new TLegend(0.69, 0.73, 0.88, 0.87);
    legend->SetTextFont(42);
    legend->SetTextSize(0.03);
    legend->SetFillColor(0);
    legend->SetFillStyle(0);
    legend->AddEntry("h_data", "Inclusive MC", "lpe");
    legend->AddEntry("htemp2", "signal MC", "l");
    legend->AddEntry("model_Norm[x]", "Fiting sig", "l");
    legend->AddEntry("model_Norm[x]_Comp[bkg]", "Fiting bkg", "l");
    legend->Draw("SAME");

    Double_t meanVal = mean.getVal();
    Double_t meanErr = mean.getError();
    Double_t widVal = sigma.getVal();
    Double_t widErr = sigma.getError();
    Double_t nsigVal = nsig.getVal();
    Double_t nsigErr = nsig.getError();
    Double_t nbkgVal = nbkg.getVal();
    Double_t nbkgErr = nbkg.getError();

    TPaveText *pave = new TPaveText(2.18, 433.135, 2.9337, 603.07);
    pave->SetTextFont(42);
    pave->SetTextSize(0.035);
    pave->SetShadowColor(0);
    pave->SetFillColor(0);
    pave->SetFillStyle(0);
    TString Text;

    stringstream oss1;
    oss1 << "m:	  " << setprecision(4) << meanVal << " #pm " << setprecision(3) << meanErr << endl;
    Text = oss1.str();
    pave->AddText(Text);

    stringstream oss2;
    oss2 << "#Gamma:    " << setprecision(4) << widVal << " #pm " << setprecision(3) << widErr << endl;
    Text = oss2.str();
    pave->AddText(Text);

    stringstream oss3;
    oss3 << "n_{S}:            " << setprecision(5) << nsigVal << " #pm " << setprecision(3) << nsigErr << endl;
    Text = oss3.str();
    pave->AddText(Text);

    stringstream oss4;
    oss4 << "n_{B}:            " << setprecision(5) << nbkgVal << " #pm " << setprecision(3) << nbkgErr << endl;
    Text = oss4.str();
    pave->AddText(Text);
    pave->Draw("SAME");

    cout << "m:       " << meanVal << endl;
    cout << "mErr:    " << meanErr << endl;
    cout << "W:       " << widVal << endl;
    cout << "WErr:    " << widErr << endl;
    cout << "Ns:       " << nsigVal << endl;
    cout << "NsErr:    " << nsigErr << endl;
    cout << "Nb:       " << nbkgVal << endl;
    cout << "NbErr:    " << nbkgErr << endl;

    c->Print(tree1 + "-" + branch1 + "-F1.png");

    std::vector<TObject *> obj = {frame, hist2, legend, pave};
    return obj;
}

std::vector<TObject *> Plot()
{
    TF1 *f1 = new TF1("f1", "sin(x)/x", -10, 10);
    f1->SetLineWidth(3);
    TF1 *f2 = new TF1("f1", "sin(2*x)/x", -10, 10);
    f2->SetLineWidth(3);
    std::vector<TObject *> obj;

    obj.push_back(f2);
    obj.push_back(f1);
    return obj;
}

int main(int argc, char **argv)
{
    std::vector<TObject *> obj = Plot();

    TApplication *theApp = new TApplication("root.exe", &argc, argv);
    MyMainFrame *myframe = new MyMainFrame(gClient->GetRoot(), 1200, 800);
    Int_t len = obj.size();

    Bool_t is_same = false;
    for (Int_t i = 0; i < len; i++)
    {
        if (obj[i] != nullptr)
        {
            if (is_same)
            {
                myframe->DoDraw(obj[i], "same");
            }
            else
            {
                myframe->DoDraw(obj[i]);
                is_same = true;
            }
        }
    }
    theApp->Run();
    return 0;
}
