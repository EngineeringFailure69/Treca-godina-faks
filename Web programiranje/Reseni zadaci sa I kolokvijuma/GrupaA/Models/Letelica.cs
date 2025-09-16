namespace Models;

public class Letelica
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public uint KapacitetPutnika { get; set; }
    public uint BrojOsobaPosade { get; set; }
    public required string BrojMotora { get; set; }

    [JsonIgnore]
    public List<Let>? Letovi { get; set; }
}
