namespace WebTemplate.Models;

public class Dimenzija
{
    
    [Key]
    public int ID { get; set; }
    public int Visina { get; set; }
    public int Sirina { get; set; }
    public List<Ram>? DimenzijeRamova { get; set; }
    public List<Fotografija>? DimenzijeFotografija { get; set; }
}