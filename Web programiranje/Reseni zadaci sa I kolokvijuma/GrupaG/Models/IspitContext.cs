namespace Models;

public class IspitContext : DbContext
{
    public required DbSet<Kupac> Kupci { get; set; }
    public required DbSet<Nekretnina> Nekretnine { get; set; }
    public required DbSet<Kupovina> Kupovine { get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
