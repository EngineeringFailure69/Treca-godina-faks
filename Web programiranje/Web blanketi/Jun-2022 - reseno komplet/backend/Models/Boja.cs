namespace WebtTemplate.Models;

public class Boja{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public Model? Model { get; set; }
    public List<Automobil>? BojaAutomobila { get; set; }
}