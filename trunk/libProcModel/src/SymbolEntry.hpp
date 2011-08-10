//#####################################################################
// Copyright 2009, Jerry Talton.
//#####################################################################
#ifndef __SYMBOL_ENTRY_HPP__
#define __SYMBOL_ENTRY_HPP__

#include <QDataStream>

namespace ProceduralModeling{

//#####################################################################
// Union SymbolEntry
//#####################################################################
union SymbolEntry
{
    enum FLAGS {CLEAR=0x0,INVALID=0x1,DETERMINISTIC=0x2};

public:
    uint mID;
    ushort mFlags;
    float mParam;

public:
    bool isJumpable() const
    { return !(mFlags & (INVALID | DETERMINISTIC)); }

    bool isInvalid() const
    { return mFlags & INVALID; }

    bool isDeterministic() const
    { return mFlags & DETERMINISTIC; }

//#####################################################################
};

typedef QVector<SymbolEntry> SymbolEntryList;

//####################################################################
} // namespace ProceduralModeling

inline QDataStream& operator<<(QDataStream& out,const ProceduralModeling::SymbolEntry& entry)
{ return out<<entry.mParam; }

inline QDataStream& operator>>(QDataStream& in,ProceduralModeling::SymbolEntry& entry)
{ return in>>entry.mParam; }

#endif // __SYMBOL_ENTRY__
