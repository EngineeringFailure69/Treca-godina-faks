namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    // DbSet kolekcije!
    public DbSet<Distribucija> Distribucije { get; set; }
    public DbSet<Potrosac> Potrosaci { get; set;}
    public DbSet<Podrucje> Podrucja { get; set; }
    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
