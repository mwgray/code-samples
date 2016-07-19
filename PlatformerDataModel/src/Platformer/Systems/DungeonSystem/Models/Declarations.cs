using System.Collections.Generic;
using Graymatter.Code.Attributes;
using Graymatter.Code.Declarations;
using Graymatter.Platformer.EXTERNAL;

namespace Graymatter.Platformer.Systems.DungeonSystem.Models
{
    public class LevelDeclaration: DataDeclaration
    {
        public List<RoomDeclaration> rooms;
    }

    public class RoomDeclaration: DataDeclaration
    {
        public Vector2i position;

        public RoomType type;

        public RoomShape shape;

        [Dependencies("position", "shape")]
        public int width;

        [Dependencies("position", "shape")]
        public int height;
    }
}