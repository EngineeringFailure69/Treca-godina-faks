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
    
    [HttpPost("DodajVoz")]
    public async Task<ActionResult> DodajVoz([FromBody] Voz el)
    {
        try
        {
            await Context.Vozovi.AddAsync(el);
            await Context.SaveChangesAsync();
            return Ok("Voz je dodat u bazu");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajGrad")]
    public async Task<ActionResult> DodajGrad([FromBody] Grad el)
    {
        try
        {
            await Context.Gradovi.AddAsync(el);
            await Context.SaveChangesAsync();
            return Ok("Grad je dodat u bazu");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajRelaciju/{vozID}/{gradPolasakaID}/{gradDolaskaID}/{brojPutnika}/{cenaKarte}/{datumSaobracanja}")]
    public async Task<ActionResult> DodajRelaciju(int vozID, int gradPolasakaID, int gradDolaskaID, 
        uint brojPutnika, double cenaKarte, DateTime datumSaobracanja)
        {
            try
            {
                var voz = await Context.Vozovi.FindAsync(vozID);
                var gradPolasaka = await Context.Gradovi.FindAsync(gradPolasakaID);
                var gradDolaska = await Context.Gradovi.FindAsync(gradDolaskaID);

                if(voz != null && gradPolasaka != null && gradDolaska != null)
                {
                    if(brojPutnika > voz.MaxKapacitetPutnika)
                    {
                        return BadRequest("Kapacitet voza je manji od broja putnika.");
                    }
                    var relacija = new Relacija
                    {
                        BrojPutinka=brojPutnika,
                        CenaKarte=cenaKarte,
                        DatumSaobracanja=datumSaobracanja,
                        GradPolaska=gradPolasaka,
                        GradDolaska=gradDolaska,
                        Voz=voz
                    };
                    await Context.Relacije.AddAsync(relacija);
                    await Context.SaveChangesAsync();
                    return Ok("Relacija je dodata u bazu");
                }
                else
                {
                    return BadRequest("Gradovi ili voz nisu pronadjeni!");
                }
            }
            catch (Exception e)
            {
                return BadRequest(e.Message);
            }
        }
    
    [HttpGet("PretragaVozova/{gradID}")]
    public async Task<ActionResult> PretragaVozova(int gradID)
    {
        try
        {
            var vozovi = await Context.Relacije
                .Include(p => p.GradPolaska)
                .Include(p => p.GradDolaska)
                .Include(p => p.Voz)
                .Where(p => p.GradPolaska!.ID == gradID || p.GradDolaska!.ID == gradID)
                .Select(p => p.Voz).ToListAsync();
            return Ok(vozovi);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("ZaradaVozaNaRelaciji/{vozID}/{relacijaID}")]
    public async Task<ActionResult> ZaradaVozaNaRelaciji(int vozID, int relacijaID)
    {
         try
        {
            var relacija = await Context.Relacije
                .Include(p => p.GradPolaska)
                .Include(p => p.GradDolaska)
                .Where(p => p.ID == relacijaID)
                .Select(p => new 
                {
                    GradPolaska = p.GradPolaska!.ID,
                    GradDolaska = p.GradDolaska!.ID
                }).FirstOrDefaultAsync();

            if (relacija == null)
            {
                return BadRequest("Relacija ne postoji!");
            }

            var ukupnaCena = await Context.Relacije
                .Include(p => p.GradPolaska)
                .Include(p => p.GradDolaska)
                .Include(p => p.Voz)
                .Where(p => p.GradPolaska!.ID == relacija.GradPolaska ||
                            p.GradPolaska!.ID == relacija.GradDolaska ||
                            p.GradDolaska!.ID == relacija.GradPolaska ||
                            p.GradDolaska!.ID == relacija.GradDolaska)
                .SumAsync(p => p.CenaKarte * p.BrojPutinka);
            return Ok($"Ukupna zarada voza: {vozID} na relaciji: {relacijaID} je {ukupnaCena}.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}
