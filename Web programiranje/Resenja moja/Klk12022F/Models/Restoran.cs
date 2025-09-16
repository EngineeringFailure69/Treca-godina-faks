using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Restoran
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public uint MaxBrojGostiju { get; set; }
    public uint MaxBrojKuvara { get; set; }
    public required string BrojTelefona { get; set; }
    public List<Zaposlen>? Zaposleni {get; set;}

}