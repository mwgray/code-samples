using Graymatter.Code.Attributes;
using Graymatter.Code.Declarations;
using Graymatter.Platformer.Systems.AvatarSystem.Models;

namespace Graymatter.Platformer.Systems.AvatarSystem.Views
{
    public abstract class WorldAvatarViewDeclaration: ViewDeclaration<AvatarDeclaration>
    {
    }
    
    public abstract class AvatarResourceViewDeclaration: ViewDeclaration<AvatarResourceDeclaration>
    {
        [Dependencies("value")]
        protected abstract void Dirty();
    }

    public abstract class AvatarHealthViewDeclaration: ViewDeclaration<AvatarHealthDeclaration>
    {
        [Dependencies("heartContainers")]
        protected abstract void Dirty();
    }
}