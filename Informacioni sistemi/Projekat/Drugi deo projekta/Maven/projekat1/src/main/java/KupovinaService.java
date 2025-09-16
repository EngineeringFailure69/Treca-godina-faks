import javax.ejb.Remote;

public interface KupovinaService {

	public void createKupovina(int idkupca, int idauta, int datum_kupovine);
	public void listAllSells();
	public String getKupovina(String id);
	
}
