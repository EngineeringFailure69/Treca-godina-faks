namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    // DbSet kolekcije!
    public required DbSet<Grad> Gradovi {get; set;}
    public required DbSet<Relacija> Relacije {get; set;}
    public required DbSet<Voz> Vozovi {get; set;}

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
