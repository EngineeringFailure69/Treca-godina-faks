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

    [HttpPost("DodavanjeBolnice")]
    public async Task<ActionResult> DodavanjeBolnice([FromBody] Bolnica el)
    {
        try
        {
            await Context.Bolnice.AddAsync(el);
            await Context.SaveChangesAsync();
            return Ok("Bolnica je dodata");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjeLekara")]
    public async Task<ActionResult> DodavanjeLekara([FromBody] Lekar el)
    {
        try
        {
            await Context.Lekari.AddAsync(el);
            await Context.SaveChangesAsync();
            return Ok("Lekar je dodat");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjeZaposlenog/{lekarID}/{bolnicaID}")]
    public async Task<ActionResult> DodavanjeZaposlenog(int lekarID, int bolnicaID, [FromBody] Zaposlen el)
    {
        try
        {
            var lekar = await Context.Lekari.FindAsync(lekarID);
            var bolnica = await Context.Bolnice.FindAsync(bolnicaID);

            if(lekar != null && bolnica != null && el != null)
            {
                var zaposlen = new Zaposlen
                {
                    Specijalnost = el.Specijalnost,
                    DatumPotpisivanjaUgovora = el.DatumPotpisivanjaUgovora,
                    IdentifikacioniBroj = el.IdentifikacioniBroj,
                    Lekar = lekar,
                    Bolnica = bolnica
                };

                await Context.Zaposleni.AddAsync(zaposlen);
                await Context.SaveChangesAsync();
                return Ok("Zaposleni je dodat");
            }
            else
            {
                return BadRequest("Greska pri unosu");
            }
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("PronadjiLekara/{bolnicaID}")]
    public async Task<ActionResult> PronadjiLekara(int bolnicaID)
    {
        try
        {
            var lekari = await Context.Zaposleni
                .Include(p => p.Bolnica)
                .Include(p => p.Lekar)
                .Where(p => p.Bolnica!.ID == bolnicaID)
                .Select(p => new{
                    p.Lekar,
                    p.Specijalnost
                }).ToListAsync();
            return Ok(lekari);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("BezLicence/{bolnicaID}")]
    public async Task<ActionResult> BezLicence(int bolnicaID)
    {
        try
        {
            var lekari = await Context.Zaposleni
                .Include(p => p.Bolnica)
                .Include(p => p.Lekar)
                .Where(p => p.Bolnica!.ID == bolnicaID)
                .Where(p => p.Lekar!.DatumDobijanjaLicence == null)
                .Select(p => new{
                    p.Lekar, 
                    Licenca = "Nema licencu"
                }).ToListAsync();
            return Ok(lekari);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}
