#ifndef _Debugger_H_
#define _Debugger_H_

class Debugger
{
public:
    Debugger();
    ~Debugger();

    void ToggleConsole();
    void SetConsoleVisible(bool visibility);

private:
    bool m_ConsoleIsOpen = false;
};

#endif // !_Debugger_H_
