# code-samples
More details are provided in each folder's accompanying README.md

Here are some quick summaries:

* Debugging facilities - when ever I start a new project or working on a new platform, I ensure the following systems are available.
 * A robust [Logging](/Debugging/Logging) system.
 * A [Binding](/Debugging/Binding) system with which to change variables at runtime.
 * A [Scripting](/Debugging/Scripting) to facilitate changing bound variables.
* The Process and Dependency Graph systems work together closely to help keep initialization requirements decoupled and 
 * [Process](/Process) is a collection of classes that represent a task of code from start to finish
 * [Dependency Graph](/Graph/Dependency) allows one to setup a dependency graph to ensure that certain tasks(like a Process) only execute once all its dependencies are resolved.
* MVC
* [Singleton](/Singleton) is a templated version of the singleton design pattern.  Don't leave home without it!
* [Tweener](/Tweener) is a simple tweening system I wrote, mainly to facilitate smooth transitions in UI code.
* [UI](/UI) is a rather old UI system that I wrote for the [Nintendo DS](https://en.wikipedia.org/wiki/Nintendo_DS) platform

* [Word Cache](/WordCache) is an algorithm I wrote for the [Doki-Doki Universe](https://www.youtube.com/watch?v=H7hFijr5v-c) instant messenger app.  The system needed a way highlight phrases in a user typed message using a dictionary with ~100k words.

As I wrote this, I realized this is an all encompassing UI system design, take from it what you will...

Game projects typically have millions of LoC and long compile build times, so each tweak of a variable can take minutes for developers.  These are systems that facilitate that kind of work flow.

Similarly, when a game is shipped, every cycle counts, so being able to strip a lot of the debugging code out of the project is a necessity.  To this end a lot of macro/template meta-programming is used to configure what code is present at compile time.

My philosophy is to make the end code readable and the setup code complex.