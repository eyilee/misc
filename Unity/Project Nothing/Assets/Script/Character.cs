namespace ProjectNothing
{
    public class Character
    {
        // HP
        public int HP { get; set; }
        // MP
        public int MP { get; set; }

        // 力量 strength
        public int STR { get; set; }
        // 智力 intelligence
        public int INT { get; set; }
        // 敏捷 agile
        public int AGI { get; set; }
        // 幸運 lucky
        public int LUK { get; set; }

        // 命中 hit rate
        public float HR { get; set; }
        // 迴避 avoid rate
        public float AR { get; set; }
        // 精神 spirit
        public int SPI { get; set; }
        // 抵抗 resistance
        public int RES { get; set; }
        // 速度 speed
        public int SPD { get; set; }

        // 物理攻擊 attack
        public int ATK { get; set; }
        // 物理防禦 defense
        public int DEF { get; set; }
        // 魔法攻擊 magic attack
        public int MATK { get; set; }
        // 魔法防禦 magic defense
        public int MDEF { get; set; }
        // 物理穿透 armor penetration
        public int ARP { get; set; }
        // 魔法穿透 magic armor penetration
        public int MARP { get; set; }
        // 爆擊率 critical rate
        public int CR { get; set; }
        // 爆擊傷害 critical damage
        public int CRD { get; set; }
    }
}
