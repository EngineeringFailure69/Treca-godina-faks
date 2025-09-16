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

    [Route("/DodajKompaniju")]
    [HttpPost]
    public async Task<ActionResult> DodajKompaniju([FromBody]Kompanija kompanija)
    {
        try
        {
            await Context.Kompanije.AddAsync(kompanija);
            await Context.SaveChangesAsync();
            return Ok("Uspesno upisana kompanija");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [Route("/DodajVozilo")]
    [HttpPost]
    public async Task<ActionResult> DodajVozilo([FromBody]Vozilo vozilo)
    {
        try
        {
            var kompanija = await Context.Kompanije.FindAsync(vozilo.KompanijinoVozilo?.ID);
            if (kompanija == null)
                return BadRequest("Kompanija ne postoji");

            vozilo.KompanijinoVozilo = kompanija;
            await Context.Vozila.AddAsync(vozilo);
            await Context.SaveChangesAsync();
            return Ok("Uspesno upisano vozilo");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [Route("/DodajRobu")]
    [HttpPost]
    public async Task<ActionResult> DodajRobu([FromBody]Roba roba)
    {
        try
        {
            await Context.Roba.AddAsync(roba);
            await Context.SaveChangesAsync();
            return Ok("Uspesno upisana roba");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/PronadjiKompanijuIVozilo/{zapremina}/{tezina}/{datumPrijema}/{datumDostave}/{cenaOd}/{cenaDo}")]
    public async Task<ActionResult> PronadjiKompanijuIVozilo(int zapremina, int tezina, DateTime datumPrijema, 
                                                            DateTime datumDostave, int cenaOd, int cenaDo)
    {
        try
        {
            var vozila = await Context.Vozila.Where(v=>v.Zapremina>=zapremina && v.Tezina>=tezina && v.DatumPrijema<=datumPrijema &&
                                                    v.DatumIsporuke>=datumDostave && v.Cena<=cenaDo && v.Cena>=cenaOd)
                                             .Include(v => v.KompanijinoVozilo)
                                             .ToListAsync();
            var rezultat = vozila.Select(v=> new
            {
                SlikaPath=$"/images/{Path.GetFileName(v.SlikaPath)}",
                Kompanija = new 
                {
                    v.KompanijinoVozilo?.ID,
                    v.KompanijinoVozilo?.Naziv,
                    v.KompanijinoVozilo?.ProsecnaZarada,
                    v.KompanijinoVozilo?.Cena
                }
            });
            return Ok(rezultat);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [Route("/Isporuci/{idKompanije}")]
    [HttpPut]
    public async Task<ActionResult> Isporuci(int idKompanije)
    {
        var kom = await Context.Kompanije.Where(p => p.ID == idKompanije).FirstOrDefaultAsync();
        if(kom == null)
            return BadRequest("Nema kompanije");
        
        try
        {
            kom.ProsecnaZarada += kom.Cena;
            await Context.SaveChangesAsync();
            return Ok(kom);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}