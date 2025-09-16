using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Potrosac
{
    [Key]
    public int ID {get; set;}
    [MaxLength(50)]
    public required string Ime {get; set;}
    [MaxLength(50)]
    public required string Prezime {get; set;}
    public uint GodinaRodjenja {get; set;}
    public required string MestoRodjenja {get; set;}
    public List<Podrucje>? Podrucja {get; set;}
}