using Graymatter.Code.Attributes;
using Graymatter.Code.Declarations;
using Graymatter.Platformer.Systems.EntitySystem.Models;
using Graymatter.Platformer.Systems.EntitySystem.Models.Doorways;

namespace Graymatter.Platformer.Systems.EntitySystem.Views
{
    public class EntityViewDeclaration: ViewDeclaration<EntityDeclaration> { }

    public class EntityContainerViewDeclaration: ViewDeclaration<EntityContainerDeclaration> { }

    public class LevelEntityViewDeclaration: EntityContainerViewDeclaration { }
    public class RoomEntityViewDeclaration: EntityContainerViewDeclaration { }

    public abstract class LevelEntityMinimapViewDeclaration: ViewDeclaration<LevelEntityDeclaration>
    {
        [Dependencies("currentRoom")]
        protected abstract void OnCurrentRoomChanged();

        [Dependencies("bounds")]
        protected abstract void OnBoundsChanged();

        [Dependencies("visibleBounds")]
        protected abstract void OnVisibleBoundsChanged();

        [Dependencies("rooms#Add")]
        protected abstract void OnRoomAdded(RoomEntityDeclaration room);

        [Dependencies("rooms#Remove")]
        protected abstract void OnRoomRemoved(RoomEntityDeclaration room);

        [Dependencies("rooms#Replace")]
        protected abstract void OnRoomReplaced(RoomEntityDeclaration newValue, RoomEntityDeclaration oldValue);
    }

    public abstract class RoomEntityMinimapViewDeclaration: ViewDeclaration<RoomEntityDeclaration>
    {
        [Dependencies("state", "typeKnowledge", "isOccupied", "isClear")]
        protected abstract void Dirty();
    }

    public class TileLayerEntityViewDeclaration: ViewDeclaration<TileLayerEntityDeclaration> { }

    public class ReferenceEntityViewDeclaration: ViewDeclaration<ReferenceEntityDeclaration> { }
}

namespace Graymatter.Platformer.Systems.EntitySystem.Views.Doorways
{
    public class DoorwayEntityViewDeclaration: ViewDeclaration<DoorwayEntityDeclaration>
    {

    }
}