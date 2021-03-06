# code-samples
Here is a collection of some systems from my most recent projects.  More details are provided in each folder's accompanying README.md.

Here's a quick rundown of the systems.  __Bold__ systems include buildable projects.

* Whenever I start a new project or working on a new platform, I ensure a good set of debugging facilities are available.  __[Logging](/Logging)__, [Variable Binding](/Binding) and [Runtime Scripting](/Scripting) are a good start.    
* The [Process](/Process), [Event](/Event) and [Dependency Graph](/DependencyGraph) systems work together closely to help keep initialization requirements decoupled and execute in the correct order.
* __[WordCache](/WordCache)__ is an data structure and algorithm I wrote to highlight a string using words supplied by a dictionary of multi word phrases. 
* [Singleton](/Singleton) is a templated version of the singleton design pattern.  Don't leave home without it!
* [Compact Strings](/CompactString) are an optimization for strings when only comparisons are needed.
* __[Tweener](/Tweener)__ is a simple tweening system and easing algorithm collection I wrote, mainly to facilitate smooth transitions in UI code.
* If you've made it this far and are looking for some more, I've thown in a [Model View Controller generator](_MVCGenerator/) for you to pick at.  Be warned, it's has minimal comments/docs!

#### Improvements / Notes
* Most of this code was written prior to C++11 availability, so you're going to see a lot of template meta-magic.  If I were to write some of these today, I would likely take advantage of C++11's features to simplify the code.  Variadic templates and lambdas would simplify a lot of the code here.
* Some of these systems(especially in logging) may seem like going overboard with optimization and macro/template programming, but in game development every cycle counts.  You typically operate at 30/60 frames per second, and with VR you need even higher frame rates, so you only have 16 milliseconds(or less) per frame to do EVERYTHING.  Release builds must strip away anything that isn't relavant to the final product.
* It should go without saying, but the folders that have sample projects aren't the best project setups.  A lot of the scaffolding code(pch files, cmake projects) are there simply to offer a runnable example.  In my production environments these projects and dependencies would be much much cleaner :smile:
