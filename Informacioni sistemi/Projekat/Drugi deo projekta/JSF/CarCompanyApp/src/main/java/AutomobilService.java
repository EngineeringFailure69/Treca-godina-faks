import java.io.IOException;
import java.util.ArrayList;
import java.util.List;

public interface AutomobilService {

	public void createCar(String marka, String model, String godiste, String cena);
	public void deleteCar(String bid);
	public void updatePrice(String aid, String cena);
	public List<Automobil> spisakAutomobila();
	
}
