namespace Graymatter.Platformer.Systems.EntitySystem.Models.Doors
{
    public abstract class BossDoorEntityBase: DoorEntity
    {
    }
    public abstract class DoorEntityBase: EntityContainer
    {
        // isClosed ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _isClosedProperty = new ReactiveProperty<bool>();

        [ShowInInspector][InspectorDisabled]
        public bool isClosed
        {
            get { return _isClosedProperty.Value; }
            private set { _isClosedProperty.Value = value; }
        }

        private void UpdateisClosed()
        {
            isClosed = CalculateisClosed();
        }

        protected abstract bool CalculateisClosed();

        // doorLocks ////////////////////////////////////////////////////////////
        [ShowInInspector][InspectorDisabled][InspectorCollapsedFoldout]
        public DataCollection<DoorLockEntity> doorLocks
        {
            get { return _doorLocks; }
            set
            {
                _doorLocks = value;

                if(doorLocks != null)
                {
                    doorLocks.BindingsCreator = doorLocksBindingsCreator;

                    _doorLocks.ObserveReplace().Subscribe(_ => UpdateisClosed());
                    _doorLocks.ObserveAdd().Subscribe(_ => UpdateisClosed());
                    _doorLocks.ObserveRemove().Subscribe(_ => UpdateisClosed());
                }
            }
        }
        private DataCollection<DoorLockEntity> _doorLocks;

        private List<IDisposable> doorLocksBindingsCreator(DoorLockEntity value)
        {
            if(value == default(DoorLockEntity))
            {
                return new List<IDisposable>();
            }

            return new List<IDisposable>()
                    {
                        value._isLockedProperty.Subscribe(_ => UpdateisClosed()),
                    };
        }

        protected abstract void UpdatedoorLocks();

        protected DoorEntityBase()
        {
            doorLocks = new DataCollection<DoorLockEntity>();

            UpdateisClosed();

        }

    }
    public abstract class NormalDoorEntityBase: DoorEntity
    {
    }
    public abstract class SpikeyDoorEntityBase: DoorEntity
    {
    }
    public abstract class TreasureDoorEntityBase: DoorEntity
    {
    }
    public abstract class WallEntityBase: DoorEntity
    {
    }
}