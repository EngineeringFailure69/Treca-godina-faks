namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    //DbSet kolekcija
    public DbSet<Aerodrom> Aerodromi { get; set; }
    public DbSet<Let> Letovi { get; set; }
    public DbSet<Letelica> Letelice { get; set; }
    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}