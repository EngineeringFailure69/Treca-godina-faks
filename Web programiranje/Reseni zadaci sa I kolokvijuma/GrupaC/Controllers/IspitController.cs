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
    public async Task<ActionResult> DodavanjeBolnice([FromBody] Bolnica bolnica)
    {
        try
        {
            await Context.Bolnice.AddAsync(bolnica);
            await Context.SaveChangesAsync();
            return Ok($"Dodata je bolnica sa ID: {bolnica.ID}.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjeLekara")]
    public async Task<ActionResult> DodavanjeLekara([FromBody] Lekar lekar)
    {
        try
        {
            await Context.Lekari.AddAsync(lekar);
            await Context.SaveChangesAsync();
            return Ok($"Dodat je lekar sa ID: {lekar.ID}.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("Zaposljenje/{lekarID}/{bolnicaID}")]
    public async Task<ActionResult> Zaposljenje(int lekarID, int bolnicaID, [FromBody] Zaposlen podaci)
    {
        try
        {
            var bolnica = await Context.Bolnice.FindAsync(bolnicaID);
            var lekar = await Context.Lekari.FindAsync(lekarID);

            if (bolnica != null && lekar != null && podaci != null)
            {
                var zaposljenje = new Zaposlen
                {
                    Specijalnost = podaci.Specijalnost,
                    DatumPotpisivanjaUgovora = podaci.DatumPotpisivanjaUgovora,
                    IdentifikacioniBroj = podaci.IdentifikacioniBroj,
                    Lekar = lekar,
                    Bolnica = bolnica
                };

                await Context.Zaposljenja.AddAsync(zaposljenje);
                await Context.SaveChangesAsync();
                return Ok($"Lekar sa ID: {lekar.ID} je zaposlen u bolnici sa ID: {bolnica.ID}.");
            }
            else
            {
                return BadRequest("Podaci nisu validni. Proverite ID-jeve i podatke iz body-a.");
            }
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("PronalazenjeLekara/{bolnicaID}")]
    public async Task<ActionResult> PronalazenjeLekara(int bolnicaID)
    {
        try
        {
            var lekari = await Context.Zaposljenja
                .Include(p => p.Bolnica)
                .Include(p => p.Lekar)
                .Where(p => p.Bolnica!.ID == bolnicaID)
                .Select(p => new
                {
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
            var lekari = await Context.Zaposljenja
                .Include(p => p.Bolnica)
                .Include(p => p.Lekar)
                .Where(p => p.Bolnica!.ID == bolnicaID)
                .Where(p => p.Lekar!.DatumDobijanjaLicence == null)
                .Select(p => new
                {
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
