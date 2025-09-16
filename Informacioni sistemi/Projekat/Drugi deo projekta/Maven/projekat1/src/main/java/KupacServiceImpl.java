import java.sql.Date;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;
import javax.ejb.Remote;
import javax.ejb.Stateless;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;
import javax.persistence.TypedQuery;

@Remote(KupacService.class)
@Stateless
public class KupacServiceImpl implements KupacService  {
    private EntityManager em;
    
    public KupacServiceImpl()
    {
    	   EntityManagerFactory emf = Persistence
                   .createEntityManagerFactory("CarCompanyPU");
           em = emf.createEntityManager();
    }
 
    //public static void main(String[] args) {
 
      //  EntityManagerFactory emf = Persistence
        //        .createEntityManagerFactory("CarCompanyPU");
        //em = emf.createEntityManager();
        
        /*createKupac("Lazar", "Trifunovic", "Adresa1", 50000f);
        createKupac("Milos", "Milosevic", "Adresa2", 40000f);
        createKupac("Milan", "Milanovic", "Adresa3", 30000f);
        createKupac("Pera", "Peric", "Adresa4", 20000f);*/
        //updateBankAccount(1, 55000f);
        //listAllBuyers();
        
    //}
    @Override
    public void createKupac(String ime, String prezime, String adresa, float stanje_racuna) {
        em.getTransaction().begin();
        Kupac emp = new Kupac(ime, prezime, adresa, stanje_racuna);
        em.persist(emp);
        em.getTransaction().commit();
    }
    
    @Override
    public void deleteKupac(int id) {
    	Kupac kupac = em.find(Kupac.class, id);
    	em.getTransaction().begin();
    	em.remove(kupac);
    	em.getTransaction().commit();
    }
    
    @Override
    public void updateBankAccount(int id, float stanje) {
        Kupac kupac = em.find(Kupac.class, id);
        em.getTransaction().begin();
        kupac.setStanje(stanje);
        em.getTransaction().commit();
     }
    
    @Override
    public void listAllBuyers()
    {
      
        TypedQuery<Kupac> query = em.createQuery("SELECT e FROM Kupac e", Kupac.class);
        List<Kupac> kupcisvi = query.getResultList();
    
       for (Kupac e1: kupcisvi) {
    	   System.out.println("ID:"+e1.getId() + " Ime: "+e1.getIme() + 
    			   " Prezime: "+e1.getPrezime() + " Adresa: " + e1.getAdresa() + " Racun: " + e1.getStanje());
       }
      
   }
    
    @Override
    public Kupac getKupac(int id) 
    {
    	
    	Kupac kupac = em.find(Kupac.class, id);
	    return kupac;
    	
    }
    
    @Override
    public String getKupacId(String id) 
    {
    	int f=0;
    	TypedQuery<Kupac> query = em.createQuery("SELECT e FROM Kupac e", Kupac.class);
        List<Kupac> kupci = query.getResultList();
    	int k1=Integer.parseInt(id);
        
        for (Kupac e1: kupci) {
           int temp=e1.getId();
     	   if(k1==temp) 
     	   {
     		  f=1;
     	   }
     	   else 
     	   {
     		   f=0;
     	   }
        }
        
        if(f==1)
        	return id;
        else
        	return null;
    	
    }
}
