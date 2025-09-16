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
    [Route("/DodajKorisnika")]
    public async Task<ActionResult> DodajKorisnika([FromBody]Korisnik korisnik)
    {
        try
        {
            await Context.Korisnici.AddAsync(korisnik);
            await Context.SaveChangesAsync();
            return Ok("Korisnik uspesno dodat");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajSobu")]
    public async Task<ActionResult> DodajSobu([FromBody]Soba soba)
    {
        try
        {
            await Context.Sobe.AddAsync(soba);
            await Context.SaveChangesAsync();
            return Ok("Soba uspesno dodata");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

   [HttpPost]
    [Route("/DodajKorisnikaUSobu/{korisnikId}/{imeSobe}")]
    public async Task<ActionResult> DodajKorisnikaUSobu(int korisnikId, string imeSobe)
    {
        try
        {
            var korisnik = await Context.Korisnici.FindAsync(korisnikId);
            if (korisnik == null)
                return BadRequest("Korisnik ne postoji u bazi");

            var soba = await Context.Sobe.Include(s => s.SobaU).FirstOrDefaultAsync(s => s.ImeSobe == imeSobe);
            if (soba == null)
            {
                soba = new Soba
                {
                    ImeSobe = imeSobe,
                    MaksBrojClanova = 5, 
                    SobaU = new List<Pripada>()
                };
                await Context.Sobe.AddAsync(soba);
                await Context.SaveChangesAsync();
            }

             if (soba.SobaU == null)
                soba.SobaU = new List<Pripada>();

            if (soba.SobaU!.Count >= soba.MaksBrojClanova)
                return BadRequest("Maksimalan broj članova u sobi je dostignut");

            var pripada = new Pripada
            {
                Korisnik = korisnik,
                Soba = soba
            };

            if (korisnik.KorisnikUSobi == null)
                korisnik.KorisnikUSobi = new List<Pripada>();

            korisnik.KorisnikUSobi!.Add(pripada);
            soba.SobaU.Add(pripada);
            await Context.Pripada.AddAsync(pripada);
            await Context.SaveChangesAsync();

            return Ok("Korisnik uspešno dodat u sobu");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiSveIzSobe/{sobaId}")]
    public async Task<ActionResult> VratiSveIzSobe(int sobaId)
    {
        try
        {
            var soba = await Context.Sobe.FindAsync(sobaId);
            if(soba == null)
                return BadRequest("Soba ne postoji u bazi");
            
            var ucesnici = await Context.Pripada.Include((p) => p.Soba)
                                                .Include((p) => p.Korisnik)
                                                .Where((p) => p.Soba!.ID == sobaId)
                                                .Select(p => new {
                                                    p.Korisnik!.ID,
                                                    p.Korisnik.Ime,
                                                    p.Korisnik.Prezime,
                                                    p.Korisnik.KorisnickoIme,
                                                    p.Korisnik.Email,
                                                    p.Korisnik.Sifra
                                                }).ToListAsync();
            return Ok(ucesnici);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/PrebrojJedinstvene/{sobaId}")]
    public async Task<ActionResult> PrebrojJedinstvene(int sobaId)
    {
        try
        {
            var soba = await Context.Sobe
                .Include(s => s.SobaU)
                .FirstOrDefaultAsync(s => s.ID == sobaId);
            if (soba == null)
                return BadRequest("Soba ne postoji u bazi");
            
            if (soba.SobaU == null)
            soba.SobaU = new List<Pripada>();
            return Ok($"Broj ucesnika u sobi je: {soba.SobaU!.Count.ToString()}");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiSvaKorisnickaImena")]
    public async Task<ActionResult> VratiSvaKorisnickaImena()
    {
        try
        {
            var imena = await Context.Korisnici.Select(p =>new{ 
                ID = p.ID,
                korisnicko = p.KorisnickoIme
            }).ToListAsync();
            if (imena == null)
                return BadRequest("Ne postoje imena u bazi");
            return Ok(imena);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiPotrebneInformacije")]
    public async Task<ActionResult> VratiPotrebneInformacije()
    {
        try
        {
            var rezultat = await Context.Pripada
                                        .Include(p => p.Korisnik)
                                        .Include(p => p.Soba)
                                        .GroupBy(p => p.Soba!.ImeSobe) // Grupisanje po sobi
                                        .Select(g => new {
                                            Soba = g.Key, // Ime sobe kao ključ
                                            Clanovi = g.Select(k => new {
                                                ime = k.Korisnik!.Ime,
                                                korisnicko = k.Korisnik!.KorisnickoIme
                                            }).ToList() // Lista korisnika u toj sobi
                                        })
                                        .ToListAsync();

            return Ok(rezultat);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}