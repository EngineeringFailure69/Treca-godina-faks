namespace WebtTemplate.Models;

public class Automobil{
    [Key]
    public int ID { get; set; }
    public required string SlikaPath { get; set; }
    public int Kolicina { get; set; }
    public int Cena { get; set; }
    public DateTime DatumPoslednjeProdaje { get; set;}
    public Marka? Marka { get; set; }
    public Model? Model { get; set; }
    public Boja? Boja { get; set; }
}