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

    [HttpPost("DodajKupca")]
    public async Task<ActionResult> DodajKupca([FromBody]Kupac kupac)
    {
        try
        {
            await Context.Kupci.AddAsync(kupac);
            await Context.SaveChangesAsync();
            return Ok($"Kupac sa ID: {kupac.ID} je dodat.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("Kupovina/{kupacID}/{nekretninaID}")]
    public async Task<ActionResult> Kupovina(int kupacID, int nekretninaID, [FromBody]Kupovina podaci)
    {
        try
        {
            var kupac = await Context.Kupci.FindAsync(kupacID);
            var nekretnina = await Context.Nekretnine
                .Include(p => p.Kupovine!)
                .ThenInclude(p => p.Kupac)
                .FirstOrDefaultAsync(p => p.ID == nekretninaID);

            if (nekretnina != null) 
            {
                var poslednja = nekretnina.Kupovine!.OrderByDescending(p => p.DatumKupovine).FirstOrDefault();
                
                if (poslednja != null && poslednja.Kupac!.ID == kupacID)
                {
                    return BadRequest("Isti kupac ne moze da kupi nekretninu 2 puta.");
                }
            }

            if (nekretnina != null && kupac != null)
            {
                var kupovina = new Kupovina
                {
                    BrojUgovora = podaci.BrojUgovora,
                    DatumKupovine = podaci.DatumKupovine,
                    IsplacenaVrednost = podaci.IsplacenaVrednost,
                    Kupac = kupac,
                    Nekretnina = nekretnina
                };

                await Context.Kupovine.AddAsync(kupovina);
                nekretnina.BrojPrethodnihVlasnika++;
                await Context.SaveChangesAsync();
                return Ok($"Kupac sa ID: {kupac.ID} je kupio nekretninu sa ID: {nekretnina.ID}");
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
                .ThenInclude(p => p.Kupac)
                .Where(p => p.Kupovine!
                    .OrderByDescending(p => p.DatumKupovine)
                    .FirstOrDefault()!
                    .Kupac!.ID == vlasnikID)
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
            var prosek = await Context.Kupovine
                .Include(p => p.Nekretnina)
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
