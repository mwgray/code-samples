namespace Graymatter.Platformer.Systems.EntitySystem.Models.Doors.Locks
{
    public abstract class ClearRoomDoorLockEntityBase: DoorLockEntity
    {
        public bool canBombOpen;

    }
    public abstract class DestroyableDoorLockEntityBase: DoorLockEntity
    {
    }
    public abstract class DoorLockEntityBase: Entity
    {
        // isLocked ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _isLockedProperty = new ReactiveProperty<bool>();

        [SerializeField]
        public bool isLocked
        {
            get { return _isLockedProperty.Value; }
            set { _isLockedProperty.Value = value; }
        }

    }
    public abstract class HealthDoorLockEntityBase: DoorLockEntity
    {
        [InspectorCollapsedFoldout] // health ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<AvatarHealth> _healthProperty = new ReactiveProperty<AvatarHealth>();

        [ShowInInspector][InspectorDisabled]
        public AvatarHealth health
        {
            get { return _healthProperty.Value; }
            set { _healthProperty.Value = value;
            health._isLowProperty.Subscribe(_=>UpdateIsLocked());
            health._isFullProperty.Subscribe(_=>UpdateIsLocked());

            }
        }
        // state ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<AvatarHealthState> _stateProperty = new ReactiveProperty<AvatarHealthState>();

        [SerializeField]
        public AvatarHealthState state
        {
            get { return _stateProperty.Value; }
            set { _stateProperty.Value = value; }
        }

        protected HealthDoorLockEntityBase()
        {
            _stateProperty.Subscribe(_ => UpdateIsLocked());

        }

        protected abstract void UpdateIsLocked();
    }
    public abstract class PayAvatarResourceDoorLockEntityBase: DoorLockEntity
    {
        // avatarResource ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<AvatarResourceType> _avatarResourceProperty = new ReactiveProperty<AvatarResourceType>();

        [SerializeField]
        public AvatarResourceType avatarResource
        {
            get { return _avatarResourceProperty.Value; }
            set { _avatarResourceProperty.Value = value; }
        }
        // needed ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<int> _neededProperty = new ReactiveProperty<int>();

        [SerializeField]
        public int needed
        {
            get { return _neededProperty.Value; }
            set { _neededProperty.Value = value; }
        }
        // paid ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<int> _paidProperty = new ReactiveProperty<int>();

        [SerializeField]
        public int paid
        {
            get { return _paidProperty.Value; }
            set { _paidProperty.Value = value; }
        }

        protected PayAvatarResourceDoorLockEntityBase()
        {
            _neededProperty.Subscribe(_ => UpdateIsLocked());
            _paidProperty.Subscribe(_ => UpdateIsLocked());

        }

        protected abstract void UpdateIsLocked();
    }
}