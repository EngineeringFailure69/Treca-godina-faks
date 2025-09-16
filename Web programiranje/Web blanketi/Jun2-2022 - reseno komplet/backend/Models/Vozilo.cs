namespace WebTemplate.Models;

public class Vozilo
{
    [Key]
    public int ID { get; set; }
    [MaxLength(50)]
    public required string Naziv { get; set; }
    public required string SlikaPath { get; set; }
    public int Zapremina { get; set; }
    public int Tezina { get; set; }
    public int Cena { get; set; }
     public DateTime DatumPrijema { get; set; }
     public DateTime DatumIsporuke { get; set; }
     public Kompanija? KompanijinoVozilo { get; set; }
     public List<Roba>? RobaUVozilu { get; set;}
}