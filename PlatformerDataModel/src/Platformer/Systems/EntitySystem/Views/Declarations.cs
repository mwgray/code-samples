using Graymatter.Code.Attributes;
using Graymatter.Code.Declarations;
using Graymatter.Platformer.Systems.DungeonSystem.Models;

namespace Graymatter.Platformer.Systems.DungeonSystem.Views
{
    public class RoomViewDeclaration: ViewDeclaration<RoomDeclaration>
    {
    }

    public class WorldRoomViewDeclaration: RoomViewDeclaration
    {
    }

    public abstract class MinimapRoomViewDeclaration: RoomViewDeclaration
    {
    }

    public abstract class WorldLevelViewDeclaration: ViewDeclaration<LevelDeclaration>
    {
        [Dependencies("rooms#Add")]
        protected abstract void OnRoomAdded(RoomDeclaration roomDeclaration);

        [Dependencies("rooms#Remove")]
        protected abstract void OnRoomRemoved(RoomDeclaration roomDeclaration);
    }
}