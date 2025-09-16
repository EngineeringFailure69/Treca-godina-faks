namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    //DbSet kolekcija
    public DbSet<Korisnik> Korisnici { get; set; }
    public DbSet<Pripada> Pripada { get; set; }
    public DbSet<Soba> Sobe { get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}