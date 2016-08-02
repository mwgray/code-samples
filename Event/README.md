# Events

This is a simple event system I wrote for a UI system.  Event Dispatchers can be listened to for specific events, or all events.  One can listen for an event by calling addEventListener, like BatchProcess does in the [Process](/Process) samples:

```
mCurrentProcess->addEventListener(ProcessEvent::PROCESS_ENDED, this, &BatchProcess::handleProcessEnded, true);
```

This listens for the process ended event, and calls handleProcessEnded when it is fired.  The final parameter removes the listener after the dispatch.

Summaries of the files: 

* [Event.h](Event.h)/[cpp](Event.cpp) is the base Event class.  Simply subclass this to create custom events with parameters.
* [EventDispatcher.h](EventDispatcher.h)/[cpp](EventDispatcher.cpp) does most of the work.  Add/removing listeners.  Templated functions simplify fucntor passing for the callers.
* [EventListener.h](EventListener.h)/[cpp](EventListener.cpp) are templated functor wrapper classes.  Event Dispatcher uses these to store callbacks.
* [EventListenerList.h](EventListenerList.h)/[cpp](EventListenerList.cpp) are just list variants that Event Dispatcher uses to store multiple callbacks.
