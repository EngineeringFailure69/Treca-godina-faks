using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;
public class Zaposlen
{
    [Key]
    public int ID {get; set;}
    public int IdentifikacioniBroj {get; set;}
    public DateTime DatumPotpisivanjaUgovora{get; set;}
    public required string Specijalnost {get; set;}
    public Lekar? Lekar {get; set;}
    public Bolnica? Bolnica {get; set;}
}