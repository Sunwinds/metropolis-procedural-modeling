//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################

#include <iostream>
#include <QApplication>
#include <QFileDialog>
#include "Common.hpp"
#include "ModelWindow.hpp"

//####################################################################
// Entry point
//#####################################################################
int main(int argc,char** argv)
{
    QApplication application(argc,argv);
    QStringList args=application.arguments();
    WidgetType widgetType=IMAGE;

    bool validArgs=true;

    if(args.size()==1) widgetType=IMAGE;
    else if(args.size()==2)
    {
        if(args.at(1)=="-image" || args.at(1)=="-i") widgetType=IMAGE;
        else validArgs=false;
    }
    else validArgs=false;

    if(!validArgs){ std::cout<<"Usage: ./ProcModeler [-image (-i) | -voxel (-v)]"<<std::endl; return -1; }

    ModelWindow window(widgetType);
    window.show();
    return application.exec();
}
//#####################################################################
