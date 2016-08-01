# Logging

This is a collection of files that create a logging system.  Two concrete implementations are included:

  * __ConsoleLogger__: A simple TTY logger. 
  * __ScreenReporter__: An on screen logger.  Log calls are rendered on top of the render target, and must be called every frame.  This allows for viewing specific values change over time, which can be easier to read when running at 60fps.  It also reduces console log spam, since these calls can be seen on screen, as opposed to in bulk in the console log.

The basic usage comes down to logging call, which consists of the the following components:

* A __category__, which allows for filtering during runtime.  For example, ui related logging uses the category "UI", physics would use "Physics", etc.
* __Severity__, which is the traditional usual fatal error, warn, info, debug, trace.  Additionally, there is force, which basically forces a logging output, regardless of the category / severity. 
* The __message__ itself, which is a printf formatted string followed by the parameters.

In order for a user to see a given log statement, they must be subscribed to that category. This offers some advantages:

 * This system reduces logging overhead.  Printf evaluation will no occur unless the log is determined to be printed, so it increases performance, which is helpful when you only have 16 ms per frame.
 * Users can configure which log statements they wish to see at runtime, reducing logging spam.  UI programmers don't need to see Physics logs and visa versa.
 * While you could achieve this with tools like grep and cat, the performance is very important for real-time systems, and logging overhead is enough to make frame rate suffer.

Some additional features:
 
* __Platform separation__ - PlatformPrint handles most of the platform separation.  In this project, it was Windows and Non-Windows platforms, which fit nicely into one file.  For projects with more platforms, you would separate these out into their own files. 
* __Call tracking__ - each log call has it's file/line/function stored within the call, adding just a little more info to traceback to where the call happened.  When using an ide like visual studio, you can click on the logging line to take you to that line of code.  LogMarker handles a majority of this.
* __Break behavior__ can be configured.  You can choose to break on a log statement, continue, or ignore all future logs of this type.  This is helpful in case there is a breaking log that you wish to stop seeing/breaking, like if an art resource is missing.
* __Scripting__ - All logging subscriptions commands are exposed to the scripting TODO: LINK system, which allows for per-user configuration at runtime.

A note on macro programming... You may have notices that ConsoleLogger and ScreenReporter have some heavy macro programming.  While the setup code isn't the easiest to read, it's highly important to use for the logging systems:

* __Global access__: throwing this in the pre-compiled header gives access to logging at all times
* __Reduce bookkeeping__: using macros allows use of `__FILE__`, `__LINE__`, `__FUNCTION__` (TODO: cleanup) macros, which lets the system embed that information at the call, removing the need for the caller to do so.
* __Removable__ - Finally, and probably the most important one, is that we can dead strip the entire logging system from the runtime.  When building a release build, we need all the compute cycles we can get, and even the overhead of calling an empty function is too much.  Dead stripping removes all the logging code from the executable, which frees up cycles and memory.  For a small embedded system like the Nintendo DS, the logging overhead can take up as much as a 1/4 of the runtime memory available, so ensuring it is removed is paramount. 