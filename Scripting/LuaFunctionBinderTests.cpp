// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "LuaFunctionBinder.h"

// extern and call testlua to use; TODO.m - probably should unit test this huh...

namespace core {

    class Event;

    class Test
    {
    public:
        void set0() { logWarn("Set0", "!"); }
        void set1(int p1) { logWarn("Set1", "%d", p1); }
        void set2(int p1, int p2) { logWarn("Set2", "%d, %d", p1, p2); }
        void set3(int p1, int p2, int p3) { logWarn("Set3", "%d, %d, %d", p1, p2, p3); }
        void set4(int p1, int p2, int p3, int p4) { logWarn("Set4", "%d, %d, %d, %d", p1, p2, p3, p4); }
        void set5(int p1, int p2, int p3, int p4, int p5) { logWarn("Set5", "%d, %d, %d, %d, %d", p1, p2, p3, p4, p5); }
    };

    void gset0() { logWarn("gSet0", "!"); }
    void gset1(int p1) { logWarn("gSet1", "%d", p1); }
    void gset2(int p1, int p2) { logWarn("gSet2", "%d, %d", p1, p2); }
    void gset3(int p1, int p2, int p3) { logWarn("gSet3", "%d, %d, %d", p1, p2, p3); }
    void gset4(int p1, int p2, int p3, int p4) { logWarn("gSet4", "%d, %d, %d, %d", p1, p2, p3, p4); }    
    void gset5(int p1, int p2, int p3, int p4, int p5) { logWarn("gSet5", "%d, %d, %d, %d, %d", p1, p2, p3, p4, p5); }

    Test t;

    void testLua()
    {
        LuaFunctionBinder c;

        c.bind("set0", &t, &Test::set0);
        c.bind("set1", &t, &Test::set1);
        c.bind("set2", &t, &Test::set2);
        c.bind("set3", &t, &Test::set3);
        c.bind("set4", &t, &Test::set4);
        c.bind("set5", &t, &Test::set5);
        
        c.bindGlobal("gset0", &gset0);
        c.bindGlobal("gset1", &gset1);
        c.bindGlobal("gset2", &gset2);
        c.bindGlobal("gset3", &gset3);
        c.bindGlobal("gset4", &gset4);
        c.bindGlobal("gset5", &gset5);

        c.executeString("set0();");
        c.executeString("set1(1);");
        c.executeString("set2(1, 2);");
        c.executeString("set3(1, 2, 3);");
        c.executeString("set4(1, 2, 3, 4);");
        c.executeString("set5(1, 2, 3, 4, 5);");

        c.executeString("gset0();");
        c.executeString("gset1(1);");
        c.executeString("gset2(1, 2);");
        c.executeString("gset3(1, 2, 3);");
        c.executeString("gset4(1, 2, 3, 4);");
        c.executeString("gset5(1, 2, 3, 4, 5);");
    }

} // namespace core