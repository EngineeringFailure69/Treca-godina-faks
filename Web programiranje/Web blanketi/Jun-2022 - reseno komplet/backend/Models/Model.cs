
namespace WebtTemplate.Models;

public class Model{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public Marka? Marka { get; set; }
    public List<Boja>? BojeModela { get; set; }
    public List<Automobil>? ModelAutomobila { get; set; }
}