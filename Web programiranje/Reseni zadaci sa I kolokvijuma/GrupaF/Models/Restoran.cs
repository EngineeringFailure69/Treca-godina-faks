namespace Models;

public class Restoran
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public uint MaxBrojGostiju { get; set; }
    public uint MaxBrojKuvara { get; set; }
    
    [RegularExpression(@"^(\+381|0)([1-9])([0-9]){6,9}$")]
    public required string BrojTelefona { get; set; }

    public List<Zaposlen>? Zaposleni { get; set; }
}
