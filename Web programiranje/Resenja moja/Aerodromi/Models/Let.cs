using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Let
{
    [Key]
    public int ID { get; set; }
    public uint BrojPutnikaNaLetu { get; set; }
    public DateTime VremePoletanja { get; set; }
    public DateTime VremeSletanja { get; set; }
    [InverseProperty("PoleteliLetovi")]
    public Aerodrom? AerodromPoletanja { get; set; }
    [InverseProperty("SleteliLetovi")]
    public Aerodrom? AerodromSletanja { get; set; }
    public Letelica? Letelica { get; set; }
}