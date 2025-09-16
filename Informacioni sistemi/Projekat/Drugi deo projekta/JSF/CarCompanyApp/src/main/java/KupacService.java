import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public interface KupacService {

	public void createKupac(String ime, String prezime, String adresa, String stanje_racuna);
	public void deleteKupac(String bid);
	public void updateBankAccount(String aid, String stanje);
	public List<Kupac> spisakKupaca();
	
}
