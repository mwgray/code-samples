namespace Graymatter.Platformer.Systems.AvatarSystem.Views
{
    public abstract class AvatarHealthViewBase: View<AvatarHealth>
    {
        public AvatarHealth AvatarHealth
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data.heartContainers.ObserveReplace().Subscribe(_=> Dirty(_.NewValue, _.OldValue));
            Data.heartContainers.ObserveAdd().Subscribe(_=> Dirty(_.Value));
            Data.heartContainers.ObserveRemove().Subscribe(_=> Dirty(_.Value));

            Dirty();
        }

    }
    public abstract class AvatarResourceViewBase: View<AvatarResource>
    {
        public AvatarResource AvatarResource
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._valueProperty.Subscribe(_=>Dirty());

            Dirty();
        }

    }
    public abstract class WorldAvatarViewBase: View<Avatar>
    {
        public Avatar Avatar
        {
            get { return Data; }
            set { Data = value; }
        }

    }
}