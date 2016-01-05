#pragma once

#include "ComGenerate.h"
#include "PGCreateTables.h"

class PGGenerate: public Generate, public PGCreateTables
{
public:
    static void  Initialize ();

    // TODO: use better approach for polimorphism
    static void  EMIT     (int i);
    static void  EMITTYPE (int i);
    static void  EMITSTR  (int i);
    static int   VALUE    (int i);
};
