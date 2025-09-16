namespace Models;

public class IspitContext : DbContext
{
    public required DbSet<Album> Albumi { get; set; }
    public required DbSet<Autor> Autori { get; set; }
    public required DbSet<Numera> Numere { get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
