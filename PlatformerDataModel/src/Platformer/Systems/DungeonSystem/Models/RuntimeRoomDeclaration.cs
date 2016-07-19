using Graymatter.Code.Generators;
using GrayMatterFramework;

namespace Graymatter.Platformer.Model
{
    public abstract class RuntimeRoomDeclaration: RoomDeclaration
    {
        public int index;

        public RoomState state;

        public RoomTypeKnowledge typeKnowledge;

        public bool isOccupied;

        public abstract bool IsClear { get; }

        public interface ICommands
        {
            void Discover();

            void DiscoverType();

            void DiscoveredEntrance();

            void Enter();

            void Exit();
        }
    }
}
