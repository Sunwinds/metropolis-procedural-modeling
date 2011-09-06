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
#include "LSymbol.hpp"
#include "ASymbol.hpp"
#include "MSymbol.hpp"
#include "CSymbol.hpp"
#include "LnoneSymbol.hpp"
#include "TSymbol.hpp"
#include "ColorSymbol.hpp"
#include "WidthSymbol.hpp"
#include "StemSymbol.hpp"
#include "EtrunkSymbol.hpp"
#include "TurntovecSymbol.hpp"
#include "ESymbol.hpp"
#include "TurnperptovecSymbol.hpp"
#include "TurnupSymbol.hpp"
#include "PolygonSymbol.hpp"
#include "TurnztovecSymbol.hpp"
#include "LeafconnectorSymbol.hpp"
#include "LeafSymbol.hpp"
#include "BackSlash_0Symbol.hpp"
#include "BackSlash_1Symbol.hpp"
#include "BackSlash_2Symbol.hpp"
#include "BackSlash_3Symbol.hpp"
#include "BackSlash_4Symbol.hpp"
#include "BackSlash_5Symbol.hpp"
#include "BackSlash_6Symbol.hpp"
#include "BackSlash_7Symbol.hpp"
#include "BackSlash_8Symbol.hpp"
#include "BackSlash_9Symbol.hpp"
#include "BackSlash_10Symbol.hpp"
#include "BackSlash_11Symbol.hpp"
#include "BackSlash_12Symbol.hpp"
#include "BackSlash_13Symbol.hpp"
#include "BackSlash_14Symbol.hpp"
#include "BackSlash_15Symbol.hpp"
#include "BackSlash_16Symbol.hpp"
#include "BackSlash_17Symbol.hpp"
#include "BackSlash_18Symbol.hpp"
#include "BackSlash_19Symbol.hpp"
#include "Stem_0Symbol.hpp"
#include "Stem_1Symbol.hpp"
#include "Stem_2Symbol.hpp"
#include "Stem_3Symbol.hpp"
#include "Stem_4Symbol.hpp"
#include "Stem_5Symbol.hpp"
#include "Stem_6Symbol.hpp"
#include "Stem_7Symbol.hpp"
#include "Stem_8Symbol.hpp"
#include "Stem_9Symbol.hpp"
#include "Stem_10Symbol.hpp"
#include "Stem_11Symbol.hpp"
#include "Stem_12Symbol.hpp"
#include "Stem_13Symbol.hpp"
#include "Plus_0Symbol.hpp"
#include "Plus_1Symbol.hpp"
#include "Plus_2Symbol.hpp"
#include "Plus_3Symbol.hpp"
#include "Plus_4Symbol.hpp"
#include "Plus_5Symbol.hpp"
#include "Plus_6Symbol.hpp"
#include "Plus_7Symbol.hpp"
#include "Plus_8Symbol.hpp"
#include "Plus_9Symbol.hpp"
#include "Plus_10Symbol.hpp"
#include "Plus_11Symbol.hpp"
#include "Plus_12Symbol.hpp"
#include "Plus_13Symbol.hpp"
#include "Minus_0Symbol.hpp"
#include "Minus_1Symbol.hpp"
#include "Minus_2Symbol.hpp"
#include "Minus_3Symbol.hpp"
#include "Minus_4Symbol.hpp"
#include "Minus_5Symbol.hpp"
#include "Minus_6Symbol.hpp"
#include "Minus_7Symbol.hpp"
#include "Minus_8Symbol.hpp"
#include "Minus_9Symbol.hpp"
#include "Minus_10Symbol.hpp"

using namespace ProceduralModeling;

