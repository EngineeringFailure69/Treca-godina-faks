namespace Models;

public class IspitContext : DbContext
{
    public required DbSet<Aerodrom> Aerodromi { get; set; }
    public required DbSet<Letelica> Letelice { get; set; }
    public required DbSet<Let> Letovi { get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        base.OnModelCreating(modelBuilder);

        modelBuilder.Entity<Let>()
            .HasOne(p => p.AerodromPoletanja)
            .WithMany(p => p.LetoviPoleteli);

        modelBuilder.Entity<Let>()
            .HasOne(p => p.AerodromSletanja)
            .WithMany(p => p.LetoviSleteli);
    }
}
