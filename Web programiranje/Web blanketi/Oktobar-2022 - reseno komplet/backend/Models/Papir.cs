namespace WebTemplate.Models;

public class Papir
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public List<Fotografija>? PapiriFotografija { get; set; }
}