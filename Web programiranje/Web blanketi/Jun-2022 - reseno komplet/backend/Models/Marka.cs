namespace WebtTemplate.Models;

public class Marka{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public List<Model>? ModeliMarke { get; set; }
    public List<Automobil>? MarkaAutomobila { get; set; }
}