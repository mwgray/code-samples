// Copyright 2006-12 HumaNature Studios Inc.

#ifndef __TWEENTYPE_H__
#define __TWEENTYPE_H__

namespace core {

    // these correspond to the equations in TweenEquation.h
    enum TweenType
    {
        LinearTween,

        EaseInQuad,
        EaseOutQuad,
        EaseInOutQuad,
        EaseOutInQuad,

        EaseInCubic,
        EaseOutCubic,
        EaseInOutCubic,
        EaseOutInCubic,

        EaseInQuart,
        EaseOutQuart,
        EaseInOutQuart,
        EaseOutInQuart,

        EaseInQuint,
        EaseOutQuint,
        EaseInOutQuint,
        EaseOutInQuint,

        EaseInSine,
        EaseOutSine,
        EaseInOutSine,
        EaseOutInSine,

        EaseInCirc,
        EaseOutCirc,
        EaseInOutCirc,
        EaseOutInCirc,

        EaseInExpo,
        EaseOutExpo,
        EaseInOutExpo,
        EaseOutInExpo,

        EaseInElastic,
        EaseOutElastic,
        EaseInOutElastic,
        EaseOutInElastic,

        EaseInBack,
        EaseOutBack,
        EaseInOutBack,
        EaseOutInBack,

        EaseInBounce,
        EaseOutBounce,
        EaseInOutBounce,
        EaseOutInBounce,

        TweenType_Total,
        TweenType_Default = EaseOutExpo
    };

} // namespace core

#endif // __TWEENTYPE_H__