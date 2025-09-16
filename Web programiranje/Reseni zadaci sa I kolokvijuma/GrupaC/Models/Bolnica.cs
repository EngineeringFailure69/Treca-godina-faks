namespace Models;

public class Bolnica
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public required string Lokacija { get; set; }
    public uint BrojOdeljenja { get; set; }
    public uint BrojOsoblja { get; set; }

    [RegularExpression(@"^(\+381|0)([1-9])([0-9]){6,9}$")]
    public required string BrojTelefona { get; set; }

    public List<Zaposlen>? Zaposleni { get; set; }
}
