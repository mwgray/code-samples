namespace Graymatter.Platformer.Systems.InventorySystem.Views
{
    public abstract class InventoryViewBase: View<Inventory>
    {
        public Inventory Inventory
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._activatedItemProperty.Subscribe(_=>OnActivatedItemChanged());
            Data._oneShotItemProperty.Subscribe(_=>OnOneShotItemItemChanged());
            Data.items.ObserveAdd().Subscribe(_=> OnItemAdded(_.Value));
            Data.items.ObserveRemove().Subscribe(_=> OnItemRemoved(_.Value));
            Data.items.ObserveReplace().Subscribe(_=> OnItemReplaced(_.NewValue, _.OldValue));

            Dirty();
        }

        public abstract void OnActivatedItemChanged();
        public abstract void OnOneShotItemItemChanged();
        public abstract void OnItemAdded(Item item);
        public abstract void OnItemRemoved(Item item);
        public abstract void OnItemReplaced(Item newValue, Item oldValue);
    }
}