namespace Graymatter.Platformer.Systems.InventorySystem.Models
{
    public abstract class InventoryBase: Data
    {
        [InspectorCollapsedFoldout] // activatedItem ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<ActivatedItem> _activatedItemProperty = new ReactiveProperty<ActivatedItem>();

        [SerializeField]
        public ActivatedItem activatedItem
        {
            get { return _activatedItemProperty.Value; }
            set { _activatedItemProperty.Value = value; }
        }
        [InspectorCollapsedFoldout] // oneShotItem ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<Item> _oneShotItemProperty = new ReactiveProperty<Item>();

        [SerializeField]
        public Item oneShotItem
        {
            get { return _oneShotItemProperty.Value; }
            set { _oneShotItemProperty.Value = value; }
        }

        // items ////////////////////////////////////////////////////////////
        [SerializeField][InspectorCollapsedFoldout]
        public DataCollection<Item> items
        {
            get { return _items; }
            set
            {
                _items = value;

                if(items != null)
                {
                    items.BindingsCreator = itemsBindingsCreator;

                }
            }
        }
        private DataCollection<Item> _items;

        private List<IDisposable> itemsBindingsCreator(Item value)
        {
            if(value == default(Item))
            {
                return new List<IDisposable>();
            }

            return new List<IDisposable>()
                    {
                    };
        }

        protected InventoryBase()
        {
            items = new DataCollection<Item>();

        }

    }
}