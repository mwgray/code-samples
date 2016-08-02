# Binding

So where as the [Scripting](/Scripting) system generally is for binding functions, the Binding system is for binding variables.  It also integrates with the Lua Scripting, so lua scripts can control bound members.  It's a bit simpler to do variable binding in C++ than functions because functors are complex.

To Bind a member the following call is made:
```Bind::Member(muteMusic, this, "muteMusic")```

In this example the member variable `muteMusic` is exposed to lua as a global variable `muteMusic`.

Here's some summaries of the files: 

* __Bind__ is the main entry point for binding system.
* __BoundMember__ is the base storage class for bindings.  Holds the name of the variable, comments, and if it's a one-time boot variable.
* __BoundTypeMember__ holds templated subclasses of BoundMember, for primitives int, float, bool and c strings.