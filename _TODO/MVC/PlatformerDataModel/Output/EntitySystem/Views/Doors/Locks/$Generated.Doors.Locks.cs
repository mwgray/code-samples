namespace Graymatter.Platformer.Systems.EntitySystem.Views.Doors.Locks
{
    public abstract class ClearRoomDoorLockEntityViewBase: View<ClearRoomDoorLockEntity>
    {
        public ClearRoomDoorLockEntity ClearRoomDoorLockEntity
        {
            get { return Data; }
            set { Data = value; }
        }

    }
    public abstract class DestroyableDoorLockEntityViewBase: View<DestroyableDoorLockEntity>
    {
        public DestroyableDoorLockEntity DestroyableDoorLockEntity
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._isLockedProperty.Subscribe(_=>OnLockedChanged());

            Dirty();
        }

        protected abstract void OnLockedChanged();
    }
    public abstract class HealthDoorLockEntityViewBase: View<HealthDoorLockEntity>
    {
        public HealthDoorLockEntity HealthDoorLockEntity
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._stateProperty.Subscribe(_=>Dirty());

            Dirty();
        }

    }
    public abstract class PayAvatarResourceDoorLockEntityViewBase: View<PayAvatarResourceDoorLockEntity>
    {
        public PayAvatarResourceDoorLockEntity PayAvatarResourceDoorLockEntity
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._avatarResourceProperty.Subscribe(_=>OnAvatarResourceChanged());
            Data._neededProperty.Subscribe(_=>OnNeededChanged());
            Data._paidProperty.Subscribe(_=>OnPaidChanged());

            Dirty();
        }

        protected abstract void OnAvatarResourceChanged();
        protected abstract void OnNeededChanged();
        protected abstract void OnPaidChanged();
    }
}