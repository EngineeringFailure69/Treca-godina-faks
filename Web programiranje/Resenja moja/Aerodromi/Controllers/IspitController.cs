namespace WebTemplate.Controllers;

[ApiController]
[Route("[controller]")]
public class IspitController : ControllerBase
{
    public IspitContext Context { get; set; }

    public IspitController(IspitContext context)
    {
        Context=context;
    }

    [HttpPost("DodavanjeAerodroma")]
    public async Task<ActionResult> DodavanjeAerodroma([FromBody]Aerodrom aerodrom)
    {
        try
        {
            await Context.Aerodromi.AddAsync(aerodrom);
            await Context.SaveChangesAsync();
            return Ok($"Dodat je aerodrom sa ID: {aerodrom.ID}");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjeLetelice")]
    public async Task<ActionResult> DodavanjeLetelice([FromBody]Letelica letelica)
    {
        try
        {
            await Context.Letelice.AddAsync(letelica);
            await Context.SaveChangesAsync();
            return Ok($"Dodata je letelica sa ID: {letelica.ID}");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [ProducesResponseType(StatusCodes.Status403Forbidden)]
    [ProducesResponseType(StatusCodes.Status400BadRequest)]
    [ProducesResponseType(StatusCodes.Status200OK)]
    [HttpPost("DodavanjeLeta/{aerodromPoletanjaID}/{aerodromSletanjaID}/{letelicaID}/{brojPutnika}/{vremePoletanja}/{vremeSletanja}")]
    public async Task<ActionResult> DodavanjeLeta(int aerodromPoletanjaID, int aerodromSletanjaID, int letelicaID, uint brojPutnika, DateTime vremePoletanja, DateTime vremeSletanja)
    {
        try
        {
            if(aerodromPoletanjaID==aerodromSletanjaID)
            {
                return StatusCode(StatusCodes.Status403Forbidden, "Nemoguce je poleteti i sleteti na isti aerodrom");
            }
            if(vremePoletanja==vremeSletanja)
            {
                return StatusCode(StatusCodes.Status403Forbidden, "Nemoguce je poleteti i sleteti u isto vreme");
            }

            var aerodromPoletanja = await Context.Aerodromi.FindAsync(aerodromPoletanjaID);
            var aerodromSletanja = await Context.Aerodromi.FindAsync(aerodromSletanjaID);
            var letelica = await Context.Letelice.FindAsync(letelicaID);

            if(aerodromPoletanja!=null && aerodromSletanja!=null && letelica!=null)
            { 
                if(letelica.KapacitetPutnika < brojPutnika)
                {
                    return BadRequest("Letelica ne moze da prihvati sve putnike");
                }
                var let = new Let
                {
                    AerodromPoletanja=aerodromPoletanja,
                    AerodromSletanja = aerodromSletanja,
                    Letelica=letelica,
                    BrojPutnikaNaLetu=brojPutnika,
                    VremePoletanja=vremePoletanja,
                    VremeSletanja=vremeSletanja
                };

                await Context.Letovi.AddAsync(let);
                await Context.SaveChangesAsync();
                return Ok($"Dodat je let u bazu podataka");
            }
            else
            {
                return BadRequest("Nemoguce je pronaci aerodrom ili letelicu sa zadatim ID-jem");
            }
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("InformacijeOLetu/{aerodromPoletanjaID}")]
    public async Task<ActionResult> InformacijeOLetu(int aerodromPoletanjaID)
    {
        try
        {
            var letovi = await Context.Letovi.Include(p => p.AerodromPoletanja)
                          .Include(p => p.AerodromSletanja)
                          .Include(p => p.Letelica)
                          .Where(p => p.AerodromPoletanja != null && p.AerodromPoletanja.ID == aerodromPoletanjaID)
                          .Select(p => new{
                            AerodromPoletanja = p.AerodromPoletanja!.Naziv,
                            AerodromSletanja = p.AerodromSletanja!.Naziv,
                            Letelica = p.Letelica!.Naziv
                          }).ToListAsync();
            return Ok(letovi);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("ProsecnaDuzinaLeta/{aerodromPoletanjaID}/{aerodromSletanjaID}")]
    public async Task<ActionResult> ProsecnaDuzinaLeta(int aerodromPoletanjaID, int aerodromSletanjaID)
    {
        try
        {
            var aerodromPoletanja = await Context.Aerodromi.FindAsync(aerodromPoletanjaID);
            var aerodromSletanja = await Context.Aerodromi.FindAsync(aerodromSletanjaID);

            var prosecnoVreme = await Context.Letovi
                .Include(p => p.AerodromPoletanja)
                .Include(p => p.AerodromSletanja)
                .Where(p => p.AerodromPoletanja!.ID == aerodromPoletanjaID && p.AerodromSletanja!.ID == aerodromSletanjaID)
                .ToListAsync();

                if(prosecnoVreme.Count <= 0)
                    return BadRequest("Nije pronadjen ni jedan let izmedju aerodroma.");
                
                double prosecnoSekundi = prosecnoVreme.Average(p => (p.VremeSletanja - p.VremePoletanja).TotalSeconds);
                var tsps = TimeSpan.FromSeconds(prosecnoSekundi);

            return Ok($"Prosecna duzina leta izmedju aerodroma: {aerodromPoletanja?.Naziv} i {aerodromSletanja?.Naziv} je: {tsps}");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}