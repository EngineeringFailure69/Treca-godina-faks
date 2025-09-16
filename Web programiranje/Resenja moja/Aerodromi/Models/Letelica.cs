namespace WebTemplate.Models;

public class Letelica
{
    [Key]
    public int ID { get; set; }
    [MaxLength(50)]
    public required string Naziv { get; set; }
    public uint KapacitetPutnika { get; set; }
    public uint Posada { get; set; }
    public uint BrojMotora { get; set; } //Broj motora koje letelica poseduje
    public List<Let>? Letovi { get; set; }
}