#ifndef _Time_H_
#define _Time_H_

namespace QwerkE
{
    class Time
    {
    public:
        Time();
        ~Time();

        static void SetDeltatime(double* deltaTime);
        static double GetDeltaTime();

    private:
        static const double* m_DeltaTime;
    };
}

#endif // !_Time_H_
