namespace Models;

public class Kupac
{
    [Key]
    public int ID { get; set; }
    public required string Ime { get; set; }
    public required string Prezime { get; set; }
    public DateTime DatumRodjenja { get; set; }
    public required string MestoRodjenja { get; set; }

    public List<Kupovina>? Kupovine { get; set; }
}
