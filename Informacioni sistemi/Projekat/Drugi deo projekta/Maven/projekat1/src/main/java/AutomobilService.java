import javax.ejb.Remote;

public interface AutomobilService {

	public void createCar(String marka, String model, int godiste, float cena);
	public void deleteCar(int id);
	public void updatePrice(int id, float cena);
	public void listAllCars();
	public String getAutomobil(String model);
	public void delCar(String model);
	
}
