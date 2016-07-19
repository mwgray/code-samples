namespace Graymatter.Platformer.Systems.EntitySystem.Models.Monsters
{
    public abstract class MonsterBase: Data
    {
        // life ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<int> _lifeProperty = new ReactiveProperty<int>();

        [SerializeField]
        public int life
        {
            get { return _lifeProperty.Value; }
            set { _lifeProperty.Value = value; }
        }
        // isAlive ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _isAliveProperty = new ReactiveProperty<bool>();

        [ShowInInspector][InspectorDisabled]
        public bool isAlive
        {
            get { return _isAliveProperty.Value; }
            private set { _isAliveProperty.Value = value; }
        }

        private void UpdateisAlive()
        {
            isAlive = CalculateisAlive();
        }

        protected abstract bool CalculateisAlive();

        protected MonsterBase()
        {
            _lifeProperty.Subscribe(_ => UpdateisAlive());

            UpdateisAlive();

        }

    }
    public abstract class MonsterSpawnBase: Entity
    {
        public bool spawnFlipped;
        // active ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _activeProperty = new ReactiveProperty<bool>();

        [ShowInInspector][InspectorDisabled]
        public bool active
        {
            get { return _activeProperty.Value; }
            set { _activeProperty.Value = value;
            }
        }
        // wasKilled ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _wasKilledProperty = new ReactiveProperty<bool>();

        [SerializeField]
        public bool wasKilled
        {
            get { return _wasKilledProperty.Value; }
            set { _wasKilledProperty.Value = value; }
        }

    }
}