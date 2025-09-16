namespace Models;

public class Nekretnina
{
    [Key]
    public int ID { get; set; }
    public required string Tip { get; set; }
    public uint Vrednost { get; set; }
    public required string Lokacija { get; set; }
    public uint BrojPrethodnihVlasnika { get; set; }

    [JsonIgnore]
    public List<Kupovina>? Kupovine { get; set; }
}
