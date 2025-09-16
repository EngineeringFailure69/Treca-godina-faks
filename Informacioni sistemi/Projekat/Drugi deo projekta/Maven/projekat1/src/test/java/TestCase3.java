import org.junit.After;
import org.junit.AfterClass;
import org.junit.Before;
import org.junit.BeforeClass;
import org.junit.Test;
import static org.junit.Assert.*;

import java.util.List;

import javax.ejb.EJB;
import javax.persistence.EntityManager;
import javax.persistence.EntityManagerFactory;
import javax.persistence.Persistence;

//Testiramo da li je kupac sa id=1 izvrsio kupovinu u salonu automobila 
public class TestCase3 {
	
	private static KupovinaServiceImpl service;
	private static KupacServiceImpl service2;
	
	@BeforeClass
	   public static void initTest()
	   {
		   service=new KupovinaServiceImpl();
		   service2=new KupacServiceImpl();
		   System.out.println("BeforeClass: Uspesno inicijalizovan test");
	   }
		 
	  //Preduslov:Kupac mora da postoji u bazi 
	   @Before
	   public void testPrecondition()
	   {
		   String k1=service2.getKupacId("1");
	       assertNull(k1);
		   System.out.println("Before: Preduslov");

	   }
	   
	   //Provera da li se kupovina nalazi u bazi, ukoliko je vracena vednost razlicita od NULL, postoji kupovina
	   @Test
	   public void testAssertions() 
	   {
		   //Kupovina k1=new Kupovina();
		   String k1=service.getKupovina("1");
		   assertNotNull(k1);
		   System.out.println("Test:izvrsen");
	   }
}
