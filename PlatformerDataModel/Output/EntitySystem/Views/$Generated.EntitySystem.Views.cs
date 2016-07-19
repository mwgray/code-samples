namespace Graymatter.Platformer.Systems.EntitySystem.Views
{
    public abstract class EntityContainerViewBase: View<EntityContainer>
    {
        public EntityContainer EntityContainer
        {
            get { return Data; }
            set { Data = value; }
        }

    }
    public abstract class EntityViewBase: View<Entity>
    {
        public Entity Entity
        {
            get { return Data; }
            set { Data = value; }
        }

    }
    public abstract class LevelEntityMinimapViewBase: View<LevelEntity>
    {
        public LevelEntity LevelEntity
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._currentRoomProperty.Subscribe(_=>OnCurrentRoomChanged());
            Data._boundsProperty.Subscribe(_=>OnBoundsChanged());
            Data._visibleBoundsProperty.Subscribe(_=>OnVisibleBoundsChanged());
            Data.rooms.ObserveAdd().Subscribe(_=> OnRoomAdded(_.Value));
            Data.rooms.ObserveRemove().Subscribe(_=> OnRoomRemoved(_.Value));
            Data.rooms.ObserveReplace().Subscribe(_=> OnRoomReplaced(_.NewValue, _.OldValue));

            Dirty();
        }

        protected abstract void OnCurrentRoomChanged();
        protected abstract void OnBoundsChanged();
        protected abstract void OnVisibleBoundsChanged();
        protected abstract void OnRoomAdded(RoomEntity room);
        protected abstract void OnRoomRemoved(RoomEntity room);
        protected abstract void OnRoomReplaced(RoomEntity newValue, RoomEntity oldValue);
    }
    public abstract class LevelEntityViewBase: EntityContainerView
    {
    }
    public abstract class ReferenceEntityViewBase: View<ReferenceEntity>
    {
        public ReferenceEntity ReferenceEntity
        {
            get { return Data; }
            set { Data = value; }
        }

    }
    public abstract class RoomEntityMinimapViewBase: View<RoomEntity>
    {
        public RoomEntity RoomEntity
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._stateProperty.Subscribe(_=>Dirty());
            Data._typeKnowledgeProperty.Subscribe(_=>Dirty());
            Data._isOccupiedProperty.Subscribe(_=>Dirty());
            Data._isClearProperty.Subscribe(_=>Dirty());

            Dirty();
        }

    }
    public abstract class RoomEntityViewBase: EntityContainerView
    {
    }
    public abstract class TileLayerEntityViewBase: View<TileLayerEntity>
    {
        public TileLayerEntity TileLayerEntity
        {
            get { return Data; }
            set { Data = value; }
        }

    }
}