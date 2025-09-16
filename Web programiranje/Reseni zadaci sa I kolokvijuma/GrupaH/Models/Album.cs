namespace Models;

public class Album
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public DateTime GodinaIzdavanja { get; set; }
    public required string IzdavackaKuca { get; set; }

    public Autor? Autor { get; set; }
    public List<Numera>? Numere { get; set; }
}
