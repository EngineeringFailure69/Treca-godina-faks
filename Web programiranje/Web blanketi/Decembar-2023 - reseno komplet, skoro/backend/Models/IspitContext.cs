namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    //DbSet kolekcija
    public DbSet<Prodavnica> Prodavnice { get; set; }
    public DbSet<Proizvod> Proizvodi { get; set; }
    public DbSet<Pripada> Pripada { get; set; }
    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}