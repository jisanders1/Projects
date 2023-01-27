package projectclasses;

import java.util.ArrayList;
import java.util.Scanner;

//import lab7Package.ProviderDir;
/**
 * The provider controller allows the user to primarily BillChocAn as well as 
 * supporting validating and getting the name of a member.
 *
 * @author Zack Michaels
 * 
 */
public class ProviderController {
  private ArrayList<Integer> providerIds = new ArrayList<Integer>();
  //private ArrayList<boolean> validProviderIDs;
  private ArrayList<Integer> memberIds = new ArrayList<Integer>();
  private ArrayList<String> memberNames = new ArrayList<String>();
  //private ArrayList<boolean> validMemberIDs;
  private int sizeMember;
  private int sizeProvider; //LOOK BELOW AND CHANGE
  
  /**
   * Provider Controller constructor that stores up-to-date data of the system.
   *
   * @author Zachary Michaels
   * @param memberList and providerList are the ArrayLists passed that will be stored to 
   *        the Provider Controller that will be referenced later
   */

  public ProviderController(ArrayList<Member> memberList, ArrayList<Provider> providerList) { 
    //arraylist of member,provider records passed 
    sizeMember = memberList.size();
    sizeProvider = providerList.size();
    for (int i = 0; i < sizeMember; i++) {
      memberIds.add(memberList.get(i).getNumber());
    }
    for (int j = 0; j < sizeProvider; j++) {
      providerIds.add(providerList.get(j).getNumber());
    }
    for (int k = 0; k < sizeMember; k++) {
      memberNames.add(memberList.get(k).getName());
    }
  }
  
  /** 
  * Validates Member.
  *
  * @author Zachary Michaels
  * @param id that is being checked
  * @return Returns Valid Member if found, else Invalid Member as a string
  */
  public String validateMember(int id) {
    for (int i = 0; i < sizeMember; i++) {
      if (memberIds.get(i) == id) {
        return "Valid Member";
      }
    }
    return "Invalid Member";
  }

  /**
   * Returns member name.
   *
   * @author Zachary Michaels
   * @param id that is being used to return corresponding name
   * @return Returns the name corresponding to the entered ID as a string
   */
  public String getMemName(int id) {
    for (int i = 0; i < sizeMember; i++) {
      if (memberIds.get(i) == id) {
        return memberNames.get(i);
      }
    }
    return "";
  }

  /**
   * BillChocAn allows the provider to bill ChocAn by selecting the 
   * appropriate service code and using a valid member.

   * @author Zachary Michaels
   * @param dos Provider-entered Date of service, serviceCode used, member #,
   *     provider #, list of service records, and the provider object being used
   * @return Returns true if BillChocAn process was successful, else false if cancelled
   */
  public boolean billChocAn(String dos, int serviceCode, int memNum, int provNum, 
      ArrayList<ServiceRecords> servRec, Provider prov) { 
    //verify member through comparison of member records
    boolean invalidId = true;
    int input = memNum;
    Scanner reader = new Scanner(System.in);
    while (invalidId && input != 0) {
      String check = validateMember(input);
      if (check.equals("Valid Member")) {
        System.out.println("Valid Member");
        invalidId = false;
        break;
      }
      System.out.println("Invalid member ID. Please enter again. Enter 0 to quit.");
      input = reader.nextInt();
    }
    if (invalidId) {
      System.out.println("Quit Bill ChocAn");
      //reader.close();
      return false;
    } else {
      ProviderDirectory pd = new ProviderDirectory();
      pd.requestDirectory(); //displays directory
      System.out.println(pd.verifyCode(serviceCode));
      boolean invalidsC = true;
      int choice1 = 9;
      int updatedsC = serviceCode;
      while (invalidsC && updatedsC != 0) {
        if (!pd.verifyCode(updatedsC).equals("INVALID CODE")) { //FOR valid code
          invalidsC = false;
          System.out.println("VALID CODE");
          System.out.println("Description: " + pd.verifyCode(updatedsC));
          System.out.println("If description matches service provided, "
              + "enter 1. To enter another code press 2");
          choice1 = reader.nextInt();
          if (choice1 == 1) {
            break;
          }
          invalidsC = true;
        }
        System.out.println("Invalid code entered. "
            + "Please enter a new 6-digit code or enter 0 to exit");
        updatedsC = reader.nextInt();
      }
      if (invalidsC) {
        //reader.close();
        System.out.println("Quit Bill ChocAn");
        return false;
      }
      System.out.println("Enter comments up to 100 characters(optional)");
      String comments2 = reader.nextLine();
      comments2 = reader.nextLine();

      ServiceRecords newRecord = new ServiceRecords("11-23-2022 08:19:00", dos, provNum, 
          prov.getName(), input, 
          getMemName(input), updatedsC, pd.verifyCode(updatedsC), comments2);
      servRec.add(newRecord);
      prov.addServiceCode(updatedsC);
      prov.setConsultations(prov.getConsultations() + 1);
      prov.setTotalFee(prov.getTotalFee() + pd.getFee(updatedsC));
      
      System.out.println("--- Stored to Service Records ---");
      System.out.println();
      System.out.println("Provider Data For Personal Records");
      System.out.println("CurrD&T: 11-23-2022 08:19:00");
      System.out.println("DOS Provided:" + dos);
      //System.out.println("ProviderID Num:" + provNum); //validated before entering chocan
      System.out.println("Member ID Number: " + input);
      System.out.println("Service Code: " + updatedsC);
      //System.out.println("Service Description: " + pd.verifyCode(updatedSC));
      System.out.println("Fee: " + pd.getFee(updatedsC));
      //System.out.println("Comments: " + comments2);
      System.out.println();
      //reader.close(); //Check into closing readers -> dont need to close b/c we use system.in
      return true;
    }
  }
}