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

    [HttpPost("DodatiAerodrom")]
    public async Task<ActionResult> DodatiAerodrom([FromBody]Aerodrom aerodrom)
    {
        try
        {
            await Context.Aerodromi.AddAsync(aerodrom);
            await Context.SaveChangesAsync();
            return Ok($"Dodat je aerodrom sa ID: {aerodrom.ID}");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajLetelicu")]
    public async Task<ActionResult> DodajLetelicu([FromBody]Letelica letelica)
    {
        try
        {
            await Context.Letelice.AddAsync(letelica);
            await Context.SaveChangesAsync();
            return Ok($"Dodata je letelica sa ID: {letelica.ID}");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajLet/{aerodromPoletanja}/{aerodromSletanja}/{letelicaID}/{vremePoletanja}/{vremeSletanja}")]
    public async Task<ActionResult> DodajLet(int aerodromPoletanja, int aerodromSletanja, int letelicaID, DateTime vremePoletanja, DateTime vremeSletanja)
    {
        try
        {
            if (vremeSletanja <= vremePoletanja)
            {
                return BadRequest("Vreme sletanja ne sme da bude isto ili pre vremena poletanja!");
            }

            if (aerodromPoletanja == aerodromSletanja)
            {
                return BadRequest("Aerodromi poletanja i sletanja ne smeju da budu isti!");
            }

            var aPol = await Context.Aerodromi.FindAsync(aerodromPoletanja);
            var aSle = await Context.Aerodromi.FindAsync(aerodromSletanja);
            var letelica = await Context.Letelice.FindAsync(letelicaID);

            if (aPol != null && aSle != null && letelica != null)
            {
                var let = new Let
                {
                    AerodromPoletanja = aPol,
                    AerodromSletanja = aSle,
                    Letelica = letelica,
                    VremePoletanja = vremePoletanja,
                    VremeSletanja = vremeSletanja
                };

                await Context.Letovi.AddAsync(let);
                await Context.SaveChangesAsync();
                return Ok($"Dodat je let između aerodroma: {aPol.ID} i {aSle.ID}, sa ID: {let.ID}");
            }

            return BadRequest("Greška u prosleđenim podacima. Nemoguće pronaći aerodrome ili letelicu u bazi.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("PronadjiLet/{aerodromPoletanja}")]
    public async Task<ActionResult> PronadjiLet(int aerodromPoletanja)
    {
        try
        {
            var svi = Context.Letovi
                .Include(p => p.AerodromPoletanja)
                .Include(p => p.AerodromSletanja)
                .Include(p => p.Letelica)
                .Where(p => p.AerodromPoletanja!.ID == aerodromPoletanja)
                .Select(p => new {
                    p.AerodromPoletanja,
                    p.AerodromSletanja,
                    p.Letelica,
                    p.VremePoletanja,
                    p.VremeSletanja
                });

            return Ok(await svi.ToListAsync());
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("ProsecnaDuzinaLeta/{aerodrom1}/{aerodrom2}")]
    public async Task<ActionResult> ProsecnaDuzinaLeta(int aerodrom1, int aerodrom2)
    {
        try
        {
            var prosek = await Context.Letovi
                .Include(p => p.AerodromPoletanja)
                .Include(p => p.AerodromSletanja)
                .Where(p =>
                        (p.AerodromPoletanja!.ID == aerodrom1 &&
                         p.AerodromSletanja!.ID == aerodrom2) || 
                        (p.AerodromPoletanja!.ID == aerodrom2 &&
                         p.AerodromSletanja!.ID == aerodrom1))
                .ToListAsync();

            if (prosek.Count == 0)
            {
                return BadRequest("Nema letova izmedju zadatih aerodroma!");
            }

            return Ok(TimeSpan
                .FromSeconds(prosek
                    .Average(p => (p.VremeSletanja - p.VremePoletanja).TotalSeconds)));
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}
