import javax.ejb.Remote;

public interface KupacService {

	public void createKupac(String ime, String prezime, String adresa, float stanje_racuna);
	public void deleteKupac(int id);
	public void updateBankAccount(int id, float stanje);
	public void listAllBuyers();
	public Kupac getKupac(int id);
	public String getKupacId(String id);
	
}
