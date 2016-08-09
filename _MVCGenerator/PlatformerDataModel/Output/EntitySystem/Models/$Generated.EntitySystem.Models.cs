namespace Graymatter.Platformer.Systems.EntitySystem.Models
{
    public abstract class EntityContainerBase: Entity
    {
        // children ////////////////////////////////////////////////////////////
        [SerializeField][InspectorCollapsedFoldout]
        public DataCollection<Entity> children
        {
            get { return _children; }
            set
            {
                _children = value;

                if(children != null)
                {
                    children.BindingsCreator = childrenBindingsCreator;

                    _children.ObserveAdd().Subscribe(_ => OnChildAdded(_.Value));

                    _children.ObserveRemove().Subscribe(_ => OnChildRemoved(_.Value));

                    _children.ObserveReplace().Subscribe(_ => OnChildReplaced(_.NewValue, _.OldValue));
                }
            }
        }
        private DataCollection<Entity> _children;

        private List<IDisposable> childrenBindingsCreator(Entity value)
        {
            if(value == default(Entity))
            {
                return new List<IDisposable>();
            }

            return new List<IDisposable>()
                    {
                    };
        }

        protected EntityContainerBase()
        {
            children = new DataCollection<Entity>();

        }

        public abstract void OnChildAdded(Entity entity);
        public abstract void OnChildRemoved(Entity entity);
        public abstract void OnChildReplaced(Entity newValue, Entity oldValue);
    }
    public abstract class EntityBase: Data
    {
        public Vector3i localPosition;

    }
    public abstract class LevelEntityBase: EntityContainer
    {
        // bounds ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<RectInt> _boundsProperty = new ReactiveProperty<RectInt>();

        [ShowInInspector][InspectorDisabled]
        public RectInt bounds
        {
            get { return _boundsProperty.Value; }
            private set { _boundsProperty.Value = value; }
        }

        private void Updatebounds()
        {
            bounds = Calculatebounds();
        }

        protected abstract RectInt Calculatebounds();
        // visibleBounds ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<RectInt> _visibleBoundsProperty = new ReactiveProperty<RectInt>();

        [ShowInInspector][InspectorDisabled]
        public RectInt visibleBounds
        {
            get { return _visibleBoundsProperty.Value; }
            private set { _visibleBoundsProperty.Value = value; }
        }

        private void UpdatevisibleBounds()
        {
            visibleBounds = CalculatevisibleBounds();
        }

        protected abstract RectInt CalculatevisibleBounds();
        // currentRoom ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<RoomEntity> _currentRoomProperty = new ReactiveProperty<RoomEntity>();

        [ShowInInspector][InspectorDisabled]
        public RoomEntity currentRoom
        {
            get { return _currentRoomProperty.Value; }
            set { _currentRoomProperty.Value = value;
            }
        }

        // rooms ////////////////////////////////////////////////////////////
        [ShowInInspector][InspectorDisabled][InspectorCollapsedFoldout]
        public DataCollection<RoomEntity> rooms
        {
            get { return _rooms; }
            set
            {
                _rooms = value;

                if(rooms != null)
                {
                    rooms.BindingsCreator = roomsBindingsCreator;

                    _rooms.ObserveReplace().Subscribe(_ => Updatebounds());
                    _rooms.ObserveAdd().Subscribe(_ => Updatebounds());
                    _rooms.ObserveRemove().Subscribe(_ => Updatebounds());
                }
            }
        }
        private DataCollection<RoomEntity> _rooms;

        private List<IDisposable> roomsBindingsCreator(RoomEntity value)
        {
            if(value == default(RoomEntity))
            {
                return new List<IDisposable>();
            }

            return new List<IDisposable>()
                    {
                        value._isVisibleProperty.Subscribe(_ => UpdatevisibleBounds()),
                    };
        }

        protected abstract void Updaterooms();

        protected LevelEntityBase()
        {
            rooms = new DataCollection<RoomEntity>();

            _boundsProperty.Subscribe(_ => UpdatevisibleBounds());

            Updatebounds();

            UpdatevisibleBounds();

        }

    }
    public abstract class ReferenceEntityBase: Entity
    {
        [InspectorCollapsedFoldout] public EntityAsset reference;

    }
    public abstract class RoomEntityBase: EntityContainer
    {
        public Room room = new Room();
        // state ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<RoomState> _stateProperty = new ReactiveProperty<RoomState>();

        [SerializeField]
        public RoomState state
        {
            get { return _stateProperty.Value; }
            set { _stateProperty.Value = value; }
        }
        // typeKnowledge ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<RoomTypeKnowledge> _typeKnowledgeProperty = new ReactiveProperty<RoomTypeKnowledge>();

        [SerializeField]
        public RoomTypeKnowledge typeKnowledge
        {
            get { return _typeKnowledgeProperty.Value; }
            set { _typeKnowledgeProperty.Value = value; }
        }
        // isVisible ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _isVisibleProperty = new ReactiveProperty<bool>();

        [ShowInInspector][InspectorDisabled]
        public bool isVisible
        {
            get { return _isVisibleProperty.Value; }
            private set { _isVisibleProperty.Value = value; }
        }

        private void UpdateisVisible()
        {
            isVisible = CalculateisVisible();
        }

        protected abstract bool CalculateisVisible();
        // isClear ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _isClearProperty = new ReactiveProperty<bool>();

        [ShowInInspector][InspectorDisabled]
        public bool isClear
        {
            get { return _isClearProperty.Value; }
            private set { _isClearProperty.Value = value; }
        }

        private void UpdateisClear()
        {
            isClear = CalculateisClear();
        }

        protected abstract bool CalculateisClear();
        // isOccupied ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _isOccupiedProperty = new ReactiveProperty<bool>();

        [ShowInInspector][InspectorDisabled]
        public bool isOccupied
        {
            get { return _isOccupiedProperty.Value; }
            set { _isOccupiedProperty.Value = value;
            }
        }

        // monsters ////////////////////////////////////////////////////////////
        [ShowInInspector][InspectorDisabled][InspectorCollapsedFoldout]
        public DataCollection<MonsterSpawn> monsters
        {
            get { return _monsters; }
            set
            {
                _monsters = value;

                if(monsters != null)
                {
                    monsters.BindingsCreator = monstersBindingsCreator;

                }
            }
        }
        private DataCollection<MonsterSpawn> _monsters;

        private List<IDisposable> monstersBindingsCreator(MonsterSpawn value)
        {
            if(value == default(MonsterSpawn))
            {
                return new List<IDisposable>();
            }

            return new List<IDisposable>()
                    {
                        value._wasKilledProperty.Subscribe(_ => UpdateisClear()),
                    };
        }

        protected abstract void Updatemonsters();

        protected RoomEntityBase()
        {
            monsters = new DataCollection<MonsterSpawn>();

            _stateProperty.Subscribe(_ => UpdateisVisible());



            UpdateisVisible();

            UpdateisClear();

        }

    }
    public abstract class TileLayerEntityBase: EntityContainer
    {
        public string name;
        public Vector3 cellSize;
        public Vector2i layerSize;

    }
}