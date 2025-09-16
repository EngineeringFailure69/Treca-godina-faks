using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Podrucje
{
    [Key]
    public int ID {get; set;}
    public uint KorisnickiBroj{get; set;}
    public DateTime DatumPotpisivanjaUgovora {get; set;}
    public required string BrojBrojila {get; set;}
    public Distribucija? Distribucija {get; set;}
    public Potrosac? Potrosac {get; set;}
}