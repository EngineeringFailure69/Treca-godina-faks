namespace Models;

public class Banka
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public required string Lokacija { get; set; }
    
    [RegularExpression(@"^(\+381|0)([1-9])([0-9]){6,9}$")]
    public required string BrojTelefona { get; set; }
    public uint BrojZaposlenih { get; set; }

    public List<Racun>? Racuni { get; set; }
}
