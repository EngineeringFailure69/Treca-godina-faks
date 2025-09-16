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

    [HttpPost("DodajGrad")]
    public async Task<ActionResult> DodajGrad([FromBody]Grad grad)
    {
        try
        {
            await Context.Gradovi.AddAsync(grad);
            await Context.SaveChangesAsync();
            return Ok($"Upisan je grad sa ID: {grad.ID}");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajVoz")]
    public async Task<ActionResult> DodajVoz([FromBody]Voz voz)
    {
        try
        {
            await Context.Vozovi.AddAsync(voz);
            await Context.SaveChangesAsync();
            return Ok($"Upisan je voz sa ID: {voz.ID}");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajRelaciju/{gradPolaska}/{gradDolaska}/{vozID}")]
    public async Task<ActionResult> DodajRelaciju(int gradPolaska, int gradDolaska, int vozID, [FromBody]Relacija podaci)
    {
        try
        {
            var polazak = await Context.Gradovi.FindAsync(gradPolaska);
            var dolazak = await Context.Gradovi.FindAsync(gradDolaska);
            var voz = await Context.Vozovi.FindAsync(vozID);

            if (polazak != null && dolazak != null && voz != null)
            {
                if (podaci.BrojPutnika > voz.MaxKapacitetPutnika)
                {
                    return BadRequest("Kapacitet voza je manji od broja putnika.");
                }

                var relacija = new Relacija
                {
                    BrojPutnika = podaci.BrojPutnika,
                    CenaKarte = podaci.CenaKarte,
                    Datum = podaci.Datum,
                    GradPolaska = polazak,
                    GradDolaska = dolazak,
                    Voz = voz
                };

                await Context.Relacije.AddAsync(relacija);
                await Context.SaveChangesAsync();
                return Ok($"Upisana je relacija izmedju gradova: {polazak.ID} i {dolazak.ID}, sa vozom: {voz.ID}");
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
                .SumAsync(p => p.CenaKarte * p.BrojPutnika);
            return Ok($"Ukupna zarada voza: {vozID} na relaciji: {relacijaID} je {ukupnaCena}.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}
