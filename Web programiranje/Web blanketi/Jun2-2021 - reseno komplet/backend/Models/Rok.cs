namespace WebTemplate.Models;

public class Rok
{
    [Key]
    public int ID { get; set; }
    [MaxLength(10)]
    public required string Naziv { get; set; }
    public List<Spoj>? StudentiPredmeti { get; set; }
}