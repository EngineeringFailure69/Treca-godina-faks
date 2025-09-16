namespace Models;

public class Numera
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public TimeSpan Trajanje { get; set; }
    public required string Zanr { get; set; }
    public uint BrojUmetnika { get; set; }

    public Album? Album { get; set; }
}
