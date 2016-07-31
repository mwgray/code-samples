# Logging

This is a collection of files that create a logging system, which can be used for printing to files, or to the screen.  

The basic usage comes down to logging call, which consists of the the following components:

* A category, which allows for filtering during runtime.  For example, ui related logging uses the category "UI", physics would use "Physics", etc.
* Severity, which is the traditional usual fatal error, warn, info, debug, trace.  Additionally, there is force, which basically forces a logging output, regardless of the category / severity. 
* the message itself, which is a printf formatted string followed by the parameters.

In order for a user to see a given log statement, they must be subscribed to that category. This offers some advantages:

 * This system reduces logging overhead.  Printf evaluation will no occur unless the log is determined to be printed, so it increases performance, which is helpful when you only have 16 ms per frame.
 * Users can configure which log statements they wish to see at runtime, reducing logging spam.  UI programmers don't need to see Physics logs and visa versa.
 * While you could achieve this with tools like grep and cat, the performance is very important for real-time systems, and logging overhead is enough to make frame rate suffer.

Some additional features:
 
* Platform separation - PlatformPrint handles most of the platform separation.  In this project, it was Windows and Non-Windows platforms, which fit nicely into one file.  For projects with more platforms, you would separate these out into their own files. 
* Call location tracking - each log call has it's file/line/function stored within the call, adding just a little more info to traceback to where the call happened.  When using an ide like visual studio, you can click on the logging line to take you to that line of code.  LogMarker handles a majority of this.
* Break behavior can be configured.  You can choose to break on a log statement, continue, or ignore all future logs of this type.  This is helpful in case there is a breaking log that you wish to stop seeing/breaking, like if an art resource is missing.
* All logging subscriptions commands are exposed to the scripting TODO: LINK system, which allows for per-user configuration at runtime.

A note on macro programming... You may have notices that ConsoleLogger and ScreenReporter have some heavy macro programming.  While ugly and generally hard to follow, it's highly important to use for the logging systems:

* Global access - throwing this in the pre-compiled header gives access to logging at all times
* Reducing bookkeeping: using macros allows use of __FILE__, __LINE__, __FUNCTION__ macros, which lets the system embed that information at the call, removing the need for the caller to do so.
* Removable - Finally, and probably the most important one, is that we can dead strip the entire logging system from the runtime.  When building a release build, we need all the compute cycles we can get, and even the overhead of calling an empty function is too much.  Dead stripping removes all the logging code from the executable, which frees up cycles and memory.  For a small embedded system like the Nintendo DS, the logging overhead can take up as much as a 1/4 of the runtime memory available, so ensuring it is removed is paramount. 