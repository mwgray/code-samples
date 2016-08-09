# Tweener

####Overview

This is a simple Tweening system I loosely ported from an ActionScript library.  It was used extensively when implementing UI transitions and effects.  You can see examples of the transitions [here](http://hosted.zeh.com.br/tweener/docs/en-us/misc/transitions.html) (this is from the ActionScript library documentation).

Example usage:
```
Tweener.addTween("TestTween")
        .delay(0.5)                                 // wait 0.5 seconds before starting the tween
        .time(1)                                    // take 1 second to complete the tween
        .type(EaseOutExpo)                          // quick at first, slow towards end
        .param(&value, 25)                          // tween the float stored by value to 25 over the course of the tween
        .onStart(this, &QuickTest::onStart)         // when the tween starts(after the delay timer), call onStart
        .onUpdate(this, &QuickTest::onUpdate)       // when ever the tween changes the value, call onUpdate
        .onComplete(this, &QuickTest::onComplete);  // when the tween reaches the target value of 25, call onComplete
```

This is a tween that animates value from to 25.0f.  It calls a callback when the tweening starts, each frame it updates, and when it completes.  It uses a ease out exponential easing function, and takes 5.

####Summaries of the files in this folder 

* [Tweener.h](Tweener.h)/[cpp](Tweener.cpp) is the entry point for the user, where TweenerTweener.addTweenTweener is exposed.  It also updates and tracks all tweens over time, so the user doesn't have to worry about cleanup unless they need to.
* [Tween.h](Tween.h)/[cpp](Tween.cpp) is the base level class that holds all the tween parameters, and the binding to the member being tweened.
* [TweenCallback.h](TweenCallback.h)/[cpp](TweenCallback.cpp) is a functor storage class for the tween callbacks for global and member functions. 
* [TweenEquation.h](TweenEquation.h)/[cpp](TweenEquation.cpp) holds all the tweening equations.
* [Misc.h](Misc.h) is just some helper code to get the extract code to run by itself
* [main.cpp](main.cpp) is a simple demo that will be built by the CMake project.  Run `mainMakeAndRun.sh` to run.
