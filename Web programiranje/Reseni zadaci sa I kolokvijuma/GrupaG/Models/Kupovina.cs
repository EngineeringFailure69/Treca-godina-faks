namespace Models;

public class Kupovina
{
    [Key]
    public int ID { get; set; }
    public DateTime DatumKupovine { get; set; }
    public int BrojUgovora { get; set; }
    public uint IsplacenaVrednost { get; set; }

    public Kupac? Kupac { get; set; }
    public Nekretnina? Nekretnina { get; set; }
}
