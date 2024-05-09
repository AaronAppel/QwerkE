#pragma once

typedef unsigned short uint16_t;

template <typename T>
class CallbackFunction
{
public:
    CallbackFunction(T callback)
    {
        m_Callback = callback;
        m_Id = GetNextId();
    }

    T Callback() { return m_Callback; }
    uint16_t Id() { return m_Id; }

private:
    uint16_t GetNextId()
    {
        static uint16_t s_IdCounter = 0; // #TODO Review making a .cpp file for this
        return ++s_IdCounter;
    }
    T m_Callback = nullptr;
    uint16_t m_Id = 0;
};
