namespace Models;

public class Voz
{
    [Key]
    public int ID { get; set; }
    public uint MaxKapacitetPutnika { get; set; }
    public DateTime DatumProizvodnje { get; set; }
    public uint MaxBrzina { get; set; }
    public uint Tezina { get; set; }

    public List<Relacija>? Relacije { get; set; }
}
