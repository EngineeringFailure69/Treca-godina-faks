namespace WebTemplate.Models;
public class Prodavnica
{
    [Key]
    public int ID { get; set; }
    [MaxLength(50)]
    public required string Naziv { get; set; }
    public required string Lokacija { get; set;}
    public required string BrTelefona { get; set; }
    public List<Pripada>? ProizvodUProdavnici { get; set; }
}