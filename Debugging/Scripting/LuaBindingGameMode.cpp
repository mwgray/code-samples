// Copyright 2006-12 HumaNature Studios Inc.

#include "CorePch.h"

#include "GameMode/GameMode.h"

#include "LuaContext.h"

namespace core {

    class LuaBindingGameMode
        : public GameMode
    {
    public:
        virtual void Initialize(PropertyObject* properties)
        {
            GameMode::Initialize(properties);

            LuaContext context;

            const char* createObjectScript = "object = { builtInFunction = function() return 2.3; end; subobject = { builtInFunction = function() return 4.5; end; }; };";            
            context.executeString(createObjectScript);

            context.bind("object.boundFunction", this, &LuaBindingGameMode::ReturnFloat);
            context.bind("object.subobject.boundFunction", this, &LuaBindingGameMode::ReturnSubFloat);

            context.executeString("print('built-in returned:'..object.builtInFunction());");
            context.executeString("print('built-in sub returned:'..object.subobject.builtInFunction());");

            context.executeString("print(object.boundFunction);");

            context.executeString("print('bound returned: '..object.boundFunction());");
            context.executeString("print('bound sub returned: '..object.subobject.boundFunction());");
        }

        float ReturnFloat()
        {
            return 1.5f;
        }

        float ReturnSubFloat()
        {
            return 0.3f;
        }

        virtual void Update(float deltaSeconds)
        {	
            GameMode::Update(deltaSeconds);
        }

        virtual void Finalize()
        {
            GameMode::Finalize();
        }
    };

	REGISTER_FACTORY_CLASS_NS(core, LuaBindingGameMode, GameMode);

} // namespace core