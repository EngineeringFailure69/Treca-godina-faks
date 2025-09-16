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
            return Ok("Prodavnica uspesno dodata");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajProizvod")]
    public async Task<ActionResult> DodajProizvod([FromBody]Proizvod proizvod)
    {
        try
        {
            await Context.Proizvodi.AddAsync(proizvod);
            await Context.SaveChangesAsync();
            return Ok(proizvod.ID);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajProizvodUProdavnicu/{proizvodId}/{prodavnicaId}/{kolicina}")]
    public async Task<ActionResult> DodajProizvodUProdavnicu(int proizvodId, int prodavnicaId, uint kolicina)
    {
        try
        {
            var proizvod = await Context.Proizvodi.FindAsync(proizvodId);
            var prodavnica = await Context.Prodavnice.FindAsync(prodavnicaId);

            if(proizvod==null || prodavnica==null)
                return BadRequest("Prodavnica ili proizvod ne postoje u bazi");
            if(kolicina>100 || kolicina<=0)
                return BadRequest("Uneli ste kolicinu vecu od 100 ili manju, jednaku nuli");

            var pripada = new Pripada
            {
                Prodavnica=prodavnica,
                Proizvod=proizvod,
                Kolicina=kolicina
            };

            await Context.Pripada.AddAsync(pripada);
            await Context.SaveChangesAsync();
            return Ok("Uspesno unet proizvod u prodavnicu");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiSveProdavnice")]
    public async Task<ActionResult> VratiSveProdavnice()
    {
        try
        {
            var prodavnice = await Context.Prodavnice
                            .Select(p => new{
                                p.ID,
                                p.Naziv
                                //p.Lokacija,
                                //p.BrTelefona
                            }).ToListAsync();
            return Ok(prodavnice);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiSveKategorije")]
    public async Task<ActionResult> VratiSveKategorije()
    {
        try
        {
            var kategorije = await Context.Proizvodi.Select(p => new{
                p.Kategorija
            }).ToListAsync();
            return Ok(kategorije);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/ProizvodUProdavnici/{prodavnicaId}")]
    public async Task<ActionResult> VratiProizvodeUProdavnici(int prodavnicaId)
    {
        try
        {
            var prodavnica = await Context.Prodavnice.FindAsync(prodavnicaId);
            if (prodavnica == null)
                return BadRequest("Prodavnica ne postoji u bazi");
            
            var proizvodi = await Context.Pripada.Include((p) => p.Prodavnica)
                                                 .Include((p) => p.Proizvod)
                                                 .Where((p)=>p.Prodavnica!.ID==prodavnicaId)
                                                 .Select(p=>new{
                                                    p.Proizvod!.ID,
                                                    p.Proizvod.Naziv,
                                                    p.Proizvod.Cena,
                                                    p.Proizvod.Kategorija,
                                                    p.Kolicina
                                                 }).ToListAsync();
            return Ok(proizvodi);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPut]
    [Route("/ProdajProizvod/{prodavnicaId}/{proizvodId}/{kolicina}")]
    public async Task<ActionResult> ProdajProizvod(int prodavnicaId, int proizvodId, uint kolicina)
    {
        try
        {
            var prodavnica = await Context.Prodavnice.FindAsync(prodavnicaId);
            var proizvod = await Context.Proizvodi.FindAsync(proizvodId);

            if(prodavnica==null || proizvod==null)
                return BadRequest("Ne postoji proizvod ili prodavnica u bazi");

            var pripada = await Context.Pripada.Include(p=>p.Proizvod)
                                               .Include(p=>p.Prodavnica)
                                               .Where(p=>p.Prodavnica!.ID==prodavnicaId 
                                               && p.Proizvod!.ID==proizvodId)
                                               .FirstOrDefaultAsync(); 
            if(pripada==null)
                return BadRequest("Proizvod ne postoji u prodavnici");

            if(pripada.Kolicina<kolicina)
                return BadRequest("Nema dovoljno proizvoda u prodavnici za prodaju");
            
            pripada.Kolicina = pripada.Kolicina  - kolicina;
            Context.Pripada.Update(pripada);
            await Context.SaveChangesAsync();
            return Ok($"Od proizvoda:{proizvodId} u prodavnici:{prodavnicaId} je prodato:{kolicina} i ostalo je:{pripada.Kolicina}");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpDelete]
    [Route("/ObrisiProizvod/{prodavnicaId}/{proizvodId}")]
    public async Task<ActionResult> ObrisiProizvod(int prodavnicaId, int proizvodId)
    {
        try
        {
            var proizvod = await Context.Pripada.Include(p=>p.Prodavnica)
                                                .Include(p=>p.Proizvod)
                                                .Where(p=>p.Proizvod!.ID==proizvodId 
                                                && p.Prodavnica!.ID==prodavnicaId)
                                                .FirstOrDefaultAsync();
            if(proizvod==null)
                return BadRequest("Proizvod ne postoji u prodavnici");
            
            Context.Pripada.Remove(proizvod);
            await Context.SaveChangesAsync();
            return Ok($"Proizvod:${proizvodId} je uspesno obrisan iz prodavnice:${prodavnicaId}");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}