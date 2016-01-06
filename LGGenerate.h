#pragma once
#include "ComGenerate.h"
#include "LGCreateTables.h"

class LGGenerate : public Generate, public LGCreateTables
{
protected:
    void initialize() override {}
    void emit(int i) override;
    void emitType(int i) override;
    void emitStr(int i) override;
    int value(int i) override;
};
