namespace Models;

public class Aerodrom
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }

    [MinLength(3)]
    [MaxLength(3)]
    public required string Kod { get; set; }

    [Range(-90, 90)]
    public double Latitude { get; set; }

    [Range(-180, 180)]
    public double Longitude { get; set; }
    public uint KapacitetLetelica { get; set; }
    public uint KapacitetPutnika { get; set; }

    [JsonIgnore]
    public List<Let>? LetoviPoleteli { get; set; }
    [JsonIgnore]
    public List<Let>? LetoviSleteli { get; set; }
}
