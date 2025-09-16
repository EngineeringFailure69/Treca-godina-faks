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

@Remote(KupovinaService.class)
@Stateless
public class KupovinaServiceImpl implements KupovinaService {
    private static EntityManager em;
    
    public KupovinaServiceImpl()
    {
    	   EntityManagerFactory emf = Persistence
                   .createEntityManagerFactory("CarCompanyPU");
           em = emf.createEntityManager();
    }
 
    //public static void main(String[] args) {
 
    	//KupovinaServiceImpl k=new KupovinaServiceImpl();
    	//k.createKupovina(1, 1, 2023);
     //   EntityManagerFactory emf = Persistence
       //         .createEntityManagerFactory("CarCompanyPU");
        //em = emf.createEntityManager();
        
        //createKupovina(1, 1, 2023);
        //listAllSells();
        
    //}
    @Override
    public void createKupovina(int idkupca, int idauta, int datum_kupovine) {
        
    	Automobil automobil = em.find(Automobil.class, idauta);
    	Kupac kupac=em.find(Kupac.class, idkupca);
    	if(kupac.getStanje()<automobil.getCena())
    	{
    		System.out.println("Nemoguce je kupiti zeljeni auto, nemate dovoljno novca na racunu");
    		return;
    	}
    	
    	em.getTransaction().begin();
        Kupovina emp = new Kupovina(idkupca, idauta, datum_kupovine);
        em.persist(emp);
        em.getTransaction().commit();
        float novo_stanje=kupac.getStanje()-automobil.getCena();
        em.getTransaction().begin();
        kupac.setStanje(novo_stanje);
        em.getTransaction().commit();
        System.out.println("Kupili ste auto, vase stanje na racunu je azurirano i iznosi: ");
        System.out.print(novo_stanje);
    }
    
    @Override
    public void listAllSells()
    {
      
        TypedQuery<Kupovina> query = em.createQuery("SELECT e FROM Kupovina e", Kupovina.class);
        List<Kupovina> kupovinasve = query.getResultList();
    
       for (Kupovina e1: kupovinasve) {
    	   System.out.println("ID:"+e1.getId() + " Id kupca: "+e1.getIdKupca() + 
    			   " Id auta: "+e1.getIdAuta() + " Datum kupovine: " + e1.getDatumKupovine());
       }
      
   }
    
    @Override
    public String getKupovina(String id) 
    {
    	int f=0;
    	TypedQuery<Kupovina> query = em.createQuery("SELECT e FROM Kupovina e", Kupovina.class);
        List<Kupovina> kupovine = query.getResultList();
        //Kupovina kupovina=new Kupovina();
        int k2=Integer.parseInt(id);
        
        for (Kupovina k1: kupovine) {
        	 int tempk=k1.getIdKupca();
     	   if(tempk==k2) 
     	   {
     		   f=1;
     		  //kupovina=k1;
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