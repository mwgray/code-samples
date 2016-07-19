// Copyright 2006-12 HumaNature Studios Inc.

#include "TweenEquation.h"

#include "Misc.h"

#include <math.h>

namespace core {

    // common tweener equations, from Tweener.as
    TweenEquation::Signature TweenEquation::getEquation(TweenType type)
    {
        switch(type)
        {

        case LinearTween: return &easeNone;

        case EaseInQuad: return &easeInQuad;
        case EaseOutQuad: return &easeOutQuad;
        case EaseInOutQuad: return &easeInOutQuad;
        case EaseOutInQuad: return &easeOutInQuad;

        case EaseInCubic: return &easeInCubic;
        case EaseOutCubic: return &easeOutCubic;
        case EaseInOutCubic: return &easeInOutCubic;
        case EaseOutInCubic: return &easeOutInCubic;

        case EaseInQuart: return &easeInQuart;
        case EaseOutQuart: return &easeOutQuart;
        case EaseInOutQuart: return &easeInOutQuart;
        case EaseOutInQuart: return &easeOutInQuart;

        case EaseInQuint: return &easeInQuint;
        case EaseOutQuint: return &easeOutQuint;
        case EaseInOutQuint: return &easeInOutQuint;
        case EaseOutInQuint: return &easeOutInQuint;

        case EaseInSine: return &easeInSine;
        case EaseOutSine: return &easeOutSine;
        case EaseInOutSine: return &easeInOutSine;
        case EaseOutInSine: return &easeOutInSine;

        case EaseInCirc: return &easeInCirc;
        case EaseOutCirc: return &easeOutCirc;
        case EaseInOutCirc: return &easeInOutCirc;
        case EaseOutInCirc: return &easeOutInCirc;

        case EaseInExpo: return &easeInExpo;
        case EaseOutExpo: return &easeOutExpo;        // default
        case EaseInOutExpo: return &easeInOutExpo;
        case EaseOutInExpo: return &easeOutInExpo;

        case EaseInElastic: return &easeInElastic;
        case EaseOutElastic: return &easeOutElastic;
        case EaseInOutElastic: return &easeInOutElastic;
        case EaseOutInElastic: return &easeOutInElastic;

        case EaseInBack: return &easeInBack;
        case EaseOutBack: return &easeOutBack;
        case EaseInOutBack: return &easeInOutBack;
        case EaseOutInBack: return &easeOutInBack;

        case EaseInBounce: return &easeInBounce;
        case EaseOutBounce: return &easeOutBounce;
        case EaseInOutBounce: return &easeInOutBounce;
        case EaseOutInBounce: return &easeOutInBounce;
            
        default:
            return easeNone;
        }
    }

    float TweenEquation::easeNone(float t, float b, float c, float d)
    {
        return c*t/d + b;
    }

    float TweenEquation::easeInQuad(float t, float b, float c, float d)
    {
        return c*(t/=d)*t + b;
    }

    float TweenEquation::easeOutQuad(float t, float b, float c, float d)
    {
        return -c *(t/=d)*(t-2) + b;
    }

    float TweenEquation::easeInOutQuad(float t, float b, float c, float d)
    {
        if ((t/=d/2) < 1) return c/2*t*t + b;
        return -c/2 * ((--t)*(t-2) - 1) + b;
    }

    float TweenEquation::easeOutInQuad(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutQuad (t*2, b, c/2, d);
        return easeInQuad((t*2)-d, b+c/2, c/2, d);
    }

    float TweenEquation::easeInCubic(float t, float b, float c, float d)
    {
        return c*(t/=d)*t*t + b;
    }

    float TweenEquation::easeOutCubic(float t, float b, float c, float d)
    {
        return c*((t=t/d-1)*t*t + 1) + b;
    }

    float TweenEquation::easeInOutCubic(float t, float b, float c, float d)
    {
        if ((t/=d/2) < 1) return c/2*t*t*t + b;
        return c/2*((t-=2)*t*t + 2) + b;
    }

    float TweenEquation::easeOutInCubic(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutCubic (t*2, b, c/2, d);
        return easeInCubic((t*2)-d, b+c/2, c/2, d);
    }

    float TweenEquation::easeInQuart(float t, float b, float c, float d)
    {
        return c*(t/=d)*t*t*t + b;
    }

    float TweenEquation::easeOutQuart(float t, float b, float c, float d)
    {
        return -c * ((t=t/d-1)*t*t*t - 1) + b;
    }

    float TweenEquation::easeInOutQuart(float t, float b, float c, float d)
    {
        if ((t/=d/2) < 1) return c/2*t*t*t*t + b;
        return -c/2 * ((t-=2)*t*t*t - 2) + b;
    }

    float TweenEquation::easeOutInQuart(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutQuart (t*2, b, c/2, d);
        return easeInQuart((t*2)-d, b+c/2, c/2, d);
    }

    float TweenEquation::easeInQuint(float t, float b, float c, float d)
    {
        return c*(t/=d)*t*t*t*t + b;
    }

    float TweenEquation::easeOutQuint(float t, float b, float c, float d)
    {
        return c*((t=t/d-1)*t*t*t*t + 1) + b;
    }

    float TweenEquation::easeInOutQuint(float t, float b, float c, float d)
    {
        if ((t/=d/2) < 1) return c/2*t*t*t*t*t + b;
        return c/2*((t-=2)*t*t*t*t + 2) + b;
    }

    float TweenEquation::easeOutInQuint(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutQuint (t*2, b, c/2, d);
        return easeInQuint((t*2)-d, b+c/2, c/2, d);
    }

    float TweenEquation::easeInSine(float t, float b, float c, float d)
    {
        return -c * cosf(t/d * (kPi/2)) + c + b;
    }

