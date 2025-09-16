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

    [HttpPost("DodavanjeBanke")]
    public async Task<ActionResult> DodavanjeBanke([FromBody]Banka banka)
    {
        try
        {
            await Context.Banke.AddAsync(banka);
            await Context.SaveChangesAsync();
            return Ok($"Banka sa ID-jem: {banka.ID} je dodata.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjeKlijenta")]
    public async Task<ActionResult> DodavanjeKlijenta([FromBody]Klijent klijent)
    {
        try
        {
            await Context.Klijenti.AddAsync(klijent);
            await Context.SaveChangesAsync();
            return Ok($"Klijent sa ID-jem: {klijent.ID} je dodat.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("KreiranjeRacuna/{klijentID}/{bankaID}")]
    public async Task<ActionResult> KreiranjeRacuna(int klijentID, int bankaID, [FromBody]Racun podaci)
    {
        try
        {
            var klijent = await Context.Klijenti.FindAsync(klijentID);
            var banka = await Context.Banke.FindAsync(bankaID);

            if (banka != null && klijent != null)
            {
                var racun = new Racun
                {
                    Banka = banka,
                    Klijent = klijent,
                    BrojRacuna = podaci.BrojRacuna,
                    Otvoren = podaci.Otvoren,
                    PodigaoDoSada = 0,
                    Sredstva = 0
                };

                await Context.Racuni.AddAsync(racun);
                await Context.SaveChangesAsync();
                return Ok($"Klijentu sa ID-jem: {klijent.ID} je dodat racun: {racun.ID} u banci sa ID: {banka.ID}.");
            }
            else
            {
                return BadRequest("Ne postoji banka ili klijent.");
            }
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("PromenaStanja/{brojRacuna}/{novoStanje}")]
    public async Task<ActionResult> PromenaStanja(int brojRacuna, int novoStanje)
    {
        try
        {
            var racun = await Context.Racuni.Where(p => p.BrojRacuna == brojRacuna).FirstOrDefaultAsync();

            if (racun != null)
            {
                if (racun.Sredstva > novoStanje)
                {
                    racun.PodigaoDoSada += (racun.Sredstva - novoStanje);
                }
                racun.Sredstva = novoStanje;
                await Context.SaveChangesAsync();
                return Ok($"Novo stanje je: {racun.Sredstva}.");
            }
            else
            {
                return BadRequest("Ne postoji račun.");
            }
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("Ukupno/{bankaID}")]
    public async Task<ActionResult> Ukupno(int bankaID)
    {
        try
        {
            var sredstvaBanke = await Context.Banke
                .Include(p => p.Racuni)
                .Where(p => p.ID == bankaID)
                .SelectMany(p => p.Racuni!)
                .SumAsync(p => p.PodigaoDoSada + p.Sredstva);

            return Ok($"Ukupna sredstva koje je banka imala i ima su: {sredstvaBanke}");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}
