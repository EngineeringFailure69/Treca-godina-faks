namespace WebTemplate.Models;

public class Fotografija
{
    [Key]
    public int ID { get; set; }
    public int BrFotografija { get; set; }
    public required string SlikaPath { get; set; }
    public required string Naziv { get; set; }
    public Prodavnica? Prodavnica { get; set; }
    public Papir? Papir { get; set; }
    public Dimenzija? Dimenzija { get; set; }
    public Ram? Ram { get; set; }
}