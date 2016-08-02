namespace Graymatter.Platformer.Systems.EntitySystem.Views.Doorways
{
    public abstract class DoorwayEntityViewBase: View<DoorwayEntity>
    {
        public DoorwayEntity DoorwayEntity
        {
            get { return Data; }
            set { Data = value; }
        }

    }
}