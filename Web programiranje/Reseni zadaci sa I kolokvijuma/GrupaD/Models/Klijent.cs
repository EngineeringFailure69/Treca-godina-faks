namespace Models;

public class Klijent
{
    [Key]
    public int ID { get; set; }
    public required string Ime { get; set; }
    public required string Prezime { get; set; }

    [Range(typeof(DateTime), "1900-01-01", "2004-01-01")]
    public DateTime DatumRodjenja { get; set; }

    [RegularExpression(@"^(\+381|0)([1-9])([0-9]){6,9}$")]
    public required string BrojTelefona { get; set; }

    public List<Racun>? Racuni { get; set; }
}
