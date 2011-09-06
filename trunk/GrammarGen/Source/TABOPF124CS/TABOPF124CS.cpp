//#####################################################################
// Copyright 2009, Jerry Talton.
// This software is governed by the license contained in LICENSE.
//#####################################################################
#include "Common.hpp"
#include "LeftBracketSymbol.hpp"
#include "RightBracketSymbol.hpp"
#include "PlusSymbol.hpp"
#include "MinusSymbol.hpp"
#include "AmpersandSymbol.hpp"
#include "CaretSymbol.hpp"
#include "BackSlashSymbol.hpp"
#include "ForwardSlashSymbol.hpp"
#include "PipeSymbol.hpp"
#include "ExclamationSymbol.hpp"
#include "FSymbol.hpp"
#include "CSymbol.hpp"

using namespace ProceduralModeling;

//#####################################################################
// Function axiomNumSymbolEntries
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C uint axiomNumSymbolEntries()
{
    return 3;
}
//#####################################################################
// Function axiomNumSymbols
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C uint axiomNumSymbols()
{
    return 1;
}
//#####################################################################
// Function axiom
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C void axiom(SymbolEntry* axiom,Symbol** symbolTable)
{
    axiom[0].mID=100;
    axiom[2].mParam=0.0;
    axiom[1].mFlags=symbolTable[100]->setFlags(reinterpret_cast<float*>(axiom+2));
}
//#####################################################################
// Function createSymbolTable
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C Symbol** createSymbolTable()
{
    Symbol** table=new Symbol*[400];
    table=new Symbol*[400];
    memset(table,0,400*sizeof(Symbol*));
    table[0]=new LeftBracketSymbol(table);
    table[1]=new RightBracketSymbol(table);
    table[2]=new PlusSymbol(table);
    table[3]=new MinusSymbol(table);
    table[4]=new AmpersandSymbol(table);
    table[5]=new CaretSymbol(table);
    table[6]=new BackSlashSymbol(table);
    table[7]=new ForwardSlashSymbol(table);
    table[8]=new PipeSymbol(table);
    table[9]=new ExclamationSymbol(table);
    table[100]=new FSymbol(table);
    table[200]=new CSymbol(table);
    return table;
}
//#####################################################################
// Function createSymbolTable
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C uint numSymbolsInTable()
{
    return 400;
}
//#####################################################################
// Function derivationDepthLimit
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C uint derivationDepthLimit()
{
    return 5;
}
//#####################################################################
// Function destroySymbolTable
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C void destroySymbolTable(Symbol*** table)
{
    for(uint i=0;i<400;i++) delete (*table)[i];
    delete[] (*table);
}
//#####################################################################
// Function setupGraphics
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C void setupGraphics(Symbol** table, ResourceHandler* resourceHandler)
{
    for (uint i=0;i<400;i++) if(table[i])
    { table[i]->setResourceHandler(resourceHandler); table[i]->initGraphics(); }
    resourceHandler->initOutputMesh(new Mesh(), MESH);
}
//#####################################################################
// Function initOutputMesh
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C void initOutputMesh(ResourceHandler* resourceHandler)
{
    resourceHandler->getOutputMesh()->init();
}
//#####################################################################
// Function saveOutputMesh
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C void saveOutputMesh(ResourceHandler* resourceHandler, const QString& name)
{
    resourceHandler->getOutputMesh()->saveToObjFile(name);
}
//#####################################################################
