using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Kuvar
{
   [Key]
    public int ID { get; set; }
    public required string Ime { get; set; }
    public required string Prezime { get; set; }
    public DateTime DatumRodjenja { get; set; }
    public required string StrucnaSprema { get; set; }
    public List<Restoran>? ZaposlenU { get; set; }
}