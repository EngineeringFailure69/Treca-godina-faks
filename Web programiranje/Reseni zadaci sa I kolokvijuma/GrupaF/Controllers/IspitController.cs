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

    [HttpPost("DodavanjeRestorana")]
    public async Task<ActionResult> DodavanjeRestorana([FromBody]Restoran restoran)
    {
        try
        {
            await Context.Restorani.AddAsync(restoran);
            await Context.SaveChangesAsync();
            return Ok($"Restoran sa ID: {restoran.ID} je dodat.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjeKuvara")]
    public async Task<ActionResult> DodavanjeKuvara([FromBody]Kuvar kuvar)
    {
        try
        {
            await Context.Kuvari.AddAsync(kuvar);
            await Context.SaveChangesAsync();
            return Ok($"Kuvar sa ID: {kuvar.ID} je dodat.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("Zaposljavanje/{restoranID}/{kuvarID}")]
    public async Task<ActionResult> Zaposljavanje(int restoranID, int kuvarID, [FromBody]Zaposlen podaci)
    {
        try
        {
            var kuvar = await Context.Kuvari.FindAsync(kuvarID);
            var restoran = await Context.Restorani.Include(p => p.Zaposleni!).ThenInclude(p => p.Kuvar).FirstOrDefaultAsync(p => p.ID == restoranID);

            if (restoran != null && kuvar != null && restoran.Zaposleni!.Any(p => p.Kuvar!.ID == kuvar.ID))
            {
                return BadRequest("Kuvar je vec zaposlen u restoranu!");
            }

            if (restoran != null && restoran.MaxBrojKuvara <= restoran.Zaposleni!.Count)
            {
                return BadRequest("Nemoguće zaposliti kuvara!");
            }

            if (restoran != null && kuvar != null)
            {
                var zaposlen = new Zaposlen
                {
                    Pozicija = podaci.Pozicija,
                    DatumZaposlenja = podaci.DatumZaposlenja,
                    Plata = podaci.Plata,
                    Kuvar = kuvar,
                    Restoran = restoran
                };

                await Context.Zaposleni.AddAsync(zaposlen);
                await Context.SaveChangesAsync();
                return Ok($"Zaposlen je kuvar sa ID: {kuvar.ID} u restoranu sa ID: {restoran.ID}");
            }
            else
            {
                return BadRequest("Restoran ili kuvar nisu pronadjeni!");
            }
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("IzracunajPlatuKuvara/{kuvarID}")]
    public async Task<ActionResult> IzracunajPlatuKuvara(int kuvarID)
    {
        try
        {
            var kuvar = await Context.Kuvari
                .Include(p => p.ZaposlenU)
                .FirstOrDefaultAsync(p => p.ID == kuvarID);
            
            if (kuvar == null)
            {
                return BadRequest("Kuvar nije pronadjen!");
            }

            return Ok(kuvar.ZaposlenU!.Sum(p => p.Plata));
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("RadiUNajviseRestorana")]
    public async Task<ActionResult> RadiUNajviseRestorana()
    {
        try
        {
            var kuvar = await Context.Kuvari
                .Include(p => p.ZaposlenU)
                .OrderByDescending(p => p.ZaposlenU!.Count)
                .FirstOrDefaultAsync();

            return Ok(kuvar);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}
