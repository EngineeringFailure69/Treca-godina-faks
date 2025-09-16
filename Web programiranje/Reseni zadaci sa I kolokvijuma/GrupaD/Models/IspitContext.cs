namespace Models;

public class IspitContext : DbContext
{
    public required DbSet<Banka> Banke { get; set; }
    public required DbSet<Klijent> Klijenti { get; set; }
    public required DbSet<Racun> Racuni { get; set; }

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
