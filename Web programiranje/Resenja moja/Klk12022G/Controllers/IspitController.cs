namespace WebTemplate.Controllers;

[ApiController]
[Route("[controller]")]
public class IspitController : ControllerBase
{
    public IspitContext Context { get; set; }

    public IspitController(IspitContext context)
    {
        Context = context;
    }

    [HttpPost("DodajNekretninu")]
    public async Task<ActionResult> DodajNekretninu([FromBody]Nekretnina nekretnina)
    {
        try
        {
            await Context.Nekretnine.AddAsync(nekretnina);
            await Context.SaveChangesAsync();
            return Ok($"Nekretnina sa ID: {nekretnina.ID} je dodata.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajVlasnika")]
    public async Task<ActionResult> DodajVlasnika([FromBody]Vlasnik vlasnik)
    {
        try
        {
            await Context.Vlasnici.AddAsync(vlasnik);
            await Context.SaveChangesAsync();
            return Ok($"Vlasnik sa ID: {vlasnik.ID} je dodat.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
   
   [HttpPost("DodajKupovinu/{vlasnikID}/{nekretninaID}/{datumKupovine}/{brUgovora}/{isplacenaVrednost}")]
   public async Task<ActionResult> DodajKupovinu(int vlasnikID, int nekretninaID, DateTime datumKupovine, int brUgovora, uint isplacenaVrednost)
   {
        try
        {
            var vlasnik = await Context.Vlasnici.FindAsync(vlasnikID);
            var nekretnina = await Context.Nekretnine.FindAsync(nekretninaID);

            if(vlasnik != null && nekretnina != null)
            {
                var kupovina = new Kupovina
                {
                    BrojUgovora = brUgovora,
                    DatumKupovine = datumKupovine,
                    IsplacenaVrednost = isplacenaVrednost,
                    Vlasnik = vlasnik,
                    Nekretnina = nekretnina
                };

                await Context.Kupovine.AddAsync(kupovina);
                await Context.SaveChangesAsync();
                return Ok($"Kupovina sa ID: {kupovina.ID} je dodata.");
            }
            else
            {
                return BadRequest("Kupac ili nekretnina nisu pronadjeni!");
            }
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
   }

   [HttpGet("Pretraga/{vlasnikID}")]
   public async Task<ActionResult> Pretraga(int vlasnikID)
    {
        try
        {
            var nekretnine = await Context.Nekretnine
                .Include(p => p.Kupovine!)
                .ThenInclude(p => p.Vlasnik)
                .Where(p => p.Kupovine!
                    .OrderByDescending(p => p.DatumKupovine)
                    .FirstOrDefault()!
                    .Vlasnik!.ID == vlasnikID)
                .ToListAsync();
                
            
            if (nekretnine == null)
            {
                return BadRequest("Nekretnine nisu pronadjene!");
            }

            return Ok(nekretnine);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("ProsecnaCenaNekretnine/{nekretninaID}")]
    public async Task<ActionResult> ProsecnaCenaNekretnine(int nekretninaID)
    {
        try
        {
            var prosek = await Context.Kupovine.Include(p => p.Nekretnina)
                .Where(p => p.Nekretnina!.ID == nekretninaID)
                .AverageAsync(p => p.IsplacenaVrednost);
            return Ok(prosek);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}
