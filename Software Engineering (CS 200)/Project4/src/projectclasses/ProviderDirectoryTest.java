package projectclasses;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import org.junit.Before;
import org.junit.Test;
/**
 * Provider Directory JTest.

 * @author Zachary Michaels
 *
 */

public class ProviderDirectoryTest {

  ProviderDirectory pdCheck;
  
  @Before
  public void setUp() throws Exception {
    pdCheck = new ProviderDirectory();
  }

  @Test
  public void test1Zm() {
    //For valid input/serviceCode
    assertTrue(pdCheck.verifyCode(883948).equals("Aerobics session"));
  }

  @Test
  public void test2Zm() {
    //For invalid input/serviceCode
    assertTrue(pdCheck.verifyCode(555555).equals("INVALID CODE"));
  }

  @Test
  public void test3Js() {
    //For invalid fee
    assertEquals(0, pdCheck.getFee(76));
  }

}
