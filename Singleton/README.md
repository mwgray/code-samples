# Singleton

This is a simple implementation of the [Singleton](https://en.wikipedia.org/wiki/Singleton_pattern) design pattern.  Every project I have worked on has had a need to reference a class a global scope, which the Singleton pattern can provide.

I mainly use this pattern for following reasons:
* Ensuring only one instance of an object exists.
* Exposing a way to globaly access said variables.
* Additionally, being able to 'override' singletons to provide platform specific classes, like logging or serialization classes.

I prefer to write a template version of this class to avoid the alternative, where each class author maintains their own static storage and accessors.
 
All of the work is done in the h.  I also added a global map which keeps track of all the singleton instances at run-time.  It's incredibly useful as you can access any data during particularly nasty debug sessions.
 
Two items of note:
* In this implementation, construction and destruction is the responsibility of the calling code.  In game development there's a need for very strict ordering of construction, so it's often preferable to do so explicitly, as opposed to letting the compiler choose when.
* Singletons can be overridden in this system.  This comes in handy in case you have to use a different set of logging functions for a different platform(or remove logging altogether).  
