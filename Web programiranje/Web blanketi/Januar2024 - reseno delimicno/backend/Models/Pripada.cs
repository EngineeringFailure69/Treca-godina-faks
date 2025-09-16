namespace WebTemplate.Models;

public class Pripada
{
    [Key]
    public int ID { get; set; }
    public Korisnik? Korisnik { get; set;}
    public Soba? Soba{ get; set; }
}