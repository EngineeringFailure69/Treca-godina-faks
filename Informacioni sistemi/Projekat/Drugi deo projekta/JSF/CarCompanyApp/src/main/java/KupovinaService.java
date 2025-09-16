import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public interface KupovinaService {

	public void createKupovina(String idkupca, String idauta, String datum_kupovine);
	public void deleteKupovina(String bid);
	public void updateKupovina(String aid, String idkupca, String idauta, String datum_kupovine);
	public List<Kupovina> spisakKupovina();
	
}
