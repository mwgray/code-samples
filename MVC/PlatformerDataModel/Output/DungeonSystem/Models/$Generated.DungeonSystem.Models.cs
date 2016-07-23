namespace Graymatter.Platformer.Systems.DungeonSystem.Models
{
    public abstract class LevelBase: Data
    {
        // rooms ////////////////////////////////////////////////////////////
        [SerializeField][InspectorCollapsedFoldout]
        public DataCollection<Room> rooms
        {
            get { return _rooms; }
            set
            {
                _rooms = value;

                if(rooms != null)
                {
                    rooms.BindingsCreator = roomsBindingsCreator;

                }
            }
        }
        private DataCollection<Room> _rooms;

        private List<IDisposable> roomsBindingsCreator(Room value)
        {
            if(value == default(Room))
            {
                return new List<IDisposable>();
            }

            return new List<IDisposable>()
                    {
                    };
        }

        protected LevelBase()
        {
            rooms = new DataCollection<Room>();

        }

    }
    public abstract class RoomBase: Data
    {
        // position ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<Vector2i> _positionProperty = new ReactiveProperty<Vector2i>();

        [SerializeField]
        public Vector2i position
        {
            get { return _positionProperty.Value; }
            set { _positionProperty.Value = value; }
        }
        // shape ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<RoomShape> _shapeProperty = new ReactiveProperty<RoomShape>();

        [SerializeField]
        public RoomShape shape
        {
            get { return _shapeProperty.Value; }
            set { _shapeProperty.Value = value; }
        }
        public RoomType type;
        // width ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<int> _widthProperty = new ReactiveProperty<int>();

        [ShowInInspector][InspectorDisabled]
        public int width
        {
            get { return _widthProperty.Value; }
            private set { _widthProperty.Value = value; }
        }

        private void Updatewidth()
        {
            width = Calculatewidth();
        }

        protected abstract int Calculatewidth();
        // height ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<int> _heightProperty = new ReactiveProperty<int>();

        [ShowInInspector][InspectorDisabled]
        public int height
        {
            get { return _heightProperty.Value; }
            private set { _heightProperty.Value = value; }
        }

        private void Updateheight()
        {
            height = Calculateheight();
        }

        protected abstract int Calculateheight();

        protected RoomBase()
        {
            _positionProperty.Subscribe(_ => Updatewidth());
            _shapeProperty.Subscribe(_ => Updatewidth());

            _positionProperty.Subscribe(_ => Updateheight());
            _shapeProperty.Subscribe(_ => Updateheight());

            Updatewidth();

            Updateheight();

        }

    }
}