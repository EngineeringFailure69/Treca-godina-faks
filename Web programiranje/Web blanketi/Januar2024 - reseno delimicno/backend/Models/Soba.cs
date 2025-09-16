namespace WebTemplate.Models;

public class Soba
{
    [Key]
    public int ID { get; set; }
    [MaxLength(30)]
    public required string ImeSobe { get; set; }
    public uint MaksBrojClanova { get; set; }
    public List<Pripada>? SobaU { get; set; }
}