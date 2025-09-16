namespace Models;

public class Grad
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    
    [Range(-90, 90)]
    public double Latitude { get; set; }

    [Range(-180, 180)]
    public double Longitude { get; set; }
    public uint BrojKoloseka { get; set; }
    public uint BrojStanovnika { get; set; }

    public List<Relacija>? RelacijePolaska { get; set; }
    public List<Relacija>? RelacijeDolaska { get; set; }
}
