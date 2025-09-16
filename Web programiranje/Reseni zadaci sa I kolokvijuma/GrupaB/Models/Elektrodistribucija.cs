namespace Models;

public class Elektrodistribucija
{
    [Key]
    public int ID { get; set; }
    public required string Naziv { get; set; }
    public required string Grad { get; set; }

    [RegularExpression(@"^([\w-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([\w-]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$")]
    public required string Email { get; set; }
    public uint BrojRadnika { get; set; }
    public List<DistributivnoPodrucje>? Podrucja { get; set; }
}
