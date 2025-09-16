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

//Testiramo da li se stanje na racunu kupca uspesno update-uje 
public class TestCase2 {

	private static KupacServiceImpl service;
	
	@BeforeClass
	   public static void initTest()
	   {
		   service=new KupacServiceImpl();
		   System.out.println("BeforeClass: Uspesno inicijalizovan test");
	   }
	   
	   //Azuriramo stanje na racunu
	   @Test
	   public void testAssertions() 
	   {
		   service.updateBankAccount(1, 55000f);
		   Kupac e2=service.getKupac(1);
	       assertEquals(55000f,e2.getStanje(),55000f);
		   System.out.println("Test:izvrsen");
		   //service.updateBankAccount(1, 50000f);
		   
	   }
	   
	   //Post-uslov:Vracamo prethodno stanje 
	   @After
	   public void testPostcondition()
	   {
		   service.updateBankAccount(1, 50000f);
		   Kupac e2=service.getKupac(1);
	       assertEquals(55000f,e2.getStanje(),50000f);
	       System.out.println("After: post-uslov");
	   }
	   
}
