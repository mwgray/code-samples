# Logging

####Overview

This is a collection of classes to facilitate configurable logging at runtime.  A log call is as follows:
```
void log(Severity severity, const char* category, const char* message, ...);
```

It consists of the the following components:

* A __severity__, which is the traditional usual fatal error, warn, info, debug, trace.  Additionally, there is force, which always logs output, regardless of the category / severity.
* A __category__, which allows for filtering during runtime.  For example, ui related logging uses the category "UI", physics would use "Physics", etc.
* A __message__ and __parameters__, which are formatted via printf.  The evaulation is delayed until the log actually must occur.

A logger can be configured via:

* __Subscriptions__: If a category is subscribed to it will show logs from that category.
* __Severity Filters__: By default, severities of warn and above are always shown, and severities below info are filtered out.  These can be changed to customize the verbosity of the logger.

In order for a user to see a given log statement, they must be subscribed to that category. This offers some advantages:

* This system reduces logging overhead.  Printf evaluation will no occur unless the log is determined to be printed, so it increases performance, which is helpful when you only have 16 ms per frame.
* Users can configure which log statements they wish to see at runtime, reducing logging spam.  UI programmers don't(usually) need to see Physics logs and visa versa.
* While you could achieve this with tools like grep and cat, the performance is very important for real-time systems, and logging overhead is enough to make frame rate suffer.

####Macros

The common log calls are wrapped in help log macros.  While the setup code isn't the easiest to read, it's highly important to use for the logging systems:

* __Global access__: throwing this in the pre-compiled header gives access to logging at all times
* __Reduce bookkeeping__: using macros allows use of ```__FILE__```, ```__LINE__```, ```__FUNCTION__``` macros, which lets the system embed that information at the call, removing the need for the caller to do so.
* __Removable__ - Finally, and probably the most important one, is that we can dead strip the entire logging system from the runtime.  When building a release build, we need all the compute cycles we can get, and even the overhead of calling an empty function is too much.  Dead stripping removes all the logging code from the executable, which frees up cycles and memory.  For a small embedded system like the Nintendo DS, the logging overhead can take up as much as a 1/4 of the runtime memory available, so ensuring it is removed is paramount. 

####Summaries of the files in this folder

* The [Loggers](Loggers) folder has a collection of more concrete loggers that I have used in production environments.
* [Logger.h](Logger.h)/[cpp](Logger.cpp) is the abstract logger class.  It handles severities and category subscriptions.  To create a new logger, sub-class it and implement the `logVargs` function. 
* [Subscription.h](Subscription.h)/[cpp](Subscription.cpp) tracks if a given category is subscribed to or not.
* [SimpleLogger.h](SimpleLogger.h)/[cpp](SimpleLogger.cpp) is a very simple printf style logger.
* [main.cpp](main.cpp) is a sample program to demonstrate some of the logging features.
* [LoggingPch.h](LoggingPch.h) is a helper file to include dependencies for the example.
* [MakeAndRunDebug.sh](MakeAndRunDebug.sh)/[MakeAndRunRelease.sh](MakeAndRunRelease.sh) are scripts that builds and runs the project with logging enabled/disabled at the compile stage.
