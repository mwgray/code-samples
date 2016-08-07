#TODO

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