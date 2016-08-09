# Singleton

This is a simple implementation of the [Singleton](https://en.wikipedia.org/wiki/Singleton_pattern) design pattern.  Every project I have worked on has had a need to reference a class a global scope, which the Singleton pattern can provide.  For those unfamiliar with the pattern, it offers the following advantages:
* Ensuring only one instance of an object exists.
* Exposing a way to globally access said instance.

I prefer to write a template version of this class to avoid the alternative, where each class author maintains their own static storage and accessors.
 
All of the work is done in the h.  I also added a global map which keeps track of all the singleton instances at run-time.  It's incredibly useful as you can access any data during particularly nasty debug sessions.
 
Two items of note:
* In this implementation, construction and destruction is explicitly the responsibility of the calling code.  In game development there's a need for very strict ordering of construction.  For example, you might need to wait until the memory management system is initialized before creating objects.  Therefore it is often create objects explicitly, as opposed using a lazy initialization system, which depends on a combinations of the compiler and when a singleton is called on in code.
* Singletons can be overridden in this system.  This comes in handy in case you have to use a different set of logging functions for a different platform(or remove logging altogether).  Other applicable systems are disk/network IO, input, cameras, etc.

Summaries of the files in this folder:

* [Singleton.h](Singleton.h)/[cpp](Singleton.cpp) where most of the code for a singleton is.  The h has most of the templating code, and the cpp holds the global map. 
* [SingletonMacros.h](SingletonMacros.h) Since template code can get messy, there are some macros defined to facilitate setting up a singleton's data members. 
* [SingletonPch.h](SingletonPch.h) is a helper file to include dependencies for the example.
 
