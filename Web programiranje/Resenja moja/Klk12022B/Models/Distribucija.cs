using System.ComponentModel.DataAnnotations.Schema;

namespace WebTemplate.Models;

public class Distribucija
{
    [Key]
    public int ID {get; set;}
    [MaxLength(50)]
    public required string Naziv {get; set;}
    [MaxLength(50)]
    public required string Grad {get; set;}
    [MaxLength(50)]
    [RegularExpression(@"^([\w-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)|(([\w-]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$")]
    public required string Email {get; set;}
    public uint BrojRadnika {get; set;}
    public List<Podrucje>? Podrucja {get; set;}
}