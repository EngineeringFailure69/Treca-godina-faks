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
public class KupacServiceImpl implements KupacService  {
	@PersistenceContext(name = "CarCompanyApp")
	
    private EntityManager em;
    
    @Override
    public void createKupac(String ime, String prezime, String adresa, String stanje_racuna) {
    	
    	try{

    	    float ra=Float.parseFloat(stanje_racuna);

    	    Kupac z1 = new Kupac(ime,prezime,adresa,ra);
    	    em.persist(z1);
    	 
    	        } catch(Exception ex) {
    		
    		System.out.println("Greska prilikom rada sa bazom: \n" + ex.getMessage());
    		
    		}  
    }
    
    @Override
    public void deleteKupac(String bid) {
    	
    	try{
    		 
    	    int kid=Integer.parseInt(bid);
    	    Kupac k1 =em.find(Kupac.class, kid);
    	    em.remove(k1);
    	        } catch(Exception ex) {
    		
    		System.out.println("Greska prilikom rada sa bazom: \n" + ex.getMessage());
    		
    		}  
    }
    
    @Override
    public void updateBankAccount(String aid, String stanje) {
        
    	try{

    	    
    	    int kid=Integer.parseInt(aid);
    	    Kupac k1 =em.find(Kupac.class, kid);
    	    float nstanje=Float.parseFloat(stanje);
    	    k1.setStanje(nstanje);

    	    
    	       
    	        } catch(Exception ex) {
    		
    		System.out.println("Greska prilikom rada sa bazom: \n" + ex.getMessage());
    		
    		}  
     }
    
    @Override
    public List<Kupac> spisakKupaca() {

    	List<Kupac> sviKupci = em.createQuery("SELECT k FROM Kupac k", Kupac.class).getResultList();
    	return sviKupci;
    }
}
