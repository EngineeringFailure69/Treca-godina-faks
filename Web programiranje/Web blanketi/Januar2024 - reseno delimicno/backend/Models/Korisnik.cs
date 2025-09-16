namespace WebTemplate.Models;

public class Korisnik
{
    [Key]
    public int ID { get; set; }
    [MaxLength(15)]
    public required string KorisnickoIme { get; set; }
    [MaxLength(30)]
    public required string Ime { get; set; }
    [MaxLength(30)]
    public required string Prezime { get; set; }
    public required string Sifra { get; set; }
    public required string Email { get; set; }
    public List<Pripada>? KorisnikUSobi { get; set; }
}