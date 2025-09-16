using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;
public class Lekar
{
    [Key]
    public int ID {get; set;}
    public required string Ime {get; set;}
    public required string Prezime {get; set;}
    public DateTime DatumRodjenja {get; set;}
    public DateTime DatumDiplomiranja{get; set;}
    public DateTime? DatumDobijanjaLicence {get; set;}
    public List<Bolnica>? Bolnice {get; set;}
}