namespace WebTemplate.Models;

public class Spoj
{
    [Key]
    public int ID { get; set; }
    [Range(5, 10)]
    public int Ocena { get; set; }
    public Rok? Rok { get; set; }
    public Ispit? Ispit { get; set; }
    public Student? Student { get; set; }
}