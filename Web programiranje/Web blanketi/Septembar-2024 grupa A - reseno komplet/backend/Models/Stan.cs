namespace WebTemplate.Models;

public class Stan{
    [Key]
    public int ID { get; set; }
    [MaxLength(30)]
    public required string ImeVlasnika { get; set; }
    public int Povrsina { get; set; }
    public int BrClanova { get; set; }
    public List<Racun>? RacuniZaStan { get; set; }
}