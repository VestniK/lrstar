#pragma once

#include "ComGenerate.h"
#include "PGCreateTables.h"

class PGGenerate: public Generate, public PGCreateTables
{
protected:
    void initialize() override;
    void emit(int i) override;
    void emitType(int i) override;
    void emitStr(int i) override;
    int value(int i) override;
};
