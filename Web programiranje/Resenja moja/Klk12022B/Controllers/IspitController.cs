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
    
    [HttpPost("DodavanjeElektroDistribucije")]
    public async Task<ActionResult> DodavanjeElektroDistribucije([FromBody] Distribucija el)
    {
        try
        {
            await Context.Distribucije.AddAsync(el);
            await Context.SaveChangesAsync();
            return Ok($"Elektrodistribucija sa ID: {el.ID} je dodata.");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjePotrosaca")]
    public async Task<ActionResult> DodavanjePotrosaca([FromBody] Potrosac el)
    {
        try
        {
            await Context.Potrosaci.AddAsync(el);
            await Context.SaveChangesAsync();
            return Ok($"Potrosac sa ID: {el.ID} je dodat");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjePotrosacaDistribuciji/{potrosacID}/{distribucijaID}/{brojBrojila}/{korisnickiBroj}/{datumPotpisivanja}")]
    public async Task<ActionResult> DodavanjePotrosacaDistribuciji(int potrosacID, int distribucijaID, string brojBrojila, 
        uint korisnickiBroj, DateTime datumPotpisivanja)
    {
        try
        {
            var potrosac = await Context.Potrosaci.FindAsync(potrosacID);
            var distribucija = await Context.Distribucije.FindAsync(distribucijaID);

            if(potrosac != null && distribucija != null)
            {
                var el = new Podrucje
                {
                    BrojBrojila = brojBrojila,
                    DatumPotpisivanjaUgovora=datumPotpisivanja=DateTime.Now,
                    Distribucija=distribucija,
                    Potrosac=potrosac,
                    KorisnickiBroj=korisnickiBroj,
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
                .Include(p => p.Distribucija)
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
