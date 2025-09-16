using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Aerodrom
{
    [Key]
    public int ID { get; set; }
    [MaxLength(50)]
    public required string Naziv { get; set; }
    [Length(3, 3)] //Minimum i maksimum 3 karaktera, odnosno kod mora da bude duzine 3 
    //[MinLength(3)]
    //[MaxLength(3)]
    public required string Kod { get; set; }
    [Range(-90, 90)]
    public double Latitude { get; set; }
     [Range(-180, 180)]
    public double Longitude { get; set; }
    public uint KapacitetLetelica { get; set; }
    public uint KapacitetPutnika { get; set; }
    //[ForeignKey("PoleteliLetoviFK")]
    public List<Let>? PoleteliLetovi { get; set; }
    //[ForeignKey("SleteliLetoviFK")]
    public List<Let>? SleteliLetovi { get; set; }
}