namespace Graymatter.Platformer.Systems.EntitySystem.Views.Doors
{
    public abstract class DoorEntityViewBase: View<DoorEntity>
    {
        public DoorEntity DoorEntity
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._isClosedProperty.Subscribe(_=>OnIsClosedChanged());

            Dirty();
        }

        public abstract void OnIsClosedChanged();
    }
}