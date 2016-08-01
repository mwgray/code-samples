This collection of files simplify the headache that comes with binding C++ code to Lua.  The usual lua library requires knowledge of the lua stack and can generally get pretty complicated to follow and debug.  I wrote these files to make it easier for others to bind c++ functions to a Lua runtime context.

Binding is as simple as getting a LuaContext and calling bind on it:
`LuaScriptManager::Instance().GetContext("debug").bind("log", this, &ConsoleLogger::SubscribeTo);`

In this example, we are adding a function `log` to the `debug` context, which will execute the `ConsoleLogger::SubscribeTo` function when called from lua.  Note there is no need to clarify parameters, the system knows how to interpret all of it on it's own.

At first glance the binding code is a bit to take in.  This is mainly because this code was written before C++ had variadic template parameters and I had to explictly make a function for each parameter count.  Each function 'group' is a set of template functions, with a specialization for 0 to 5 parameters.  Templates are also specialize with float, int, string, bool, since they are the only types easily translatable to Lua.

Here's some summaries of the files: 

* __LuaScriptManager__ is the entry point for most users.  Calling `LuaScriptManager::Instance().GetContext("context") will return a LuaContext, which you can then use to bind functions to.  You can then call `executeScript` on a LuaContext to run any script, with the bound functions exposed to lua script.
* __CoreLuaScript__ - Class that actually executes the lua scripts.  It links to the actual lua lib, which is outside of the scope of this example.
* __LuaBindingGameMode__ - example of some lua tests
* __LuaContext__ - exposes a lot of templated functions for binding global and class functions.
* __LuaContextTests__ - some tests for using LuaContexts
* __LuaFunctionBinding__ - this is where a lot of the meat is.  LuaFucntionBinding is a base class for templated sub-classes.
* __LuaFunctionBinding.cpp__ uses partial template specialization to extract parameters from a lua function call and inject a return value into the lua runtime when the C++ function is complete.  Using templates enforces parameter types on the lua side, so there's no implicit types when going from C++ to lua.
* __LuaFunctionBinding.h__ is the base class for three types of function bindings.  Most of these are just a group of template functions varying with the parameter counts of 0 to 5.  They setup the parameter injection and return value extraction.
* __LuaMemberFunctionBindings.h__ is for binding to class member functions.
* __LuaMemberProcedureBindings.h__ is like function bindings, but since templates get (even more) messy with return values, it's easier to just omit the return value of void from the template.  These are just functions with void return value.
* __LuaGlobalProcedureBindings.h__ is for binding to global functions with a void return value.

