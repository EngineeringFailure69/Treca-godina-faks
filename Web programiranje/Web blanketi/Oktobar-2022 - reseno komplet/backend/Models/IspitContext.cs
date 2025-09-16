namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    //DbSet kolekcija
    public DbSet<Fotografija> Fotografije { get; set; }
    public DbSet<Dimenzija> Dimenzije { get; set; }
    public DbSet<Papir> Papiri { get; set; }
    public DbSet<Ram> Ramovi { get; set; }
    public DbSet<Prodavnica> Prodavnice { get; set; }
    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}