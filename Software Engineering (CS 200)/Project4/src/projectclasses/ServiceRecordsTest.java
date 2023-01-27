package projectclasses;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Before;
import org.junit.Test;

/**
 * JUnit test for getProviderNumber().
 * author Tyler Coleman
 */

public class ServiceRecordsTest {

  @Before
  public void setUp() throws Exception {
	
  }

  @Test
  public void test1Tc() {
    ServiceRecords sr = new ServiceRecords("11-23-2022 08:19:00", 
        "L", 123456789, "t", 1, "k", 4, "m", "l");
    assertTrue(sr.getProviderNumber() == 123456789);
  }
	
  @Test
  public void test2Tc() {
    ServiceRecords sr = new ServiceRecords("11-23-2022 08:19:00", 
        "L", 123456789, "t", 1, "k", 4, "m", "l");
    assertFalse(sr.getProviderNumber() == 145236789);

  }

}
