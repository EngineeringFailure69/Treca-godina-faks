import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.PersistenceContext;
import javax.persistence.TypedQuery;


@Stateless
public class AutomobilServiceImpl implements AutomobilService {
	@PersistenceContext(name = "CarCompanyApp")
	
    private EntityManager em;
 
    @Override
    public void createCar(String marka, String model, String godiste, String cena) {
       
    	try{

    	    float c=Float.parseFloat(cena);
    	    int g=Integer.parseInt(godiste);

    	    Automobil a1 = new Automobil(marka,model,g,c);
    	    em.persist(a1);
    	 
    	        } catch(Exception ex) {
    		
    		System.out.println("Greska prilikom rada sa bazom: \n" + ex.getMessage());
    		
    		}  
    	
    }
    
    @Override
    public void deleteCar(String bid) {
    	try{
   		
    		int k=Integer.parseInt(bid);
    	    Automobil a1 =em.find(Automobil.class, k);
    	    em.remove(a1);
    	        } catch(Exception ex) {
    		
    		System.out.println("Greska prilikom rada sa bazom: \n" + ex.getMessage());
    		
    		}  
    }
    
    @Override
    public void updatePrice(String aid, String cena) {
    	try{

    		int k=Integer.parseInt(aid);
    		float c=Float.parseFloat(cena);
    	    Automobil k1 =em.find(Automobil.class, k);
    	    k1.setCena(c);
    	       
    	        } catch(Exception ex) {
    		
    		System.out.println("Greska prilikom rada sa bazom: \n" + ex.getMessage());
    		
    		}  
     }
    
    @Override
    public List<Automobil> spisakAutomobila() {

    	List<Automobil> sviAutomobili = em.createQuery("SELECT e FROM Automobil e", Automobil.class).getResultList();
    	return sviAutomobili;
    }
}