namespace WebTemplate.Models;

public class Pripada
{
    [Key]
    public int ID { get; set; }
    public uint Kolicina { get; set; }
    public Prodavnica? Prodavnica { get; set; }
    public Proizvod? Proizvod { get; set;}
}