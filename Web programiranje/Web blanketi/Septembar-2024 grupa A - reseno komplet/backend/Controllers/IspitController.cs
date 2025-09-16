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
    
    [HttpPost]
    [Route("/DodajStan")]
    public async Task<ActionResult> DodajStan([FromBody]Stan stan){
        try{
            await Context.Stanovi.AddAsync(stan);
            await Context.SaveChangesAsync();
            return Ok("Uspesno upisan stan");
        }
        catch(Exception e){
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajRacun/{mesec}/{voda}/{stanId}/{placen}")]
    public async Task<ActionResult> DodajRacun(int mesec, int voda, int stanId, string placen){
        try{
            var stan = await Context.Stanovi.FindAsync(stanId);
            var racun = new Racun{
                MesecIzdavanja = mesec,
                Voda = voda, 
                Struja = 150*stan.BrClanova,
                KomunalneUsluge = 100*stan.BrClanova,
                Placen = placen,
                Stan = stan
            };
            await Context.Racuni.AddAsync(racun);
            await Context.SaveChangesAsync();
            return Ok("Uspesno upisan racun");
        }
        catch(Exception e){
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/PodaciOStanu/{stanId}")]
    public async Task<ActionResult> PodaciOStanu(int stanId){
        try{
            var podaci = await Context.Stanovi.FindAsync(stanId);
            return Ok(podaci);
        }
        catch(Exception e){
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiSveID")]
    public async Task<ActionResult> VratiSveID(){
        try{
            var podaci = await Context.Stanovi.Select(p => new{
                ID=p.ID
            }).ToListAsync();
            return Ok(podaci);
        }
        catch(Exception e){
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiInfoOStanu/{stanId}")]
    public async Task<ActionResult> VratiInfoOStanu(int stanId){
        try{
            var stan = await Context.Stanovi.FindAsync(stanId);
            var podaci = await Context.Racuni.Include((p) => p.Stan).Where((p) => p.Stan!.ID == stanId).Select(p => new{
                ID = p.ID,
                Mesec = p.MesecIzdavanja,
                Voda = p.Voda,
                Struja = p.Struja,
                KomunalneUsluge = p.KomunalneUsluge,
                Placen = p.Placen 
            }).ToListAsync();
            return Ok(podaci);
        }
        catch(Exception e){
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiTroskove/{stanId}")]
    public async Task<ActionResult> VratiTroskove(int stanId){
        try{
            int ukupno = 0;
            var stan = await Context.Stanovi.FindAsync(stanId);
            var troskovi = await Context.Racuni.Include((p) => p.Stan).Where((p) => p.Stan!.ID == stanId && p.Placen == "Ne").Select(p => new{
                Voda = p.Voda,
                Struja = p.Struja,
                KomunalneUsluge = p.KomunalneUsluge
            }).ToListAsync();
            foreach (var item in troskovi)
            {
                 ukupno = item.Voda + item.Struja + item.KomunalneUsluge;
            }
            return Ok(ukupno);
        }
        catch(Exception e){
            return BadRequest(e.Message);
        }
    }
}
