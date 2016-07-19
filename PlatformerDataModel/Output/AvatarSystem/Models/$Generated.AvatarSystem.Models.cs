namespace Graymatter.Platformer.Systems.AvatarSystem.Models
{
    public abstract class AvatarBase: Data
    {
        public AvatarHealth health = new AvatarHealth();
        public AvatarResources resources = new AvatarResources();
        public Inventory inventory = new Inventory();

    }
    public abstract class AvatarHealthBase: Data
    {
        // heartTotals ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<HeartTotals> _heartTotalsProperty = new ReactiveProperty<HeartTotals>(new HeartTotals());

        [SerializeField]
        public HeartTotals heartTotals
        {
            get { return _heartTotalsProperty.Value; }
            set { _heartTotalsProperty.Value = value; }
        }
        // isLow ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _isLowProperty = new ReactiveProperty<bool>();

        [ShowInInspector][InspectorDisabled]
        public bool isLow
        {
            get { return _isLowProperty.Value; }
            private set { _isLowProperty.Value = value; }
        }

        private void UpdateisLow()
        {
            isLow = CalculateisLow();
        }

        protected abstract bool CalculateisLow();
        // isFull ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<bool> _isFullProperty = new ReactiveProperty<bool>();

        [ShowInInspector][InspectorDisabled]
        public bool isFull
        {
            get { return _isFullProperty.Value; }
            private set { _isFullProperty.Value = value; }
        }

        private void UpdateisFull()
        {
            isFull = CalculateisFull();
        }

        protected abstract bool CalculateisFull();

        // heartContainers ////////////////////////////////////////////////////////////
        [SerializeField][InspectorCollapsedFoldout]
        public DataCollection<HeartContainerType> heartContainers
        {
            get { return _heartContainers; }
            set
            {
                _heartContainers = value;

                if(heartContainers != null)
                {
                    heartContainers.BindingsCreator = heartContainersBindingsCreator;

                    _heartContainers.ObserveReplace().Subscribe(_ => UpdateHeartTotals());
                    _heartContainers.ObserveAdd().Subscribe(_ => UpdateHeartTotals());
                    _heartContainers.ObserveRemove().Subscribe(_ => UpdateHeartTotals());
                }
            }
        }
        private DataCollection<HeartContainerType> _heartContainers;

        private List<IDisposable> heartContainersBindingsCreator(HeartContainerType value)
        {
            if(value == default(HeartContainerType))
            {
                return new List<IDisposable>();
            }

            return new List<IDisposable>()
                    {
                    };
        }

        protected AvatarHealthBase()
        {
            heartContainers = new DataCollection<HeartContainerType>();

            _heartTotalsProperty.Subscribe(_ => UpdateisLow());

            _heartTotalsProperty.Subscribe(_ => UpdateisFull());



            UpdateisLow();

            UpdateisFull();

        }

        protected abstract void UpdateHeartTotals();
    }
    public abstract class AvatarResourceBase: Data
    {
        public IntRange range = new IntRange();
        // value ////////////////////////////////////////////////////////////
        [NonSerialized]
        public ReactiveProperty<int> _valueProperty = new ReactiveProperty<int>();

        [SerializeField]
        public int value
        {
            get { return _valueProperty.Value; }
            set { _valueProperty.Value = value; }
        }

    }
    public abstract class AvatarResourcesBase: Data
    {
        public AvatarResource bombs = new AvatarResource();
        public AvatarResource keys = new AvatarResource();
        public AvatarResource coins = new AvatarResource();

    }
}