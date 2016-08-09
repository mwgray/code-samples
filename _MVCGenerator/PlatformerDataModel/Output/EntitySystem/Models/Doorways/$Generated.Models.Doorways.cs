namespace Graymatter.Platformer.Systems.EntitySystem.Models.Doorways
{
    public abstract class BottomDoorwayBase: DoorwayEntity
    {
    }
    public abstract class DoorwayEntityBase: EntityContainer
    {
        public Vector2i doorIndex;

    }
    public abstract class LeftDoorwayBase: DoorwayEntity
    {
    }
    public abstract class RightDoorwayBase: DoorwayEntity
    {
    }
    public abstract class TopDoorwayBase: DoorwayEntity
    {
    }
}