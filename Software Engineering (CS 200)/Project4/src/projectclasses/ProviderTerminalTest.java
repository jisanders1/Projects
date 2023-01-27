package projectclasses;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import org.junit.Before;
import org.junit.Test;

/**JUnit file that includes a test case for the Provider Terminal Class.
*
* @author Zachary Michaels
*
*/
public class ProviderTerminalTest {
  ProviderTerminal providerTerminalTest;

  /**This method sets up and initializes the proper variables.
  *
  * @author Zachary Michaels
  *
  */
  @Before
  public void setUp() throws Exception {
    ArrayList<Provider> providerList = new ArrayList<Provider>();
    Provider newProvider = new Provider();
    newProvider.setNumber(123456789);
    providerList.add(newProvider);
    providerTerminalTest = new ProviderTerminal(providerList);
  }

  @Test
  public void test1Zm() {
    assertFalse(providerTerminalTest.verifyProvider(123456780));
  }

  @Test
  public void test2Zm() {
    assertTrue(providerTerminalTest.verifyProvider(123456789));
  }

}
