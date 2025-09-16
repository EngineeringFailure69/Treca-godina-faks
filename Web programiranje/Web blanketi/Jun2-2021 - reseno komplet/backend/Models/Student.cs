namespace WebTemplate.Models;

public class Student
{
    [Key]
    public int ID { get; set; }
    [Range(10000, 20000)]
    public required int Indeks { get; set; }
    [MaxLength(50)]
    public required string Ime { get; set; }
    [MaxLength(50)]
    public required string Prezime { get; set; }
    public List<Spoj>? StudentPredmet { get; set; }
}