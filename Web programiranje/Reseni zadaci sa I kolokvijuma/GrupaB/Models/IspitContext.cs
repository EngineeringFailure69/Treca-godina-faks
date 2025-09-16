namespace Models;

public class IspitContext : DbContext
{
    public required DbSet<Elektrodistribucija> Elektrodistribucije { get; set; }
    public required DbSet<Potrosac> Potrosaci { get; set; }
    public required DbSet<DistributivnoPodrucje> Podrucja { get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
