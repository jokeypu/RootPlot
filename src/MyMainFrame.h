#ifndef MYMAINFRAME_H
#define MYMAINFRAME_H

#include <TGFrame.h>
#include <TGWindow.h>
#include <TRootEmbeddedCanvas.h>

class MyMainFrame : public TGMainFrame
{
private:
    TGMainFrame *fMain;
    TRootEmbeddedCanvas *fEcanvas;

public:
    MyMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
    virtual ~MyMainFrame();

    void DoDraw(TObject *obj, Option_t *option = "");
    void Exit();

    ClassDef(MyMainFrame, 0)
};

#endif