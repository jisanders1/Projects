package projectclasses;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import org.junit.Before;
import org.junit.Test;


/**
 * JUnit Test for getNumber() in Member class.
 *
 * @author Tyler Coleman
 *
 */
public class MemberTest {


  Member member;

  /**
   * JUnit Test for getNumber() in Member class.
   *
   * @author Tyler Coleman
   *
   */
  @Before
  public void setUp() throws Exception {
    member = new Member();
    member.setName("Ron");
    member.setNumber(100000000);
    member.setAddress("address");
    member.setCity("Gotham");
  }

  @Test
  public void test1Tc() { // For correct member number search
    assertEquals(100000000, member.getNumber());
  }

  @Test
  public void test2Tc() { //// For incorrect member number search
    assertFalse(member.getNumber() == 254586);
  }

  @Test
  public void test1Qg() { //testing getAddress
    assertTrue(member.getAddress().equals("address"));
  }

  @Test
  public void test2Qg() {
    assertFalse(member.getAddress().equals("adres"));
  }

  @Test
  public void test3Qg() { //testing getCity
    assertTrue(member.getCity().equals("Gotham"));
  }

  @Test
  public void test4Qg() {
    assertFalse(member.getCity().equals("Babylon"));
  }
}
