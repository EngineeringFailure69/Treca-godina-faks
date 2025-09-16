namespace WebTemplate.Models;

public class Roba
{
    [Key]
    public int ID { get; set; }
    public int Zapremina { get; set; }
    public int Tezina { get; set; }
    public int CenaOd { get; set; }
    public int CenaDo { get; set; }
     public DateTime DatumPrijema { get; set; }
     public DateTime DatumIsporuke { get; set; }
     public Vozilo? Vozilo { get; set; }
}