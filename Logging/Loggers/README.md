# Loggers

These are a pair of loggers I've used in many console projects. There are some additional features that have been quite useful:

* __Call tracking__ - each log macro has it's file/line/function stored within the call, adding just a little more info to traceback to where the call happened.  When using an ide like visual studio, you can click on the logging line to take you to that line of code.  LogMarker handles a majority of this.
* __Break behavior__ can be configured.  You can choose to break on a log statement, continue, or ignore all future logs of this type.  This is helpful in case there is a breaking log that you wish to stop seeing/breaking, like if an art resource is missing.
* __Scripting__ - All logging subscriptions commands are exposed to the [scripting](/Scripting) system, which allows for per-user configuration at runtime.
* __Platform separation__ - PlatformPrint handles most of the platform separation.  In this project, it was Windows and Non-Windows platforms, which fit nicely into one file.  For projects with more platforms, you would separate these out into their own files. 

Summaries of the files in this folder:

* [ConsoleLogger.h](ConsoleLogger.h)/[cpp](ConsoleLogger.cpp): A simple TTY logger.  Spits out logs to the consoles TTY using PlatformPrint.  Also has some break/continue/ignore functionality. 
* [ScreenReporter.h](ScreenReporter.h)/[cpp](ScreenReporter.cpp): An on screen logger.  Log calls are rendered on top of the render target, and must be called every frame.  This allows for viewing specific values change over time, which can be easier to read when running at 60fps.  It also reduces console log spam, since these calls can be seen on screen, as opposed to in bulk in the console log.
* [LogMarker.h](LogMarker.h)/[cpp](LogMarker.cpp): a logging helper class, keeps track if the log line is enabled.
* [PlatformPrint.h](PlatformPrint.h)/[cpp](PlatformPrint.cpp): a #define-d file that determines which functions to use when logging based on the target platform.  This one was used for Win32 and Sony Playstation platforms.

