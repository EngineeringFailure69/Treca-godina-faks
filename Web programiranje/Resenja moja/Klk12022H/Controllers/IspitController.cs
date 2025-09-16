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

    [HttpPost("DodajAlbum/{autorID}/{naziv}/{godinaIzdavanja}/{izdavackaKuca}")]
    public async Task<ActionResult> DodajAlbum(int autorID, [FromQuery]int[] numere, string naziv, DateTime godinaIzdavanja, string izdavackaKuca)
    {
        try
        {
            var autor = await Context.Autori.FindAsync(autorID);
            if(autor == null)
                return BadRequest("Autor ne postoji");
            var album = new Album
            {
                IzdavackaKuca = izdavackaKuca,
                Naziv = naziv,
                GodinaIzdavanja = godinaIzdavanja,
                Autor = autor,
                Numere = new List<Numera>()
            };
            foreach(int n in numere)
            {
                var num = await Context.Numere.FindAsync(n);
                if(num != null)
                    album.Numere.Add(num);
            }
            await Context.Albumi.AddAsync(album);
            await Context.SaveChangesAsync();
            return Ok("Album je upisan");
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    } 

    [HttpGet("PretragaAutora/{n}")]
    public async Task<ActionResult> PretragaAutora(int n)
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
