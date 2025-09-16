
import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;
import javax.persistence.TableGenerator;
@Entity
@Table(name = "kupac")
public class Kupac {
	
	@TableGenerator(name = "kupac_gen", table = "id_gen", pkColumnName = "gen_name", valueColumnName = "id_value", allocationSize = 1, pkColumnValue = "kupac_gen")
	@Id
	@GeneratedValue(strategy = GenerationType.TABLE, generator = "kupac_gen")
	private int id;
	@Column(name = "ime")
	private String ime;
	@Column(name = "prezime")
	private String prezime;
	@Column(name = "adresa")
	private String adresa;
	@Column(name = "stanje_racuna")
	private float stanje_racuna;
	public Kupac() {
	}
	public Kupac(String ime, String prezime, String adresa, float stanje_racuna) {
			//this.setId(id);
			this.setIme(ime);
			this.setPrezime(prezime);
			this.setAdresa(adresa);
			this.setStanje(stanje_racuna);
	}
		public int getId() {
			return id;
		}
		/*public void setId(int id) {
			this.id = id;
		}*/
		public String getIme() {
			return ime;
		}
		public void setIme(String ime) {
			this.ime = ime;
		}
		public String getPrezime() {
			return prezime;
		}
		public void setPrezime(String prezime) {
			this.prezime = prezime;
		}
		public String getAdresa() {
			return adresa;
		}
		public void setAdresa(String adresa) {
			this.adresa = adresa;
		}
		public float getStanje() {
			return stanje_racuna;
		}
		public void setStanje(float stanje_racuna) {
			this.stanje_racuna = stanje_racuna;
		}
}