namespace Graymatter.Platformer.Systems.InventorySystem.Models.Restrictions
{
    public abstract class ChargeItemRestrictionBase: ItemRestriction
    {
        // charges ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<int> _chargesProperty = new ReactiveProperty<int>();

        [SerializeField]
        public int charges
        {
            get { return _chargesProperty.Value; }
            set { _chargesProperty.Value = value; }
        }
        // chargesNeededPerUse ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<int> _chargesNeededPerUseProperty = new ReactiveProperty<int>();

        [SerializeField]
        public int chargesNeededPerUse
        {
            get { return _chargesNeededPerUseProperty.Value; }
            set { _chargesNeededPerUseProperty.Value = value; }
        }

    }
    public abstract class ItemRestrictionBase: Data
    {
        public abstract bool TryUse();
    }
    public abstract class PayAvatarHealthItemRestrictionBase: ItemRestriction
    {
    }
    public abstract class PayAvatarResourceItemRestrictionBase: ItemRestriction
    {
    }
}