#ifndef _PrintComponent_H_
#define _PrintComponent_H_

#include "../Shared_Engine/GameObject/Components/Component.h"

class PrintComponent : Component
{
public:
    PrintComponent();
    ~PrintComponent();

    void PrintMessage();

private:
    const char* m_Message = "\nPrintComponent\n";
};

#endif // !_PrintComponent_H_
