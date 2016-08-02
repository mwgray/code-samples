# Process

The process classes are a way of compartmentalizing sections of code into objects that can be grouped and re-arranged.  Once a process is kicked off you wait for the end event to be fired to indicate it is finished with the process.  Similar to Apple's NSOperation class.  

[Dependency Graphs](/DependencyGraph) has a complex example of using processes.  

* [BatchProcess.h](BatchProcess.h)/[cpp](BatchProcess.cpp) is a way to execute a list of processes in order.  It will execute a sub-process, and when it is complete it will execute the next in the list.
* [DummyProcess.h](DummyProcess.h)/[cpp](DummyProcess.cpp) is just an empty process.  It's useful to have null or instant processes like this.  I use this in [Dependency Graphs](/DependencyGraph) for dependency gates.
* [Process.h](Process.h)/[cpp](Process.cpp) is the base class used for Process.  Simply subclass it and override onBegin and you have your custom process!  Fires a begin and end event.
* [ProcessEvent.h](ProcessEvent.h)/[cpp](ProcessEvent.cpp) is the event class the process uses.  Processes are [Event Dispatchers](/Events), so it's useful to attach to begin and end events.
* [ProcessManager.h](ProcessManager.h)/[cpp](ProcessManager.cpp) is just a bookkeeping class.  It keeps track of all active processes.  Useful for tracking dead or abandoned processes.
