using WebtTemplate.Models;

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
    [Route("/DodajMarku")]
    public async Task<ActionResult> DodajMarku([FromBody]Marka marka)
    {
        try
        {
            await Context.Marke.AddAsync(marka);
            await Context.SaveChangesAsync();
            return Ok("Marka je dodata");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajModel/{idMarka}/{model}")]
    public async Task<ActionResult> DodajModel(int idMarka, string model)
    {
        try
        {
            var marka = await Context.Marke.Where(p=>p.ID==idMarka).FirstOrDefaultAsync();
            if(marka==null)
                return BadRequest("Ne postoji marka");
            
            Model m=new Model
            {
                Naziv = model,
                Marka = marka 
            };
            
            await Context.Modeli.AddAsync(m);
            await Context.SaveChangesAsync();
            return Ok("Model je dodat");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajBoju/{idModel}/{boja}")]
    public async Task<ActionResult> DodajBoju(int idModel, string boja)
    {
        try
        {
            var model = await Context.Modeli.Where(p=>p.ID==idModel).FirstOrDefaultAsync();
            if(model==null)
                return BadRequest("Ne postoji model");
            
            Boja m=new Boja
            {
                Naziv = boja,
                Model = model 
            };
            
            await Context.Boje.AddAsync(m);
            await Context.SaveChangesAsync();
            return Ok("Boja je dodata");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [Route("/DodajAutomobil")]
    [HttpPost]
    public async Task<ActionResult> DodajAutomobil([FromBody] Automobil auto)
    {
        try
        {
            // Proveri da li Marka postoji, ako ne postoji, vrati grešku
            var marka = await Context.Marke.FindAsync(auto.Marka?.ID);
            if (marka == null)
            {
                return BadRequest("Marka ne postoji.");
            }
            auto.Marka = marka;
            // Proveri da li Model postoji, ako ne postoji, vrati grešku
            var model = await Context.Modeli.FindAsync(auto.Model?.ID);
            if (model == null)
            {
                return BadRequest("Model ne postoji.");
            }
            auto.Model = model;
            // Proveri da li Boja postoji, ako ne postoji, vrati grešku
            var boja = await Context.Boje.FindAsync(auto.Boja?.ID);
            if (boja == null)
            {
                return BadRequest("Boja ne postoji.");
            }
            auto.Boja = boja;

            await Context.Automobili.AddAsync(auto);
            await Context.SaveChangesAsync();

            return Ok("Automobil je dodat");
        }
        catch (DbUpdateException ex)
        {
            // Vratite detalje o grešci u vezi sa bazom podataka
            return BadRequest($"Greška pri čuvanju podataka: {ex.InnerException?.Message}");
        }
        catch (Exception e)
        {
            // Vratite generičku grešku
            return BadRequest($"Neočekivana greška: {e.Message}");
        }
    }

    [HttpGet]
    [Route("/PreuzmiMarku")]
    public async Task<ActionResult> PreuzmiMarku()
    {
        try
        {
            var marke = await Context.Marke.ToListAsync();
            return Ok(marke);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/PreuzmiModel/{idMarke}")]
    public async Task<ActionResult> PreuzmiModel(int idMarke)
    {
        try
        {
            var modeli = await Context.Modeli.Include(p=>p.Marka)
                                            .Where(p=>p.Marka.ID==idMarke)
                                            .ToListAsync();
            var model = modeli.Select(m=>new{
                ID = m.ID,
                Naziv = m.Naziv
            }).ToList();
            return Ok(model);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/PreuzmiBoju/{idMarke}")]
    public async Task<ActionResult> PreuzmiBoju(int idMarke)
    {
        try
        {
            var boje = await Context.Boje.Include(p=>p.Model)
                                         .ThenInclude(m=>m.Marka)
                                         .Where(p=>p.Model.Marka.ID==idMarke)
                                         .ToListAsync();
            var boja = boje.Select(b=>new{
                ID=b.ID,
                Naziv=b.Naziv
            }).ToList();
            return Ok(boja);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [Route("/PronadjiAutomobil")]
    [HttpGet]
    public async Task<ActionResult> PronadjiAutomobil([FromQuery] int? marka, [FromQuery] int? model, [FromQuery] int? boja)
    {
        var query = Context.Automobili.AsQueryable();

        if (marka.HasValue)
        {
            query = query.Where(p => p.Marka.ID == marka.Value);
        }
        if (model.HasValue)
        {
            query = query.Where(p => p.Model.ID == model.Value);
        }
        if (boja.HasValue)
        {
            query = query.Where(p => p.Boja.ID == boja.Value);
        }

        // Filtriraj automobile čija je količina veća od 0
        query = query.Where(p => p.Kolicina > 0);

        var automobili = await query
            .Select(p => new
            {
                MarkaNaziv = p.Marka.Naziv,
                ModelNaziv = p.Model.Naziv,
                SlikaPath = $"/images/{Path.GetFileName(p.SlikaPath)}", // Pretvori putanju u URL
                p.Kolicina,
                p.DatumPoslednjeProdaje,
                p.Cena
            })
            .ToListAsync();

        if (automobili == null || !automobili.Any())
        {
            return NotFound("Ne postoje automobili sa zadatim kriterijumima.");
        }

        return Ok(automobili);
    }

    [Route("/NaruciAutomobil/{marka}/{model}")]
    [HttpPut]
    public async Task<ActionResult> NaruciAutomobil(string marka, string model)
    {
        var ma = Context.Marke.Where(p => p.Naziv == marka).FirstOrDefault();
        if(ma == null)
        {
            return BadRequest("Ne postoji takva marka");
        }

        var mo = Context.Modeli.Where(p => p.Naziv == model).FirstOrDefault();
        if(mo == null)
        {
            return BadRequest("Ne postoji takav model");
        }

        try
        {
            var auto = await Context.Automobili.Where(p => p.Marka.Naziv == marka && p.Model.Naziv == model).FirstOrDefaultAsync();
            if(auto==null)
                return BadRequest("Ne postoji auto");
            auto.Kolicina--;

            if(auto.Kolicina < 1)
            {
                Context.Automobili.Remove(auto);
            }
            else
            {
                Context.Automobili.Update(auto);
            }
            await Context.SaveChangesAsync();
            
            return Ok("Auto uspesno porucen");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}