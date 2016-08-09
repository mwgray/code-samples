# Model View Controller Generator

# DISCLAIMER 
This code isn't very well commented or documented.  I included it because it's the kind of thing I really like doing, creating tools tha simplify some of the tedious aspects of day-to-day programming.  If have looked at my other samples and you are still interested in how my coding brain works, go for it!  But do check out my other samples first, they are much more coherent :-)

####Overview

This is a tool I wrote to facilitate writing the 'glue' code for traditional [Model-View-Controller](https://en.wikipedia.org/wiki/Model%E2%80%93view%E2%80%93controller) systems.  More often than not I find myself bogged down in writing maintaining bindings for these types of classes, when a lot of it can be automated.  Thus this system was born.

It was written in C#, so I can take advantage of C# Attributes and reflection to create a system of hints for an exporter.

####Thoughts

Using C# as the source date format was an quick way to get this system off the ground, but it ended up being tedious in the end.  I did so to avoid having to write my own parser, and annotating classes with attributes seemed like a quick way to get things started.  It worked out pretty well at the beginning when I was building the tool, but once I started using it for my own projects the cracks began to show.  The main problem was any model changes required they tool to be recompiled and run, which in turn requires the main project to be compiled an run again.  I think what's there is a good foundation, it's just the input needs changing to a format that is data-driven, and easy to read. 

####Summaries of folders

* [MVCPreCompiler](MVCPreCompiler/) is the tool that compiles the project.  It is divided into the code that does the C# class parsing, and templates, which are used to output the resultant C# files.
* [PlatformerDataModel](PlatformerDataModel/) is the source and output format from a project I used this on.
    * [Systems](PlatformerDataModel/src/Platformer/Systems/) contains the source files with classes that have attributes for the exporter.
    * [EXTERNAL](PlatformerDataModel/src/Platformer/EXTERNAL/) is a file with mock classes that are needed by the exporter tool, but are included in the fuller project.
    * [Output](PlatformerDataModel/Output/) contains the source files with classes that have attributes for the exporter.  Each System has a `$Generated` file which sets up all the scaffolding code.  In addition, each concrete class has its own file.
