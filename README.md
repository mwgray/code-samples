# code-samples
Here is a collection of some systems from my most recent projects.  More details are provided in each folder's accompanying README.md.

Here's a quick rundown of the systems

* Whenever I start a new project or working on a new platform, I ensure a good set of debugging facilities are available.  [Logging](/Debugging/Logging), [Variable Binding](/Debugging/Binding) and [Runtime Scripting](/Debugging/Scripting) are a good start.    
* The [Process](/Process), [Event](/Event) and [Dependency Graph](/Graph/Dependency) systems work together closely to help keep initialization requirements decoupled and execute in the correct order.
* [Singleton](/Singleton) is a templated version of the singleton design pattern.  Don't leave home without it!
* [Compact Strings](/CompactString) are an optimization for strings when only comparisons are needed.
* [Tweener](/Tweener) is a simple tweening system I wrote, mainly to facilitate smooth transitions in UI code.

#### Improvements / Notes
* Most of this code was written prior to C++11 availablity, so you're going to see a lot of template meta-magic.  If I were to write some of these today, I would likely take advantage of C++11's features to simplify the code.  Variadic templates and lambdas would simplify a lot of the code here.
* Some of these systems(especially in logging) may seem like going overboard with optimization and macro/template programming, but in game development every cycle counts.  You typically operate at 30/60 frames per second, and with VR you need even higher frame rates, so you only have 16 milliseconds per frame to do EVERYTHING.  Release builds must strip away anything that isn't relavant to the final product.

TODO:
* MVC
* [UI](/UI) is a rather old UI system that I wrote for the [Nintendo DS](https://en.wikipedia.org/wiki/Nintendo_DS) platform
* [Word Cache](/WordCache) is an algorithm I wrote for the [Doki-Doki Universe](https://www.youtube.com/watch?v=H7hFijr5v-c) instant messenger app.  The system needed a way highlight phrases in a user typed message using a dictionary with ~100k words.

As I wrote this, I realized this is an all encompassing UI system design, take from it what you will...

Game projects typically have millions of LoC and long compile build times, so each tweak of a variable can take minutes for developers.  These are systems that facilitate that kind of work flow.

Similarly, when a game is shipped, every cycle counts, so being able to strip a lot of the debugging code out of the project is a necessity.  To this end a lot of macro/template meta-programming is used to configure what code is present at compile time.

My philosophy is to make the end code readable and the setup code complex.

# System Desing
* Game dev doesn't have a lot of UI options available(at least not back)
* Would probably just get something off the shelf these days

#MVC
* Code annotations
* Detect dependencies
* Code generator
* Runtime uses subscriptsion to update dependeencies invisible to the user, be it caclcuatioed properties or views of the element

# UI
* Rather old UI system, but basics are there
* Go file by file ensure a decent structure is there
* watch out for function based events

# Word Cache
* point out the algorithm, describe it
* talk about tradeoffs, not big eough for speed
* might use a trie if written again