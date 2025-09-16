import java.sql.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import javax.persistence.TableGenerator;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;

@Entity
@Table(name = "automobil")
public class Automobil {
	
	@TableGenerator(name = "auto_gen", table = "id_gen", pkColumnName = "gen_name", valueColumnName = "id_value", allocationSize = 1, pkColumnValue = "auto_gen")
	@Id
	@GeneratedValue(strategy = GenerationType.TABLE, generator = "auto_gen")
    private int id;
	@Column(name = "marka")
	private String marka;
	@Column(name = "model")
	private String model;
	@Column(name = "godiste")
	private int godiste;
	@Column(name = "cena")
	private float cena;
	public Automobil() {
	}
	public Automobil(String marka, String model, int godiste, float cena) {
			//this.setId(id);
			this.setMarka(marka);
			this.setModel(model);
			this.setGodiste(godiste);
			this.setCena(cena);
	}
	public int getId() {
		return id;
	}
	/*public void setId(int id) {
		this.id = id;
	}*/
	public String getMarka() {	
		return marka;
	}
	public void setMarka(String marka) {
		this.marka = marka;
	}
	public String getModel() {
		return model;
	}
	public void setModel(String model) {
		this.model = model;
	}
	public int getGodiste() {
		return godiste;
	}
	public void setGodiste(int godiste) {
		this.godiste = godiste;
	}
	public float getCena() {
		return cena;
	}
	public void setCena(float cena) {
		this.cena = cena;
	}
}