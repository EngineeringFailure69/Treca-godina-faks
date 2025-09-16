namespace WebTemplate.Models;

public class Proizvod
{
    [Key]
    public int ID { get; set; }
    [MaxLength(50)]
    public required string Naziv { get; set; }
    public required string Kategorija { get; set; }
    public double Cena { get; set; }
    public List<Pripada>? ProizvodU { get; set; }
}