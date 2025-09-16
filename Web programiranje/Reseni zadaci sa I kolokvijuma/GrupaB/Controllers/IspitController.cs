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

    [HttpPost("DodavanjeElektrodistribucije")]
    public async Task<ActionResult> DodavanjeElektrodistribucije([FromBody] Elektrodistribucija el)
    {
        try
        {
            await Context.Elektrodistribucije.AddAsync(el);
            await Context.SaveChangesAsync();
            return Ok($"Elektrodistribucija sa ID: {el.ID} je dodata.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjePotrosaca")]
    public async Task<ActionResult> DodavanjePotrosaca([FromBody] Potrosac pot)
    {
        try
        {
            await Context.Potrosaci.AddAsync(pot);
            await Context.SaveChangesAsync();
            return Ok($"Potrosac sa ID: {pot.ID} je dodat.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjePotrosacaDistribuciji/{potrosacID}/{distribucijaID}/{brojBrojila}/{korisnickiBroj}/{datumPotpisivanja}")]
    public async Task<ActionResult> DodavanjePotrosacaDistribuciji(int potrosacID, int distribucijaID, string brojBrojila, int korisnickiBroj, DateTime datumPotpisivanja)
    {
        try
        {
            var potrosac = await Context.Potrosaci.FindAsync(potrosacID);
            var distribucija = await Context.Elektrodistribucije.FindAsync(distribucijaID);

            if (potrosac != null && distribucija != null)
            {
                var el = new DistributivnoPodrucje
                {
                    BrojBrojila = brojBrojila,
                    // Moglo je da bude i DateTime.Now
                    DatumPotpisivanjaUgovora = datumPotpisivanja,
                    Elektrodistribucija = distribucija,
                    KorisnickiBroj = korisnickiBroj,
                    Potrosac = potrosac
                };

                await Context.Podrucja.AddAsync(el);
                await Context.SaveChangesAsync();
                return Ok($"Dodat je potrosac sa ID: {potrosac.ID} na distributivno podrucje sa ID: {distribucija.ID}");
            }
            else
            {
                return BadRequest("Potrosac ili elektrodistribucija nisu pronadjeni!");
            }
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("PronadjiPotrosaca/{brojBrojila}")]
    public async Task<ActionResult> PronadjiPotrosaca(string brojBrojila)
    {
        try
        {
            var potrosaci = await Context.Podrucja
                .Include(p => p.Potrosac)
                .Where(p => p.BrojBrojila == brojBrojila)
                .Select(p => p.Potrosac).ToListAsync();
            return Ok(potrosaci);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("PronadjiPotrosace/{podrucjeID}/{datumOd}/{datumDo}")]
    public async Task<ActionResult> PronadjiPotrosace(int podrucjeID, DateTime datumOd, DateTime datumDo)
    {
        try
        {
            var potrosaci = await Context.Podrucja
                .Include(p => p.Elektrodistribucija)
                .Include(p => p.Potrosac)
                .Where(p => p.DatumPotpisivanjaUgovora > datumOd && p.DatumPotpisivanjaUgovora < datumDo)
                .Select(p => p.Potrosac)
                .ToListAsync();
            return Ok(potrosaci);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}
