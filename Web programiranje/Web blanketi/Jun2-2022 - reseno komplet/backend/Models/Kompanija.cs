namespace WebTemplate.Models;

public class Kompanija
{
    [Key]
    public int ID { get; set; }
    [MaxLength(50)]
    public required string Naziv { get; set; }
    public int ProsecnaZarada { get; set; }
    public int Cena { get; set; }
    public int BrojDanaZaIsporuku { get; set; }
    public List<Vozilo>? VozilaKompanije { get; set; }
}