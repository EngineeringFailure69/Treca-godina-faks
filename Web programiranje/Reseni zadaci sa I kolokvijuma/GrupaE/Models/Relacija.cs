namespace Models;

public class Relacija
{
    [Key]
    public int ID { get; set; }
    public uint BrojPutnika { get; set; }
    public uint CenaKarte { get; set; }
    public DateTime Datum { get; set; }

    public Grad? GradPolaska { get; set; }
    public Grad? GradDolaska { get; set; }
    public Voz? Voz { get; set; }
}
