using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Zaposlen
{
    [Key]
    public int ID { get; set; }
    public DateTime DatumZaposlenja { get; set; }
    public uint Plata { get; set; }
    public required string Pozicija { get; set; }
    public Kuvar? Kuvar {get; set;}
    public Restoran? Restoran {get; set;}
}