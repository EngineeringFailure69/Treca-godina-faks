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
public class KupovinaServiceImpl implements KupovinaService {
	@PersistenceContext(name = "CarCompanyApp")
	
    private EntityManager em;

    @Override
    public void createKupovina(String idkupca, String idauta, String datum_kupovine) {
        
    	try{
    	    
    		int s1=Integer.parseInt(idauta);
    	    Automobil a1=em.find(Automobil.class, s1);
    	    int s2=Integer.parseInt(idkupca);
    	    Kupac k1=em.find(Kupac.class, s2);
    	    int s3=Integer.parseInt(datum_kupovine);
    	    
    	    if(k1.getStanje()<a1.getCena())
        	{
        		System.out.println("Nemoguce je kupiti zeljeni auto, nemate dovoljno novca na racunu");
        		return;
        	}
        	     
    	        Kupovina k = new Kupovina(s2,s1,s3);
    	        em.persist(k);
    	        float novoStanje=k1.getStanje()-a1.getCena();
    	        k1.setStanje(novoStanje);

    	}
    	        catch(Exception ex) {
    		
    		System.out.println("Greska prilikom rada sa bazom: \n" + ex.getMessage());
    		
    		}  
    }
    
    @Override
    public void deleteKupovina(String bid) {
    	
    	try{
    		
    		int s=Integer.parseInt(bid);
    	    Kupovina k1 =em.find(Kupovina.class, s);
    	    em.remove(k1);
    	        } catch(Exception ex) {
    		
    		System.out.println("Greska prilikom rada sa bazom: \n" + ex.getMessage());
    		
    		}  
    }
    
    @Override
    public void updateKupovina(String aid, String idkupca, String idauta, String datum_kupovine) {
        
    	try{
    		
    		int s1=Integer.parseInt(aid);
    		int s3=Integer.parseInt(idauta);
    	    int s2=Integer.parseInt(idkupca);
    	    int s4=Integer.parseInt(datum_kupovine);
    	    Kupovina k1 =em.find(Kupovina.class, s1);
    	    k1.setIdKupca(s2);
    	    k1.setIdAuta(s3);
    	    k1.setDatumKupovine(s4);

    	        } catch(Exception ex) {
    		
    		System.out.println("Greska prilikom rada sa bazom: \n" + ex.getMessage());
    		
    		}  
     }
    
    @Override
    public List<Kupovina> spisakKupovina() {

    	List<Kupovina> sveKupovine = em.createQuery("SELECT e FROM Kupovina e", Kupovina.class).getResultList();
    	return sveKupovine;
    }
}