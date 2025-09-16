namespace Models;

public class DistributivnoPodrucje
{
    [Key]
    public int ID { get; set; }
    public int KorisnickiBroj { get; set; }
    public DateTime DatumPotpisivanjaUgovora { get; set; }
    public required string BrojBrojila { get; set; }
    public Elektrodistribucija? Elektrodistribucija { get; set; }
    public Potrosac? Potrosac { get; set; }
}
