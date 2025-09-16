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
    
    [HttpPost("DodajBanku")]
    public async Task<ActionResult> DodajBanku([FromBody] Banka el)
    {
        try
        {
            await Context.Banke.AddAsync(el);
            await Context.SaveChangesAsync();
            return Ok("Dodata je banka");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajKlijenta")]
    public async Task<ActionResult> DodajKlijenta([FromBody] Klijent el)
    {
        try
        {
            await Context.Klijenti.AddAsync(el);
            await Context.SaveChangesAsync();
            return Ok("Dodat je klijent");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajRacun/{bankaID}/{klijentID}/{brRacuna}/{datumOtvaranja}/{sredstva}/{podigao}")]
    public async Task<ActionResult> DodajRacun(int bankaID, int klijentID, int brRacuna, DateTime datumOtvaranja, int sredstva, int podigao)
    {
        
        try
        {
           var banka = await Context.Banke.FindAsync(bankaID);
           var klijent = await Context.Klijenti.FindAsync(klijentID);

           if(banka != null && klijent != null)
           {
               var racun = new Racun
               {
                    Banka = banka,
                    Klijent = klijent, 
                    BrojRacuna = brRacuna,
                    DatumOtvaranjaRacuna = datumOtvaranja,
                    Sredstva = sredstva, 
                    PodigaoDoSada = podigao
               };
               await Context.Racuni.AddAsync(racun);
               await Context.SaveChangesAsync();
               return Ok("Dodat je racun");
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

            if(racun!=null)
            {
                if(racun.Sredstva > novoStanje)
                {
                    racun.PodigaoDoSada += (racun.Sredstva - novoStanje); 
                }

                racun.Sredstva = novoStanje;
                await Context.SaveChangesAsync();
                return Ok($"Novo stanje je: {racun.Sredstva}");
            }
            else
           {
                return BadRequest("Ne postoji racun");
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
            var ukupnaSredstvaBanke = await Context.Banke
                .Include(p => p.Racuni)
                .Where(p => p.ID == bankaID)
                .SelectMany(p => p.Racuni!)
                .SumAsync(p => p.PodigaoDoSada + p.Sredstva);

                return Ok($"Ukupna sredstva koja je banka imala do sada su: {ukupnaSredstvaBanke}");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

}
