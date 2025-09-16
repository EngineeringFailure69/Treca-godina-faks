namespace WebTemplate.Models;

public class IspitContext : DbContext
{
    //DbSet kolekcija

    public IspitContext(DbContextOptions options) : base(options)
    {
        
    }
}