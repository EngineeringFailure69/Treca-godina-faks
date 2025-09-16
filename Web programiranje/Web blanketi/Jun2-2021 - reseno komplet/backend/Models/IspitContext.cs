namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    //DbSet kolekcija
    public DbSet<Ispit> Ispiti { get; set; }
    public DbSet<Student> Studenti { get; set; }
    public DbSet<Rok> Rokovi { get; set; }
    public DbSet<Spoj> Spojevi { get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}