using System.Collections.Generic;
using Graymatter.Code.Attributes;
using Graymatter.Code.Declarations;
using Graymatter.Common.Mathematics;
using Graymatter.Platformer.EXTERNAL;
using Graymatter.Platformer.Systems.InventorySystem.Models;

namespace Graymatter.Platformer.Systems.AvatarSystem.Models
{
    public class AvatarResourcesDeclaration: DataDeclaration
    {
        public AvatarResourceDeclaration bombs;

        public AvatarResourceDeclaration keys;

        public AvatarResourceDeclaration coins;
    }

    public class AvatarResourceDeclaration: DataDeclaration
    {
        public IntRange range;

        public int value;
    }

    public abstract class AvatarHealthDeclaration: DataDeclaration
    {
        public List<HeartContainerType> heartContainers;

        public HeartTotals heartTotals;

        [Dependencies("heartTotals")]
        public bool isLow;

        [Dependencies("heartTotals")]
        public bool isFull;

        [Dependencies("heartContainers")]
        protected abstract void UpdateHeartTotals();
    }

    public class AvatarDeclaration: DataDeclaration
    {
        public AvatarHealthDeclaration health;

        public AvatarResourcesDeclaration resources;

        public InventoryDeclaration inventory;
    }
}