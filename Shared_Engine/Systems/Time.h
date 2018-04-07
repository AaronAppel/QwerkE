#ifndef _Time_H_
#define _Time_H_

class Time
{
public:
    Time();
    ~Time();

    void SetDeltatime(double* deltaTime);
    double GetDeltaTime();

private:
    const double* m_DeltaTime;
};

#endif // !_Time_H_
