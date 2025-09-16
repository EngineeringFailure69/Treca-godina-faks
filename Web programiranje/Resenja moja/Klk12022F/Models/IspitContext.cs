namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    // DbSet kolekcije!
    public required DbSet<Restoran> Restorani {get; set;}
    public required DbSet<Kuvar> Kuvari {get; set;}
    public required DbSet<Zaposlen> Zaposleni {get; set;}

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
