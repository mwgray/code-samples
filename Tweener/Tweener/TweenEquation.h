// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __TWEENEQUATION_H__
#define __TWEENEQUATION_H__

#include "TweenType.h"

namespace core {

    class TweenEquation
    {
    public:
        // Easing equation general parameters
        // @param t     Current time (in frames or seconds).
        // @param b     Starting value.
        // @param c     Change needed in value.
        // @param d     Expected easing duration (in frames or seconds).
        typedef float(*Signature)(float t, float b, float c, float d);        

        static Signature getEquation(TweenType type);

        // Easing equation function for a simple linear tweening, with no easing.
        static float easeNone (float t, float b, float c, float d);

        // Easing equation function for a quadratic (t^2) easing in: accelerating from zero velocity.
        static float easeInQuad (float t, float b, float c, float d);

        // Easing equation function for a quadratic (t^2) easing out: decelerating to zero velocity.
        static float easeOutQuad (float t, float b, float c, float d);

        // Easing equation function for a quadratic (t^2) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutQuad (float t, float b, float c, float d);

        // Easing equation function for a quadratic (t^2) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInQuad (float t, float b, float c, float d);

        // Easing equation function for a cubic (t^3) easing in: accelerating from zero velocity.
        static float easeInCubic (float t, float b, float c, float d);

        // Easing equation function for a cubic (t^3) easing out: decelerating from zero velocity.
        static float easeOutCubic (float t, float b, float c, float d);

        // Easing equation function for a cubic (t^3) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutCubic (float t, float b, float c, float d);

        // Easing equation function for a cubic (t^3) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInCubic (float t, float b, float c, float d);

        // Easing equation function for a quartic (t^4) easing in: accelerating from zero velocity.
        static float easeInQuart (float t, float b, float c, float d);

        // Easing equation function for a quartic (t^4) easing out: decelerating from zero velocity.
        static float easeOutQuart (float t, float b, float c, float d);

        // Easing equation function for a quartic (t^4) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutQuart (float t, float b, float c, float d);

        // Easing equation function for a quartic (t^4) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInQuart (float t, float b, float c, float d);

        // Easing equation function for a quintic (t^5) easing in: accelerating from zero velocity.
        static float easeInQuint (float t, float b, float c, float d);

        // Easing equation function for a quintic (t^5) easing out: decelerating from zero velocity.
        static float easeOutQuint (float t, float b, float c, float d);

        // Easing equation function for a quintic (t^5) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutQuint (float t, float b, float c, float d);

        // Easing equation function for a quintic (t^5) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInQuint (float t, float b, float c, float d);

        // Easing equation function for a sinusoidal (sin(t)) easing in: accelerating from zero velocity.
        static float easeInSine (float t, float b, float c, float d);

        // Easing equation function for a sinusoidal (sin(t)) easing out: decelerating from zero velocity.
        static float easeOutSine (float t, float b, float c, float d);

        // Easing equation function for a sinusoidal (sin(t)) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutSine (float t, float b, float c, float d);

        // Easing equation function for a sinusoidal (sin(t)) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInSine (float t, float b, float c, float d);

        // Easing equation function for an exponential (2^t) easing in: accelerating from zero velocity.
        static float easeInExpo (float t, float b, float c, float d);

        // Easing equation function for an exponential (2^t) easing out: decelerating from zero velocity.
        static float easeOutExpo (float t, float b, float c, float d);

        // Easing equation function for an exponential (2^t) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutExpo (float t, float b, float c, float d);

        // Easing equation function for an exponential (2^t) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInExpo (float t, float b, float c, float d);

        // Easing equation function for a circular (sqrt(1-t^2)) easing in: accelerating from zero velocity.
        static float easeInCirc (float t, float b, float c, float d);

        // Easing equation function for a circular (sqrt(1-t^2)) easing out: decelerating from zero velocity.
        static float easeOutCirc (float t, float b, float c, float d);

        // Easing equation function for a circular (sqrt(1-t^2)) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutCirc (float t, float b, float c, float d);

        // Easing equation function for a circular (sqrt(1-t^2)) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInCirc (float t, float b, float c, float d);

        // Easing equation function for an elastic (exponentially decaying sine wave) easing in: accelerating from zero velocity.
        static float easeInElastic (float t, float b, float c, float d);

        // Easing equation function for an elastic (exponentially decaying sine wave) easing out: decelerating from zero velocity.
        static float easeOutElastic (float t, float b, float c, float d);

        // Easing equation function for an elastic (exponentially decaying sine wave) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutElastic (float t, float b, float c, float d);

        // Easing equation function for an elastic (exponentially decaying sine wave) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInElastic (float t, float b, float c, float d);

        // Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in: accelerating from zero velocity.
        static float easeInBack (float t, float b, float c, float d);

        // Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out: decelerating from zero velocity.
        static float easeOutBack (float t, float b, float c, float d);

        // Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutBack (float t, float b, float c, float d);

        // Easing equation function for a back (overshooting cubic easing: (s+1)*t^3 - s*t^2) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInBack (float t, float b, float c, float d);

        // Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out: decelerating from zero velocity.
        static float easeOutBounce (float t, float b, float c, float d);

        // Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in: accelerating from zero velocity.
        static float easeInBounce (float t, float b, float c, float d);

        // Easing equation function for a bounce (exponentially decaying parabolic bounce) easing in/out: acceleration until halfway, then deceleration.
        static float easeInOutBounce (float t, float b, float c, float d);

        // Easing equation function for a bounce (exponentially decaying parabolic bounce) easing out/in: deceleration until halfway, then acceleration.
        static float easeOutInBounce (float t, float b, float c, float d);        
    };

} // namespace core

#endif // __TWEENEQUATION_H__