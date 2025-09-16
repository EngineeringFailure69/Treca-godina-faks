using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Relacija
{
    [Key]
    public int ID {get; set;}
    public uint BrojPutinka {get; set;}
    public double CenaKarte {get; set;}
    public DateTime DatumSaobracanja {get; set;}
    public Voz? Voz {get; set;}
    public Grad? GradPolaska {get; set;}
    public Grad? GradDolaska {get; set;}
}