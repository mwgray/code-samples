using System;
using Graymatter.Code.Attributes;
using Graymatter.Code.Declarations;
using Graymatter.Platformer.EXTERNAL;
using Graymatter.Platformer.Systems.AvatarSystem.Models;
using Graymatter.Platformer.Systems.EntitySystem.Models.Doors.Locks;

namespace Graymatter.Platformer.Systems.EntitySystem.Models.Doors.Locks
{
    public class DoorLockEntityDeclaration: EntityDeclaration
    {
        public bool isLocked;
    }

    public class ClearRoomDoorLockEntityDeclaration: DoorLockEntityDeclaration
    {
        public bool canBombOpen;
    }

    public abstract class PayAvatarResourceDoorLockEntityDeclaration: DoorLockEntityDeclaration
    {
        public AvatarResourceType avatarResource;

        public int needed;
        public int paid;

        [Dependencies("needed", "paid")]
        protected abstract void UpdateIsLocked();
    }

    public class DestroyableDoorLockEntityDeclaration: DoorLockEntityDeclaration
    {
    }

    public abstract class HealthDoorLockEntityDeclaration: DoorLockEntityDeclaration
    {
        [NonSerialized]
        [Reference]
        public AvatarHealthDeclaration health;

        public AvatarHealthState state;

        [Dependencies("health_isLow", "health_isFull", "state")]
        protected abstract void UpdateIsLocked();
    }
}

namespace Graymatter.Platformer.Systems.EntitySystem.Views.Doors.Locks
{
    public abstract class PayAvatarResourceDoorLockEntityViewDeclaration: ViewDeclaration<PayAvatarResourceDoorLockEntityDeclaration>
    {
        [Dependencies("avatarResource")]
        protected abstract void OnAvatarResourceChanged();

        [Dependencies("needed")]
        protected abstract void OnNeededChanged();

        [Dependencies("paid")]
        protected abstract void OnPaidChanged();
    }

    public abstract class HealthDoorLockEntityViewDeclaration: ViewDeclaration<HealthDoorLockEntityDeclaration>
    {
        [Dependencies("state")]
        protected abstract void Dirty();
    }

    public abstract class ClearRoomDoorLockEntityViewDeclaration: ViewDeclaration<ClearRoomDoorLockEntityDeclaration>
    {
    }

    public abstract class DestroyableDoorLockEntityViewDeclaration: ViewDeclaration<DestroyableDoorLockEntityDeclaration>
    {
        [Dependencies("isLocked")]
        protected abstract void OnLockedChanged();
    }
}