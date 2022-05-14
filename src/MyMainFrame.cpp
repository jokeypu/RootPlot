#include "MyMainFrame.h"
#include <TApplication.h>
#include <TGButton.h>
#include <TF1.h>
#include <TRandom.h>
#include <TCanvas.h>
#include <iostream>

MyMainFrame::MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h) : TGMainFrame(p, w, h)
{
    // Properly terminate application loop from window manager signal
    Connect("CloseWindow()", "MyMainFrame", this, "Exit()");
    DontCallClose();

    // Creates widgets of the example
    fEcanvas = new TRootEmbeddedCanvas("Ecanvas", this, w, h);
    AddFrame(fEcanvas, new TGLayoutHints(kLHintsExpandX | kLHintsExpandY, 10, 10, 10, 1));
    // TGHorizontalFrame *hframe=new TGHorizontalFrame(this, 1000,800);
    // TGTextButton *draw = new TGTextButton(hframe,"&Draw");
    // draw->Connect("Clicked()","MyMainFrame",this,"DoDraw()");
    // hframe->AddFrame(draw, new TGLayoutHints(kLHintsCenterX, 5,5,3,4));
    // TGTextButton *exit = new TGTextButton(hframe,"&Exit ","gApplication->Terminate()");
    // hframe->AddFrame(exit, new TGLayoutHints(kLHintsCenterX,5,5,3,4));
    // AddFrame(hframe,new TGLayoutHints(kLHintsCenterX,2,2,2,2)); SetWindowName("Simple Example");
    MapSubwindows();
    Resize(GetDefaultSize());
    MapWindow();
}

MyMainFrame::~MyMainFrame()
{
    fMain->Cleanup();
    delete fMain;
}

void MyMainFrame::DoDraw(TObject *obj, Option_t *option)
{
    TCanvas *fCanvas = fEcanvas->GetCanvas();
    fCanvas->cd();
    obj->Draw(option);
    fCanvas->Update();
}

void MyMainFrame::Exit()
{
    // to stay in the ROOT session
    // DeleteWindow();

    // To exit and close the ROOT session
    gApplication->Terminate();
}