    float TweenEquation::easeOutSine(float t, float b, float c, float d)
    {
        return c * sinf(t/d * (kPi/2)) + b;
    }

    float TweenEquation::easeInOutSine(float t, float b, float c, float d)
    {
        return -c/2 * (cosf(kPi*t/d) - 1) + b;
    }

    float TweenEquation::easeOutInSine(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutSine (t*2, b, c/2, d);
        return easeInSine((t*2)-d, b+c/2, c/2, d);
    }

    float TweenEquation::easeInExpo(float t, float b, float c, float d)
    {
        return (t==0) ? b : c * powf(2, 10 * (t/d - 1)) + b - c * 0.001f;
    }

    float TweenEquation::easeOutExpo(float t, float b, float c, float d)
    {
        return (t==d) ? b+c : c * 1.001f * (-powf(2, -10 * t/d) + 1) + b;
    }

    float TweenEquation::easeInOutExpo(float t, float b, float c, float d)
    {
        if (t==0) return b;
        if (t==d) return b+c;
        if ((t/=d/2) < 1) return c/2 * powf(2, 10 * (t - 1)) + b - c * 0.0005f;
        return c/2 * 1.0005f * (-powf(2, -10 * --t) + 2) + b;
    }

    float TweenEquation::easeOutInExpo(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutExpo (t*2, b, c/2, d);
        return easeInExpo((t*2)-d, b+c/2, c/2, d);
    }

    float TweenEquation::easeInCirc(float t, float b, float c, float d)
    {
        return -c * (sqrtf(1 - (t/=d)*t) - 1) + b;
    }

    float TweenEquation::easeOutCirc(float t, float b, float c, float d)
    {
        return c * sqrtf(1 - (t=t/d-1)*t) + b;
    }

    float TweenEquation::easeInOutCirc(float t, float b, float c, float d)
    {
        if ((t/=d/2) < 1) return -c/2 * (sqrtf(1 - t*t) - 1) + b;
        return c/2 * (sqrtf(1 - (t-=2)*t) + 1) + b;
    }

    float TweenEquation::easeOutInCirc(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutCirc (t*2, b, c/2, d);
        return easeInCirc((t*2)-d, b+c/2, c/2, d);
    }

    float TweenEquation::easeInElastic(float t, float b, float c, float d)
    {
        if (t==0) return b;
        if ((t/=d)==1) return b+c;
        float p = d*.3f;
        float s = p / 4;
        float a = c;
        return -(a*powf(2,10*(t-=1)) * sinf( (t*d-s)*(2*kPi)/p )) + b;
    }

    float TweenEquation::easeOutElastic(float t, float b, float c, float d)
    {
        if (t==0) return b;
        if ((t/=d)==1) return b+c;
        float p = d*.3f;
        float s = p / 4.0f;
        float a = c;
        return (a*powf(2,-10*t) * sinf( (t*d-s)*(2*kPi)/p ) + c + b);
    }

    float TweenEquation::easeInOutElastic(float t, float b, float c, float d)
    {
        if (t==0) return b;
        if ((t/=d/2)==2) return b+c;
        float p = d*(.3f*1.5f);
        float s = p / 4.0f;
        float a = c;
        if (t < 1) return -.5f*(a*powf(2,10*(t-=1)) * sinf( (t*d-s)*(2*kPi)/p )) + b;
        return a*powf(2,-10*(t-=1)) * sinf( (t*d-s)*(2*kPi)/p )*.5f + c + b;
    }

    float TweenEquation::easeOutInElastic(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutElastic (t*2, b, c/2, d);
        return easeInElastic((t*2)-d, b+c/2, c/2, d);
    }

    float TweenEquation::easeInBack(float t, float b, float c, float d)
    {
        float s = 1.70158f;
        return c*(t/=d)*t*((s+1)*t - s) + b;
    }

    float TweenEquation::easeOutBack(float t, float b, float c, float d)
    {
        float s = 1.70158f;
        return c*((t=t/d-1)*t*((s+1)*t + s) + 1) + b;
    }

    float TweenEquation::easeInOutBack(float t, float b, float c, float d)
    {
        float s = 1.70158f;
        if ((t/=d/2) < 1) return c/2*(t*t*(((s*=(1.525f))+1)*t - s)) + b;
        return c/2*((t-=2)*t*(((s*=(1.525f))+1)*t + s) + 2) + b;
    }

    float TweenEquation::easeOutInBack(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutBack (t*2, b, c/2, d);
        return easeInBack((t*2)-d, b+c/2, c/2, d);
    }

    float TweenEquation::easeOutBounce(float t, float b, float c, float d)
    {
        if ((t/=d) < (1/2.75f)) {
            return c*(7.5625f*t*t) + b;
        } else if (t < (2/2.75f)) {
            return c*(7.5625f*(t-=(1.5f/2.75f))*t + .75f) + b;
        } else if (t < (2.5f/2.75f)) {
            return c*(7.5625f*(t-=(2.25f/2.75f))*t + .9375f) + b;
        } else {
            return c*(7.5625f*(t-=(2.625f/2.75f))*t + .984375f) + b;
        }
    }

    float TweenEquation::easeInBounce(float t, float b, float c, float d)
    {
        return c - easeOutBounce (d-t, 0, c, d) + b;
    }

    float TweenEquation::easeInOutBounce(float t, float b, float c, float d)
    {
        if (t < d/2) return easeInBounce (t*2, 0, c, d) * .5f + b;
        else return easeOutBounce (t*2-d, 0, c, d) * .5f + c*.5f + b;
    }

    float TweenEquation::easeOutInBounce(float t, float b, float c, float d)
    {
        if (t < d/2) return easeOutBounce (t*2, b, c/2, d);
        return easeInBounce((t*2)-d, b+c/2, c/2, d);
    }

} // namespace core