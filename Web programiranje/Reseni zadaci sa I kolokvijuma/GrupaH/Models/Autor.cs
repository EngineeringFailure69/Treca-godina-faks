namespace Models;

public class Autor
{
    [Key]
    public int ID { get; set; }
    public required string Ime { get; set; }
    public required string Prezime { get; set; }
    public DateTime DatumRodjenja { get; set; }
    public DateTime? PrviAlbum { get; set; }

    public List<Album>? Albumi { get; set; }
}
