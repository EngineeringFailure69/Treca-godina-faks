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

    [HttpPost]
    [Route("/DodajProdavnicu")]
    public async Task<ActionResult> DodajProdavnicu([FromBody]Prodavnica prodavnica)
    {
        try
        {
            await Context.Prodavnice.AddAsync(prodavnica);
            await Context.SaveChangesAsync();
            return Ok("Uspesno dodata prodavnica");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajDimenziju")]
    public async Task<ActionResult> DodajDimenziju([FromBody]Dimenzija dimenzija)
    {
        try
        {
            await Context.Dimenzije.AddAsync(dimenzija);
            await Context.SaveChangesAsync();
            return Ok("Uspesno dodata dimenzija");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajPapir")]
    public async Task<ActionResult> DodajPapir([FromBody]Papir papir)
    {
        try
        {
            await Context.Papiri.AddAsync(papir);
            await Context.SaveChangesAsync();
            return Ok("Uspesno dodat papir");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajRam")]
    public async Task<ActionResult> DodajRam([FromBody]Ram ram)
    {
        try
        {
            var dimenzija = await Context.Dimenzije.FindAsync(ram.DimenzijaRama?.ID);
            if(dimenzija==null)
                return BadRequest("Ne postoji dimenzija");
            ram.DimenzijaRama = dimenzija;

            await Context.Ramovi.AddAsync(ram);
            await Context.SaveChangesAsync();
            return Ok("Uspesno dodat ram");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajFotografiju")]
    public async Task<ActionResult> DodajFotografiju([FromBody]Fotografija fotografija)
    {
        try
        {
            var prodavnica = await Context.Prodavnice.FindAsync(fotografija.Prodavnica?.ID);
            if(prodavnica == null)
                return BadRequest("Ne postoji prodavnica");
            fotografija.Prodavnica=prodavnica;

            var papir = await Context.Papiri.FindAsync(fotografija.Papir?.ID);
            if(papir==null)
                return BadRequest("Ne postoji papir");
            fotografija.Papir=papir;

            var dimenzija = await Context.Dimenzije.FindAsync(fotografija.Dimenzija?.ID);
            if(dimenzija==null)
                return BadRequest("Ne postoji dimenzija");
            fotografija.Dimenzija=dimenzija;

            var ram = await Context.Ramovi.FindAsync(fotografija.Ram?.ID);
            if(ram==null)
                return BadRequest("Ne postoji ram");
            fotografija.Ram=ram;

            await Context.Fotografije.AddAsync(fotografija);
            await Context.SaveChangesAsync();
            return Ok("Uspesno dodata fotografija");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiProdavnicu")]
    public async Task<ActionResult> VratiProdavnicu()
    {
        try
        {
            var prodavnica = await Context.Prodavnice.ToListAsync();
            if(prodavnica == null)
                return BadRequest("Ne postoji prodavnica");
            var papir = prodavnica.Select(p => new{
                naziv = p.Naziv,
                zarada=p.DnevnaZarada
            });
            return Ok(prodavnica);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/UzmiRamPoDimenziji/{idDimenzije}")]
    public async Task<ActionResult> UzmiRamPoDimenziji(int idDimenzije)
    {
        try
        {
            var ramovi = await Context.Ramovi.Include(p=>p.DimenzijaRama)
                                             .Where(p=>p.DimenzijaRama.ID==idDimenzije).ToListAsync();
            if(ramovi==null)
                return BadRequest("Nije pronadjen ram sa tom dimenzijom");
            var ram = ramovi.Select(p=> new{
                ID=p.ID,
                Materijal=p.Materijal
            });
            return Ok(ram);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/UzmiSveDimenzije")]
    public async Task<ActionResult> UzmiSveDimenzije()
    {
        try
        {
            var dimenzije = await Context.Dimenzije.ToListAsync();
            if(dimenzije==null)
                return BadRequest("Ne postoje dimenzije");
           // var dimenzija = dimenzije.Select(p => $"{p.Visina} x {p.Sirina}");
             var dimenzija = dimenzije.Select(p => new{
                id = p.ID, 
                visina = p.Visina,
                sirina=p.Sirina
            }); //- funkcija koja vraca u standardnom formatu 
            return Ok(dimenzija);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiSvePapire")]
    public async Task<ActionResult> VratiSvePapire()
    {
        try
        {
            var papiri = await Context.Papiri.ToListAsync();
            if(papiri == null)
                return BadRequest("Ne postoje papiri");
            var papir = papiri.Select(p => new{
                naziv = p.Naziv
            });
            return Ok(papir);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiSveSlikeUZavisnostiOdKriterijuma")]
    public async Task<ActionResult> VratiSveSlikeUZavisnostiOdKriterijuma([FromQuery] int? dimenzija, [FromQuery] int? papir, [FromQuery] int? ram)
    {
        try
        {
            var query = Context.Fotografije
                   .Include(p => p.Dimenzija)  // Dodajte ovo ako `Dimenzija` nije automatski učitana
                   .AsQueryable();

            if(dimenzija.HasValue)
                query = query.Where(p => p.Dimenzija.ID == dimenzija.Value);

            if(papir.HasValue)
                query = query.Where(p => p.Papir.ID == papir.Value);

            if(ram.HasValue)
                query = query.Where(p => p.Ram.ID == ram.Value);

            
            query = query.Where(p => p.BrFotografija > 0); 

            var fotografije = await query.Select(p => new {
                id=p.ID,
                NazivSlike=p.Naziv,
                Slika = $"/images/{Path.GetFileName(p.SlikaPath)}",
                NazivPapira = p.Papir.Naziv, 
                dimenzija = p.Dimenzija,
                brFotografija = p.BrFotografija
            }).ToListAsync();

            if (fotografije == null || !fotografije.Any())
            {
                return NotFound("Ne postoje slike sa zadatim kriterijumima.");
            }
            
            return Ok(fotografije);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [Route("/KupiFotografiju/{id}")]
    [HttpPut]
    public async Task<ActionResult> KupiFotografiju(int id)
    {
        var fotografija = await Context.Fotografije
            .Include(f => f.Ram) // Pretpostavljam da postoji veza između fotografije i rama
            .Where(p => p.ID == id)
            .FirstOrDefaultAsync();

        if (fotografija == null)
        {
            return BadRequest("Fotografija nije pronađena.");
        }

        try
        {
            // Smanjujemo količinu fotografije
            fotografija.BrFotografija--;

            // Smanjujemo količinu rama
            var ram = fotografija.Ram;
            if (ram != null)
            {
                ram.BrRamova--;
            }

            if (fotografija.BrFotografija < 1)
            {
                Context.Fotografije.Remove(fotografija);
            }
            else
            {
                Context.Fotografije.Update(fotografija);
            }

            if (ram != null)
            {
                if (ram.BrRamova < 1)
                {
                    return BadRequest("Nema dovoljno ramova na stanju");
                }
                else
                {
                    Context.Ramovi.Update(ram);
                }
            }

            await Context.SaveChangesAsync();
            
            return Ok("Fotografija uspesno kupljena.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}