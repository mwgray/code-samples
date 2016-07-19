namespace Graymatter.Platformer.Systems.EntitySystem.Views.Items
{
    public abstract class ItemPedestalViewBase: View<ItemPedestal>
    {
        public ItemPedestal ItemPedestal
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._ItemProperty.Subscribe(_=>OnItemChanged());
            Data._stateProperty.Subscribe(_=>OnStateChanged());

            Dirty();
        }

        public abstract void OnItemChanged();
        public abstract void OnStateChanged();
    }
}