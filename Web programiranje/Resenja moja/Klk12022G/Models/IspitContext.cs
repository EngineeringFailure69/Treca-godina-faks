namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    // DbSet kolekcije!
    public required DbSet<Nekretnina> Nekretnine {get; set;}
    public required DbSet<Vlasnik> Vlasnici {get; set;}
    public required DbSet<Kupovina> Kupovine {get; set;}

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
