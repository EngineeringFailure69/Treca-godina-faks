namespace Models;

public class IspitContext : DbContext
{
    public required DbSet<Bolnica> Bolnice { get; set; }
    public required DbSet<Lekar> Lekari { get; set; }
    public required DbSet<Zaposlen> Zaposljenja { get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
