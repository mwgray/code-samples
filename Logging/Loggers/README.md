# Loggers

These are a pair of loggers I've used in many console projects.

  * [ConsoleLogger.h](ConsoleLogger.h)/[cpp](ConsoleLogger.cpp): A simple TTY logger.  Spits out logs to the consoles TTY using PlatformPrint.  Also has some break/continue/ignore functionality. 
  * [ScreenReporter.h](ScreenReporter.h)/[cpp](ScreenReporter.cpp): An on screen logger.  Log calls are rendered on top of the render target, and must be called every frame.  This allows for viewing specific values change over time, which can be easier to read when running at 60fps.  It also reduces console log spam, since these calls can be seen on screen, as opposed to in bulk in the console log.
  * [LogMarker.h](LogMarker.h)/[cpp](LogMarker.cpp): a logging helper class, keeps track if the log line is enabled.
  * [PlatformPrint.h](PlatformPrint.h)/[cpp](PlatformPrint.cpp): a #define-d file that determines which functions to use when logging based on the target platform.  This one was used for Win32 and Sony Playstation platforms.

 