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

    [HttpPost("DodajNumeru")]
    public async Task<ActionResult> DodajNumeru([FromBody]Numera numera)
    {
        try
        {
            await Context.Numere.AddAsync(numera);
            await Context.SaveChangesAsync();
            return Ok($"Numera sa ID: {numera.ID} je dodata.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodajAutora")]
    public async Task<ActionResult> DodajAutora([FromBody]Autor autor)
    {
        try
        {
            autor.PrviAlbum = null;
            await Context.Autori.AddAsync(autor);
            await Context.SaveChangesAsync();
            return Ok($"Autor sa ID: {autor.ID} je dodat.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost("DodavanjeAlbuma/{autorID}")]
    public async Task<ActionResult> DodavanjeAlbuma(int autorID, [FromQuery]int[] numere, [FromBody]Album podaci)
    {
        try
        {
            var autor = await Context.Autori.FindAsync(autorID);

            if (autor == null)
            {
                return BadRequest("Autor nije pronadjen!");
            }

            var album = new Album
            {
                IzdavackaKuca = podaci.IzdavackaKuca,
                Naziv = podaci.Naziv,
                GodinaIzdavanja = podaci.GodinaIzdavanja,
                Autor = autor,
                Numere = new List<Numera>()
            };

            foreach (int n in numere)
            {
                var num = await Context.Numere.FindAsync(n);

                if (num != null)
                {
                    album.Numere.Add(num);
                }
            }

            await Context.Albumi.AddAsync(album);

            var autorIzmenjeni = await Context.Autori.Include(p => p.Albumi).Where(p => p.ID == autorID).FirstOrDefaultAsync();
            
            if (autorIzmenjeni != null)
            {
                var najstarijiAlbum = autorIzmenjeni.Albumi!.OrderBy(p => p.GodinaIzdavanja).FirstOrDefault();
                
                if (najstarijiAlbum != null)
                {
                    autorIzmenjeni.PrviAlbum = najstarijiAlbum.GodinaIzdavanja;
                }
                else
                {
                    autorIzmenjeni.PrviAlbum = null;
                }
            }

            await Context.SaveChangesAsync();

            return Ok($"Album sa ID: {album.ID} je upisan.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet("Pretraga/{n}")]
    public async Task<ActionResult> Pretraga(int n)
    {
        try
        {
            var autori = await Context.Autori
                .Where(p => p.PrviAlbum.HasValue && (DateTime.Now.Year - p.PrviAlbum.Value.Year) > n)
                .ToListAsync();

            return Ok(autori);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpDelete("BrisanjeAlbuma/{albumID}")]
    public async Task<ActionResult> BrisanjeAlbuma(int albumID)
    {
        try
        {
            var album = await Context.Albumi.Include(p => p.Numere).FirstOrDefaultAsync(p => p.ID == albumID);

            var autorID = Context.Albumi
                .Include(p => p.Autor)
                .Where(p => p.ID == albumID)
                .FirstOrDefault()?
                .Autor?.ID;

            if (album == null)
            {
                return BadRequest("Album nije pronadjen!");
            }

            Context.Albumi.Remove(album);
            await Context.SaveChangesAsync();

            var autorIzmenjeni = await Context.Autori.Include(p => p.Albumi).Where(p => p.ID == autorID).FirstOrDefaultAsync();
            
            if (autorIzmenjeni != null)
            {
                var najstarijiAlbum = autorIzmenjeni.Albumi!.OrderBy(p => p.GodinaIzdavanja).FirstOrDefault();
                
                if (najstarijiAlbum != null)
                {
                    autorIzmenjeni.PrviAlbum = najstarijiAlbum.GodinaIzdavanja;
                }
                else
                {
                    autorIzmenjeni.PrviAlbum = null;
                }
            }

            await Context.SaveChangesAsync();
            return Ok($"Album sa ID-jem: {albumID} je izbrisan.");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}
