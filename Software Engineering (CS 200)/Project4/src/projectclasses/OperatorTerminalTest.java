package projectclasses;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import org.junit.Before;
import org.junit.Test;



/**JUnit file that includes a test case for verifyOperator function for the OperatorTerminal class.
 *
 * @author Cindy Qiu
 *
 */
public class OperatorTerminalTest {
  OperatorTerminal opTest;
  Provider prov;
  ArrayList<Integer> serviceCodes;

  /**Setting up new member and provider records array list that will be used for test cases.
  *
  * @author Cindy Qiu
  *
  */
  
  @Before
  public void setUp() throws Exception {
    ArrayList<Member> memberRecords = new ArrayList<Member>();
    ArrayList<Provider> providerRecords = new ArrayList<Provider>();
    opTest = new OperatorTerminal(memberRecords, providerRecords);
    prov = new Provider();

    prov.setNumber(100000000);
  }

  @Test
  public void test1Cq() { //tests verifyOperator in OperatorTerminal class with invalid code
    assertFalse(opTest.verifyOperator(000000));
  }

  @Test
  public void test2Cq() { //tests verifyOperator in OperatorTerminal class with valid code
    assertTrue(opTest.verifyOperator(300000));
  }
  
  @Test
  public void test3Cq() { //tests prov.getNumber in Provider class with valid code
    assertEquals(100000000, prov.getNumber());
  }
  
  @Test
  public void test4Cq() { //tests prov.getNumber in Provider class with invalid code
    assertFalse(prov.getNumber() == 000000000);
  }
 
  @Test
  public void test5Sz() { //tests verifyOperator in OperatorTerminal class with invalid code
    assertFalse(opTest.verifyOperator(100000));
  }

  @Test
  public void test6Sz() { //tests verifyOperator in OperatorTerminal class with valid code
    assertTrue(opTest.verifyOperator(300001));
  }
  
  
}
