#include "Libraries/FlatheadGames/MathHelpers.h"

#include "QC_MathConstants.h"

double BounceEaseIn(double t, double b, double c, double d);
double BounceEaseOut(double t, double b, double c, double d);
double BounceEaseInOut(double t, double b, double c, double d);
double ElasticEaseIn(double t, double b, double c, double d, float elasticity);
double ElasticEaseOut(double t, double b, double c, double d, float elasticity);
double ElasticEaseInOut(double t, double b, double c, double d, float elasticity);

float TweenFunc_Linear(float startvalue, float valuerange, double timepassed, double totaltime)
{
    float timeperc = (float)(timepassed/totaltime);
    if( timeperc > 1 )
        timeperc = 1;
    return startvalue + valuerange*timeperc;
}

float TweenFunc_SineEaseIn(float startvalue, float valuerange, double timepassed, double totaltime)
{
    float timeperc = (float)(timepassed/totaltime);
    if( timeperc > 1 )
        timeperc = 1;
    return startvalue + -valuerange * cos(timeperc * (float)PI/2.0f) + valuerange;
}

float TweenFunc_SineEaseOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    float timeperc = (float)(timepassed/totaltime);
    if( timeperc > 1 )
        timeperc = 1;
    return startvalue + valuerange * sin(timeperc * (float)PI/2.0f);
}

float TweenFunc_SineEaseInOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    float timeperc = (float)(timepassed/totaltime);
    if( timeperc > 1 )
        timeperc = 1;
    return startvalue + -valuerange/2.0f * (cos(timeperc * (float)PI)-1.0f);
}

float TweenFunc_BounceEaseIn(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)BounceEaseIn( timepassed, startvalue, valuerange, totaltime );
}

float TweenFunc_BounceEaseOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)BounceEaseOut( timepassed, startvalue, valuerange, totaltime );
}

float TweenFunc_BounceEaseInOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)BounceEaseInOut( timepassed, startvalue, valuerange, totaltime );
}

float TweenFunc_ElasticEaseIn(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)ElasticEaseIn( timepassed, startvalue, valuerange, totaltime, 3 );
}

float TweenFunc_ElasticEaseOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)ElasticEaseOut( timepassed, startvalue, valuerange, totaltime, 3 );
}

float TweenFunc_ElasticEaseInOut(float startvalue, float valuerange, double timepassed, double totaltime)
{
    return (float)ElasticEaseInOut( timepassed, startvalue, valuerange, totaltime, 3 );
}

//http://www.kirupa.com/forum/archive/index.php/t-76799.html
double BounceEaseIn(double t, double b, double c, double d)
{
    return c - BounceEaseOut( d-t, 0, c, d ) + b;
}

double BounceEaseOut(double t, double b, double c, double d)
{
    if( (t/=d) < (1.0f/2.75f) )
    {
        return c * (7.5625*t*t) + b;
    }
    else if (t < (2/2.75))
    {
        double postFix = t -= 1.5 / 2.75;
        return c * (7.5625 * postFix * t + .75) + b;
    }
    else if (t < (2.5/2.75))
    {
        double postFix = t -= 2.25 / 2.75;
        return c * (7.5625 * postFix * t + .9375) + b;
    }
    else
    {
        double postFix = t -= 2.625 / 2.75;
        return c * (7.5625f * postFix * t + .984375) + b;
    }
}

double BounceEaseInOut(double t, double b, double c, double d)
{
    if( t < d/2 )
        return BounceEaseIn( t*2, 0, c, d ) * .5f + b;
    else
        return BounceEaseOut( t*2-d, 0, c, d ) * .5f + c*.5f + b;
}

double ElasticEaseIn(double t, double b, double c, double d, float elasticity)
{
    if( t == 0 )
        return b;
    if( t > d )
        return b+c;
    if( ( t /= d ) == 1 )
        return b+c;

    double p = d * 0.3f;
    double a = c;
    double s = p/4;

    double postFix = a * pow( 10, elasticity*(t-=1) );

    return -( postFix * (double)sin( (float)((t * d - s) * (2. * PId) / p) ) ) + b;
}

double ElasticEaseOut(double t, double b, double c, double d, float elasticity)
{
    if( t==0 )
        return b;
    if( t > d )
        return b+c;
    if( ( t /= d ) == 1 )
        return b+c;

    double p = d * 0.3f;
    double a = c;
    double s = p/4;

    return ( a * pow( 10, (double)(- elasticity) * t) * (double)sin((float)((t * d - s) * (2. * PId) / p)) + c + b);
}

double ElasticEaseInOut(double t, double b, double c, double d, float elasticity)
{
    if( t == 0 )
        return b;
    if( t > d )
        return b+c;
    if( ( t /= d/2 ) == 2 )
        return b+c;

    double p = d * ( 0.3 * 1.5 );
    double a = c;
    double s = p/4;

    if( t < 1 )
    {
        double postFix = a * pow( 10, elasticity*( t-=1 ) );
        return -0.5f * ( postFix * (double)sin( (float)((t * d - s) * (2. * PId) / p) ) ) + b;
    }

    double postFix = a * pow( 10, -elasticity*( t-=1 ) );
    return postFix * (double)sin( (float)(( t*d-s ) * ( 2. * PId )/p) ) * 0.5f + c + b;
}
