namespace Graymatter.Platformer.Systems.EntitySystem.Views.Spawns
{
    public abstract class PlayerSpawnViewBase: View<PlayerSpawn>
    {
        public PlayerSpawn PlayerSpawn
        {
            get { return Data; }
            set { Data = value; }
        }

    }
}