using System;
using System.Collections.Generic;
using Graymatter.Code.Attributes;
using Graymatter.Code.Declarations;
using Graymatter.Platformer.EXTERNAL;
using Graymatter.Platformer.Systems.DungeonSystem.Models;
using Graymatter.Platformer.Systems.EntitySystem.Models.Items;
using Graymatter.Platformer.Systems.EntitySystem.Models.Monsters;
using Graymatter.Platformer.Systems.EntitySystem.Models.Spawns;
using Graymatter.Platformer.Systems.InventorySystem.Models;
using Graymatter.Platformer.Systems.InventorySystem.Models.Restrictions;
using Graymatter.Platformer.Systems.InventorySystem.Views.Restrictions;

namespace Graymatter.Platformer.Systems.EntitySystem.Models
{
    public class EntityDeclaration: DataDeclaration
    {
        public Vector3i localPosition;
    }

    public abstract class EntityContainerDeclaration: EntityDeclaration
    {
        public List<EntityDeclaration> children;

        [Dependencies("children#Add")]
        public abstract void OnChildAdded(EntityDeclaration entity);

        [Dependencies("children#Remove")]
        public abstract void OnChildRemoved(EntityDeclaration entity);

        [Dependencies("children#Replace")]
        public abstract void OnChildReplaced(EntityDeclaration newValue, EntityDeclaration oldValue);
    }

    public abstract class TileLayerEntityDeclaration: EntityContainerDeclaration
    {
        public string name;

        public Vector3 cellSize;
        public Vector2i layerSize;
    }

    public abstract class RoomEntityDeclaration: EntityContainerDeclaration
    {
        public RoomDeclaration room;

        public RoomState state;

        public RoomTypeKnowledge typeKnowledge;

        [Dependencies("children")]
        public List<MonsterSpawnDeclaration> monsters;

        [Dependencies("state")]
        public bool isVisible;

        [NonSerialized]
        public bool isOccupied;

        [NonSerialized]
        [Dependencies("monsters.wasKilled")]
        public bool isClear;
    }

    public abstract class LevelEntityDeclaration: EntityContainerDeclaration
    {
        [Dependencies("children")]
        public List<RoomEntityDeclaration> rooms;

        [Dependencies("rooms")]
        public RectInt bounds;

        [Dependencies("bounds", "rooms.isVisible")]
        public RectInt visibleBounds;
        
        [NonSerialized]
        public RoomEntityDeclaration currentRoom;
    }

    public abstract class ReferenceEntityDeclaration: EntityDeclaration
    {
        public class EntityAsset
        {
        }

        [Reference]
        public EntityAsset reference;
    }
}

namespace Graymatter.Platformer.Systems.EntitySystem.Models.Flooring
{
    public class OneWayFloorDeclaration: EntityDeclaration { }
    public class SolidFloorDeclaration: EntityDeclaration { }
    
    public class LedgeWaypointDeclaration: EntityDeclaration { }
    public class WallWaypointDeclaration: EntityDeclaration { }
}

namespace Graymatter.Platformer.Systems.InventorySystem.Models
{
    public class InventoryDeclaration: DataDeclaration
    {
        [Reference]
        public ActivatedItem activatedItem;

        [Reference]
        public Item oneShotItem;

        public List<Item> items;
    }
}

namespace Graymatter.Platformer.Systems.InventorySystem.Models.Restrictions
{
public abstract class ItemRestrictionDeclaration: DataDeclaration
    {
        public abstract bool TryUse();
    }

    public abstract class ChargeItemRestrictionDeclaration : ItemRestrictionDeclaration
    {
        public int charges;

        public int chargesNeededPerUse = 6;
    }

    public abstract class PayAvatarResourceItemRestrictionDeclaration: ItemRestrictionDeclaration { }
    public abstract class PayAvatarHealthItemRestrictionDeclaration: ItemRestrictionDeclaration { }
}

namespace Graymatter.Platformer.Systems.InventorySystem.Views
{
    public abstract class InventoryViewDeclaration: ViewDeclaration<InventoryDeclaration>
    {
        [Dependencies("activatedItem")]
        public abstract void OnActivatedItemChanged();

        [Dependencies("oneShotItem")]
        public abstract void OnOneShotItemItemChanged();

        [Dependencies("items#Add")]
        public abstract void OnItemAdded(Item item);
        
        [Dependencies("items#Remove")]
        public abstract void OnItemRemoved(Item item);

        [Dependencies("items#Replace")]
        public abstract void OnItemReplaced(Item newValue, Item oldValue);
    }
}

namespace Graymatter.Platformer.Systems.InventorySystem.Views.Restrictions
{
    public abstract class ChargeItemRestrictionViewDeclaration: ViewDeclaration<ChargeItemRestrictionDeclaration>
    {
        [Dependencies("charges")]
        public abstract void OnChargeChanged();

        [Dependencies("chargesNeededPerUse")]
        public abstract void OnRechargeAmountChanged();
    }
}

namespace Graymatter.Platformer.Systems.EntitySystem.Models.Items
{
    public class RandomShopItemDeclaration: EntityDeclaration { }
    
    public enum ItemPoolType { } 
    public class Item {}
    public class ActivatedItem {}
    public enum ItemPedestalState { }

    public class ItemPedestalDeclaration: EntityDeclaration
    {
        [Observed]
        public ItemPoolType ItemPoolType;
        [Reference]
        public Item Item;
        public ItemPedestalState state;
    }
}   

namespace Graymatter.Platformer.Systems.EntitySystem.Models.Spawns
{
    public class PlayerSpawnDeclaration: EntityDeclaration { }
}

namespace Graymatter.Platformer.Systems.EntitySystem.Models.Monsters
{
    public class MonsterDeclaration: DataDeclaration
    {
        public int life;

        [Dependencies("life")]
        public bool isAlive;
    }

    public abstract class MonsterSpawnDeclaration: EntityDeclaration
    {
        public bool spawnFlipped;

        [NonSerialized]
        public bool active;
        
        public bool wasKilled;
    }
}

namespace Graymatter.Platformer.Systems.EntitySystem.Views.Items
{
    public abstract class ItemPedestalViewDeclaration: ViewDeclaration<ItemPedestalDeclaration>
    {
        [Dependencies("Item")]
        public abstract void OnItemChanged();
        
        [Dependencies("state")]
        public abstract void OnStateChanged();
    }
}

namespace Graymatter.Platformer.Systems.EntitySystem.Views.Monsters
{
    public abstract class MonsterViewDeclaration: ViewDeclaration<MonsterDeclaration>
    {
    }

    public abstract class MonsterSpawnViewDeclaration: ViewDeclaration<MonsterSpawnDeclaration>
    {
        [Dependencies("active")]
        protected abstract void OnActiveChanged();
    }
}

namespace Graymatter.Platformer.Systems.EntitySystem.Views.Spawns
{
    public class PlayerSpawnViewDeclaration: ViewDeclaration<PlayerSpawnDeclaration> { }
}
 
namespace Graymatter.Platformer.Systems.EntitySystem.Models.Doorways
{
    public abstract class DoorwayEntityDeclaration: EntityContainerDeclaration
    {
        public Vector2i doorIndex;
    }

    public abstract class   LeftDoorwayDeclaration: DoorwayEntityDeclaration { }
    public abstract class  RightDoorwayDeclaration: DoorwayEntityDeclaration { }
    public abstract class    TopDoorwayDeclaration: DoorwayEntityDeclaration { }
    public abstract class BottomDoorwayDeclaration: DoorwayEntityDeclaration { }
}