//#####################################################################
// Function axiomNumSymbolEntries
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C uint axiomNumSymbolEntries()
{
    return 15;
}
//#####################################################################
// Function axiomNumSymbols
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C uint axiomNumSymbols()
{
    return 4;
}
//#####################################################################
// Function axiom
//#####################################################################
PROCEDURAL_PLUGIN_EXPORT_C void axiom(SymbolEntry* axiom,Symbol** symbolTable)
{
    axiom[0].mID=200;
    axiom[1].mParam=0.6;
    axiom[2].mParam=0.3;
    axiom[3].mParam=0.1;
    axiom[4].mID=201;
    axiom[5].mParam=0.005*exp(log((12.0*12.0*12.0*0.3))/1.7);
    axiom[6].mID=300;
    axiom[7].mParam=Math::uniformZeroToOne();
    axiom[8].mID=101;
    axiom[10].mParam=12.0;
    axiom[11].mParam=1.0;
    axiom[12].mParam=137.5;
    axiom[13].mParam=1.0;
    axiom[14].mParam=(12.0*12.0*12.0*0.3);
    axiom[9].mFlags=symbolTable[101]->setFlags(reinterpret_cast<float*>(axiom+10));
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
    table[100]=new MSymbol(table);
    table[101]=new ASymbol(table);
    table[102]=new CSymbol(table);
    table[103]=new TSymbol(table);
    table[104]=new LSymbol(table);
    table[105]=new LnoneSymbol(table);
    table[200]=new ColorSymbol(table);
    table[201]=new WidthSymbol(table);
    table[202]=new StemSymbol(table);
    table[203]=new EtrunkSymbol(table);
    table[204]=new TurntovecSymbol(table);
    table[205]=new ESymbol(table);
    table[206]=new TurnperptovecSymbol(table);
    table[207]=new TurnupSymbol(table);
    table[208]=new PolygonSymbol(table);
    table[209]=new TurnztovecSymbol(table);
    table[210]=new LeafconnectorSymbol(table);
    table[211]=new LeafSymbol(table);
    table[301]=new BackSlash_0Symbol(table);
    table[304]=new BackSlash_1Symbol(table);
    table[308]=new BackSlash_2Symbol(table);
    table[312]=new BackSlash_3Symbol(table);
    table[316]=new BackSlash_4Symbol(table);
    table[320]=new BackSlash_5Symbol(table);
    table[324]=new BackSlash_6Symbol(table);
    table[327]=new BackSlash_7Symbol(table);
    table[331]=new BackSlash_8Symbol(table);
    table[335]=new BackSlash_9Symbol(table);
    table[339]=new BackSlash_10Symbol(table);
    table[343]=new BackSlash_11Symbol(table);
    table[347]=new BackSlash_12Symbol(table);
    table[352]=new BackSlash_13Symbol(table);
    table[353]=new BackSlash_14Symbol(table);
    table[354]=new BackSlash_15Symbol(table);
    table[355]=new BackSlash_16Symbol(table);
    table[356]=new BackSlash_17Symbol(table);
    table[357]=new BackSlash_18Symbol(table);
    table[358]=new BackSlash_19Symbol(table);
    table[300]=new Stem_0Symbol(table);
    table[303]=new Stem_1Symbol(table);
    table[305]=new Stem_2Symbol(table);
    table[309]=new Stem_3Symbol(table);
    table[313]=new Stem_4Symbol(table);
    table[317]=new Stem_5Symbol(table);
    table[321]=new Stem_6Symbol(table);
    table[325]=new Stem_7Symbol(table);
    table[328]=new Stem_8Symbol(table);
    table[332]=new Stem_9Symbol(table);
    table[336]=new Stem_10Symbol(table);
    table[340]=new Stem_11Symbol(table);
    table[344]=new Stem_12Symbol(table);
    table[348]=new Stem_13Symbol(table);
    table[307]=new Plus_0Symbol(table);
    table[311]=new Plus_1Symbol(table);
    table[315]=new Plus_2Symbol(table);
    table[319]=new Plus_3Symbol(table);
    table[323]=new Plus_4Symbol(table);
    table[326]=new Plus_5Symbol(table);
    table[330]=new Plus_6Symbol(table);
    table[334]=new Plus_7Symbol(table);
    table[338]=new Plus_8Symbol(table);
    table[342]=new Plus_9Symbol(table);
    table[346]=new Plus_10Symbol(table);
    table[349]=new Plus_11Symbol(table);
    table[350]=new Plus_12Symbol(table);
    table[351]=new Plus_13Symbol(table);
    table[302]=new Minus_0Symbol(table);
    table[306]=new Minus_1Symbol(table);
    table[310]=new Minus_2Symbol(table);
    table[314]=new Minus_3Symbol(table);
    table[318]=new Minus_4Symbol(table);
    table[322]=new Minus_5Symbol(table);
    table[329]=new Minus_6Symbol(table);
    table[333]=new Minus_7Symbol(table);
    table[337]=new Minus_8Symbol(table);
    table[341]=new Minus_9Symbol(table);
    table[345]=new Minus_10Symbol(table);
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
    return 45;
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
