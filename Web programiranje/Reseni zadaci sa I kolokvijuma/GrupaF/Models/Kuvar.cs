namespace Models;

public class Kuvar
{
    [Key]
    public int ID { get; set; }
    public required string Ime { get; set; }
    public required string Prezime { get; set; }
    public DateTime DatumRodjenja { get; set; }
    public required string StrucnaSprema { get; set; }

    [JsonIgnore]
    public List<Zaposlen>? ZaposlenU { get; set; }
}
