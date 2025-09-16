namespace Models;

public class Racun
{
    [Key]
    public int ID { get; set; }
    public int BrojRacuna { get; set; }
    public DateTime Otvoren { get; set; }
    public int Sredstva { get; set; }
    public int PodigaoDoSada { get; set; }

    public Banka? Banka { get; set; }
    public Klijent? Klijent { get; set; }
}
