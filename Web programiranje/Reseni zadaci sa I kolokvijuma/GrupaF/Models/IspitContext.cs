namespace Models;

public class IspitContext : DbContext
{
    public required DbSet<Kuvar> Kuvari { get; set; }
    public required DbSet<Restoran> Restorani { get; set; }
    public required DbSet<Zaposlen> Zaposleni { get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
