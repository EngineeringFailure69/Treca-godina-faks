namespace WebTemplate.Models;

public class Racun{
    [Key]
    public int ID { get; set; }
    public int MesecIzdavanja { get; set; }
    public int Voda {get; set;}
    public int Struja {get; set; }
    public int KomunalneUsluge { get; set; }
    [MaxLength(2)]
    public required string Placen { get; set; }
    public Stan? Stan {get; set;}
}