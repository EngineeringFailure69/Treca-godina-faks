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
    [Route("/DodajStudenta")]
    public async Task<ActionResult> DodajStudenta([FromBody]Student student)
    {
        try
        {
            await Context.Studenti.AddAsync(student);
            await Context.SaveChangesAsync();
            return Ok("Uspesno dodat student");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajIspit")]
    public async Task<ActionResult> DodajPredmet([FromBody]Ispit ispit)
    {
        try
        {
            await Context.Ispiti.AddAsync(ispit);
            await Context.SaveChangesAsync();
            return Ok("Uspesno dodat ispit");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajRok")]
    public async Task<ActionResult> DodajRok([FromBody]Rok rok)
    {
        try
        {
            await Context.Rokovi.AddAsync(rok);
            await Context.SaveChangesAsync();
            return Ok("Uspesno dodat rok");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpPost]
    [Route("/DodajPolozeniIspit/{indeks}/{idIspita}/{idRoka}/{ocena}")]
    public async Task<ActionResult> DodajPolozeniIspit(int indeks, int idIspita, int idRoka, int ocena)
    {
        try
        {
            var student = await Context.Studenti.Where(p=>p.Indeks == indeks).FirstOrDefaultAsync();
            if(student ==  null)
                return BadRequest("Ne postoji student sa ovim brojem indeksa");
            
            var ispit = await Context.Ispiti.FindAsync(idIspita);
            if(ispit == null)
                return BadRequest("Ne postoji ispit sa ovim ID");

            var rok = await Context.Rokovi.FindAsync(idRoka);
            if(rok == null)
                return BadRequest("Ne postoji rok sa ovim ID");

            var polozioRok = Context.Spojevi.Include(x=>x.Student)
                .Include(x=>x.Ispit)
                .Where(x=>x.Student!.ID==student.ID
                && x.Ispit!.ID==ispit.ID)
                .FirstOrDefault();

            if(polozioRok !=null)
            {
                return StatusCode(203,"Student je vec polozio taj ispit");
            }

            Spoj s = new Spoj
            {
                Student = student,
                Rok = rok, 
                Ispit = ispit,
                Ocena = ocena 
            };

            await Context.Spojevi.AddAsync(s);
            await Context.SaveChangesAsync();
            return Ok("Uspesno upisan polozen ispit");
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/PreuzmiIspite")]
    public async Task<ActionResult> PreuzmiIspite()
    {
        try
        {
            var ispiti = await Context.Ispiti.Select(p=>new{
                p.ID,
                p.Naziv
            }).ToListAsync();
            return Ok(ispiti);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/PreuzmiRokove")]
    public async Task<ActionResult> PreuzmiRokove()
    {
        try
        {
            var rokovi = await Context.Rokovi.Select(p=>new{
                p.ID,
                p.Naziv
            }).ToListAsync();
            return Ok(rokovi);
        }
        catch(Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/PreuzmiInformacijeOStudentima/{idIspita}")]
    public async Task<ActionResult> PreuzmiInformacijeOStudentima(int idIspita, [FromQuery] List<int> idRokova)
    {
        try
        {
            // Proveravamo da li ispit postoji
            var ispit = await Context.Ispiti.FindAsync(idIspita);
            if (ispit == null)
            {
                return BadRequest("Ne postoji ispit sa ovim ID.");
            }

            // Proveravamo da li su prosleđeni validni ispitni rokovi
            if (idRokova == null || !idRokova.Any())
            {
                return BadRequest("Morate odabrati barem jedan ispitni rok.");
            }

            // Prikupljamo informacije o studentima, predmetima, ispitnim rokovima i ocenama
            var informacije = await Context.Spojevi
                .Include(s => s.Student) // Uključujemo informacije o studentu
                .Include(s => s.Ispit) // Uključujemo informacije o ispitu
                .Include(s => s.Rok) // Uključujemo informacije o roku
                .Where(s => s.Ispit!.ID == idIspita && idRokova.Contains(s.Rok!.ID)) // Filtriramo po ispitima i rokovima
                .Select(s => new
                {
                    StudentIndeks = s.Student!.Indeks, // Indeks studenta
                    StudentIme = s.Student.Ime, // Ime studenta
                    StudentPrezime = s.Student.Prezime, // Prezime studenta
                    IspitNaziv = s.Ispit!.Naziv, // Naziv ispita
                    RokNaziv = s.Rok!.Naziv, // Naziv roka
                    Ocena = s.Ocena // Ocena koju je student dobio
                })
                .ToListAsync(); // Vraćamo listu rezultata

            // Ako nema podataka, vraćamo prazan odgovor
            if (!informacije.Any())
            {
                return NotFound("Nema podataka za odabrani ispit i ispitne rokove.");
            }

            // Vraćamo prikupljene informacije
            return Ok(informacije);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }

    [HttpGet]
    [Route("/VratiInformacijeOStudentu/{indeks}")]
    public async Task<ActionResult> VratiInformacijeOStudentu(int indeks)
    {
        try
        {
            // Pronalaženje studenta na osnovu broja indeksa
            var student = await Context.Studenti
                .Include(s => s.StudentPredmet) // Uključuje sve položenje ispite studenta
                .ThenInclude(sp => sp.Ispit) // Uključuje informacije o predmetu
                .Include(s => s.StudentPredmet)
                .ThenInclude(sp => sp.Rok) // Uključuje informacije o ispitnom roku
                .Where(s => s.Indeks == indeks)
                .FirstOrDefaultAsync();

            // Proveravamo da li student postoji
            if (student == null)
            {
                return NotFound("Student sa ovim brojem indeksa ne postoji.");
            }

            // Prikupljamo sve informacije o položenim ispitima
            var informacije = student.StudentPredmet.Select(sp => new
            {
                StudentIndeks = student.Indeks,
                StudentIme = student.Ime,
                StudentPrezime = student.Prezime,
                IspitNaziv = sp.Ispit.Naziv, // Naziv predmeta
                RokNaziv = sp.Rok.Naziv, // Naziv ispitnog roka
                Ocena = sp.Ocena // Ocena koju je student dobio
            }).ToList();

            // Ako nema položenih ispita, vraćamo prazan odgovor
            if (!informacije.Any())
            {
                return NotFound("Student nema položenih ispita.");
            }

            // Vraćamo prikupljene informacije
            return Ok(informacije);
        }
        catch (Exception e)
        {
            return BadRequest(e.Message);
        }
    }
}