using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Klijent
{
    [Key]
    public int ID {get; set;}
    public required string Ime {get; set;}
    public required string Prezime {get; set;}
    public required string BrojTelefona {get; set;}
    public DateTime DatumRodjenja {get; set;}
    public List<Racun>? Racuni {get; set;}
}