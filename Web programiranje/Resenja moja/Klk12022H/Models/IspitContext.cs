namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    // DbSet kolekcije!
    public required DbSet<Numera> Numere {get; set;}
    public required DbSet<Autor> Autori {get; set;}
    public required DbSet<Album> Albumi {get; set;}

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}
