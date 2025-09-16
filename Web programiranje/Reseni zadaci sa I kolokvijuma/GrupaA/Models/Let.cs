namespace Models;

public class Let
{
    [Key]
    public int ID { get; set; }
    public DateTime VremePoletanja { get; set; }
    public DateTime VremeSletanja { get; set; }
    public Aerodrom? AerodromPoletanja { get; set; }
    public Aerodrom? AerodromSletanja { get; set; }
    public Letelica? Letelica { get; set; }    
}
