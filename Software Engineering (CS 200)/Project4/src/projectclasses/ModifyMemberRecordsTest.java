package projectclasses;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import java.util.ArrayList;
import org.junit.Before;
import org.junit.Test;

/**
 * This class contains three test cases for the ModifyMemberRecords class.

 * @author Jaylon Sanders
 *
 */
public class ModifyMemberRecordsTest {

  ModifyMemberRecords modifyMemberController;

  @Before
  public void setUp() throws Exception {
    ArrayList<Member> memberRecords = new ArrayList<Member>();
    modifyMemberController = new ModifyMemberRecords(memberRecords);
  }

  @Test
  public void test1Js() {  //should test trying to modify a non-existent member
    assertTrue(modifyMemberController.modifyMemberRecords(123456789) == false);
  }

  @Test
  public void test2Js() { //should test adding a new member to the records
    Member newMember = new Member();
    newMember.setName("Jenna Johnson");
    newMember.setNumber(132435465);
    newMember.setAddress("123 Main St.");
    newMember.setCity("Hunstville");
    newMember.setState("AL");
    newMember.setZipCode(35649);
    newMember.setStatus(false);
    newMember.setEmail("jjohnson89@gmail.com");
    assertTrue(modifyMemberController.addNewMember(newMember) == true);
  }

  @Test
  public void test3Zm() {
    Member newMember = new Member();
    newMember.setName("Jenna Johnson");
    newMember.setNumber(132435465);
    newMember.setAddress("123 Main St.");
    newMember.setCity("Hunstville");
    newMember.setState("AL");
    newMember.setZipCode(35649);
    newMember.setStatus(false);
    newMember.setEmail("jjohnson89@gmail.com");
    modifyMemberController.addNewMember(newMember);
    Member newMember2 = new Member();
    newMember2.setName("Jenna Johnson");
    newMember2.setNumber(132435465);
    newMember2.setAddress("123 Main St.");
    newMember2.setCity("Hunstville");
    newMember2.setState("AL");
    newMember2.setZipCode(35649);
    newMember2.setStatus(false);
    newMember2.setEmail("jjohnson89@gmail.com");

    assertFalse(modifyMemberController.addNewMember(newMember2));
  }
}
