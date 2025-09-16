namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    //DbSet kolekcija
    public DbSet<Kompanija> Kompanije { get; set; }
    public DbSet<Roba> Roba { get; set; }
    public DbSet<Vozilo> Vozila { get; set; }
    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}