namespace Graymatter.Platformer.Systems.EntitySystem.Models.Items
{
    public abstract class ItemPedestalBase: Entity
    {
        // ItemPoolType ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<ItemPoolType> _ItemPoolTypeProperty = new ReactiveProperty<ItemPoolType>();

        [SerializeField]
        public ItemPoolType ItemPoolType
        {
            get { return _ItemPoolTypeProperty.Value; }
            set { _ItemPoolTypeProperty.Value = value; }
        }
        [InspectorCollapsedFoldout] // Item ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<Item> _ItemProperty = new ReactiveProperty<Item>();

        [SerializeField]
        public Item Item
        {
            get { return _ItemProperty.Value; }
            set { _ItemProperty.Value = value; }
        }
        // state ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<ItemPedestalState> _stateProperty = new ReactiveProperty<ItemPedestalState>();

        [SerializeField]
        public ItemPedestalState state
        {
            get { return _stateProperty.Value; }
            set { _stateProperty.Value = value; }
        }

    }
    public abstract class RandomShopItemBase: Entity
    {
    }
}