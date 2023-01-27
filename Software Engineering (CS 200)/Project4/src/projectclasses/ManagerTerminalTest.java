package projectclasses;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.io.ByteArrayInputStream;
import java.io.InputStream;
import java.util.ArrayList;
import org.junit.Before;
import org.junit.Test;


/**
 * This JUnit Test tests verifyManager() method in the ManagerTerminal class(SZ),
 *   getManagerNumber() in the ManagerClass(SZ), and the .
 *
 * @author Sandy-Zheng
 *
 */
public class ManagerTerminalTest {
  ManagerTerminal managerTerminalTest;
  Manager manager;
  
  /**
  * This JUnit Test tests verifyManager() method in the ManagerTerminal class(SZ),
  *   getManagerNumber() in the ManagerClass(SZ), and the .
  *
  * @author Sandy-Zheng
  *
   */
  @Before
  public void setUp() throws Exception {
    ArrayList<Member> memberRecords = new ArrayList<Member>();
    ArrayList<Provider> providerRecords = new ArrayList<Provider>();
    ArrayList<ServiceRecords> serviceRecords = new ArrayList<ServiceRecords>();
    managerTerminalTest = new ManagerTerminal(memberRecords, providerRecords, serviceRecords);

    manager = new Manager(400001);
    manager.setManagerNumber(400001);
  }

  @Test
  public void test1Sz() {  // tests verifyManager() in the ManagerTerminal class with a Valid code
    InputStream stdin = System.in;
    System.setIn(new ByteArrayInputStream("400000".getBytes()));
    assertTrue(managerTerminalTest.verifyManager());
  }

  @Test
  public void test2Sz() { // tests verifyManager() in the ManagerTerminal class with an Invalid code
    InputStream stdin = System.in;
    System.setIn(new ByteArrayInputStream("123456".getBytes()));
    assertFalse(managerTerminalTest.verifyManager());
  }

  @Test
  public void test3Sz() { //tests getManagerNumber() in the Manager Class 
    //                      with a existing valid Manager Number
    assertEquals(400001, manager.getManagerNumber());
  }

  @Test
  public void test4Sz() { //tests getManagerNumber() in the Manager Class
    //                      with an invalid Manager Number
    assertFalse(manager.getManagerNumber() == 123456);
  }
  
  @Test
  public void test5Cq() { //tests getManagerNumber() in the Manager Class 
    //                      with a existing valid Manager Number
    manager = new Manager(400004);
    manager.setManagerNumber(400004);
    assertEquals(400004, manager.getManagerNumber());
  }

  @Test
  public void test6Cq() { //tests getManagerNumber() in the Manager Class
    //                      with an invalid Manager Number
    assertFalse(manager.getManagerNumber() == 654321);
  }

}
