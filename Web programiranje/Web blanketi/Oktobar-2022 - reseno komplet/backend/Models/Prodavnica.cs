namespace WebTemplate.Models;

public class Prodavnica
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public int DnevnaZarada { get; set; }
    public List<Fotografija>? FotografijeUProdavnici { get; set; }
}