namespace Graymatter.Platformer.Systems.EntitySystem.Views.Monsters
{
    public abstract class MonsterSpawnViewBase: View<MonsterSpawn>
    {
        public MonsterSpawn MonsterSpawn
        {
            get { return Data; }
            set { Data = value; }
        }

        protected override void Bind()
        {
            base.Bind();

            Data._activeProperty.Subscribe(_=>OnActiveChanged());

            Dirty();
        }

        protected abstract void OnActiveChanged();
    }
    public abstract class MonsterViewBase: View<Monster>
    {
        public Monster Monster
        {
            get { return Data; }
            set { Data = value; }
        }

    }
}