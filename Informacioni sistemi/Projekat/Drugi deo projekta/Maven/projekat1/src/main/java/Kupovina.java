import java.sql.Date;

import javax.persistence.Column;
import javax.persistence.Entity;
import javax.persistence.GeneratedValue;
import javax.persistence.GenerationType;
import javax.persistence.Id;
import javax.persistence.Table;
import javax.persistence.TableGenerator;
@Entity
@Table(name = "kupovina")
public class Kupovina {
	
	@TableGenerator(name = "kupovina_gen", table = "id_gen", pkColumnName = "gen_name", valueColumnName = "id_value", allocationSize = 1, pkColumnValue = "kupovina_gen")
	@Id
	@GeneratedValue(strategy = GenerationType.TABLE, generator = "kupovina_gen")
	private int id;
	@Column(name = "idkupca")
	private int idkupca;
	@Column(name = "idauta")
	private int idauta;
	@Column(name = "datum_kupovine")
	private int datum_kupovine;
	public Kupovina() {
	}
	public Kupovina(int idkupca, int idauta , int datum_kupovine) {
			//this.setId(id);
			this.setIdKupca(idkupca);
			this.setIdAuta(idauta);
			this.setDatumKupovine(datum_kupovine);
	}
	public int getId() {
		return id;
	}
	/*public void setId(int id) {
		this.id = id;
	}*/
	public int getIdKupca() {
		return idkupca;
	}
	public void setIdKupca(int idkupca) {
		this.idkupca = idkupca;
	}
	public int getIdAuta() {
		return idauta;
	}
	public void setIdAuta(int idauta) {
		this.idauta = idauta;
	}
	public int getDatumKupovine() {
		return datum_kupovine;
	}
	public void setDatumKupovine(int datum_kupovine) {
		this.datum_kupovine = datum_kupovine;
	}
}
