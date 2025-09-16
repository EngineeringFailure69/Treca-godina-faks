namespace WebTemplate.Models;

public class Ram
{
    [Key]
    public int ID { get; set; }
    public int BrRamova { get; set; }
    public required string Materijal { get; set; }
    public Dimenzija? DimenzijaRama { get; set; }
    public List<Fotografija>? FotografijeRamovi { get; set; }
}