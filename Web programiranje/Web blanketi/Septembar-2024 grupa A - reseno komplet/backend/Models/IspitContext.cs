namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    // DbSet kolekcije!
    public DbSet<Stan> Stanovi { get; set; }
    public DbSet<Racun> Racuni {get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
