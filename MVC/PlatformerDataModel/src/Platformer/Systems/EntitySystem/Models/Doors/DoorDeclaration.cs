using System.Collections.Generic;
using Graymatter.Code.Attributes;
using Graymatter.Code.Declarations;
using Graymatter.Platformer.Systems.EntitySystem.Models.Doors;
using Graymatter.Platformer.Systems.EntitySystem.Models.Doors.Locks;

namespace Graymatter.Platformer.Systems.EntitySystem.Models.Doors
{
    public abstract class DoorEntityDeclaration: EntityContainerDeclaration
    {
        [Dependencies("children")]
        public List<DoorLockEntityDeclaration> doorLocks;

        [Dependencies("doorLocks.isLocked", "doorLocks")]
        public bool isClosed;
    }

    public abstract class NormalDoorEntityDeclaration: DoorEntityDeclaration
    {
    }

    public abstract class TreasureDoorEntityDeclaration: DoorEntityDeclaration
    {
    }

    public abstract class BossDoorEntityDeclaration: DoorEntityDeclaration
    {
    }

    public abstract class SpikeyDoorEntityDeclaration: DoorEntityDeclaration
    {
    }

    public abstract class WallEntityDeclaration: DoorEntityDeclaration
    {
    }

    ///////////////////////////////////////////////////////////////////////////////////
}

namespace Graymatter.Platformer.Systems.EntitySystem.Views.Doors
{
    public abstract class DoorEntityViewDeclaration: ViewDeclaration<DoorEntityDeclaration>
    {
        [Dependencies("isClosed")]
        public abstract void OnIsClosedChanged();
    }
}