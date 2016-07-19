namespace Graymatter.Platformer.Systems.InventorySystem.Views.Restrictions
{
    public abstract class ChargeItemRestrictionViewBase: View<ChargeItemRestriction>
    {
        public ChargeItemRestriction ChargeItemRestriction
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._chargesProperty.Subscribe(_=>OnChargeChanged());
            Data._chargesNeededPerUseProperty.Subscribe(_=>OnRechargeAmountChanged());

            Dirty();
        }

        public abstract void OnChargeChanged();
        public abstract void OnRechargeAmountChanged();
    }
}