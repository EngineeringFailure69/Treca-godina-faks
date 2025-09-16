using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Numera
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public required string Trajanje { get; set; }
    public required string Zanr { get; set; }
    public uint BrojUmetnika { get; set; }
    public Album? Album {get; set;}
}