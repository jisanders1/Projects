package projectclasses;

import java.util.ArrayList;

/** 
 * This class handles the printing of the Member Report as specified in ChocAn document.

 * @author Quincy Gunnerson
*/

public class MemberReport {
  private ArrayList<Member> memberRecords;
  private ArrayList<ServiceRecords> serviceRecords;

  /**
   * This is a simple constructor for a class of Member Report.

   * @author Quincy Gunnerson
   * @param inptMemRecords This parameter is the input value.
   * @param inptServRecords This parameter is the input value.
   */
  public MemberReport(ArrayList<Member> inptMemRecords, ArrayList<ServiceRecords> inptServRecords) {
    memberRecords = inptMemRecords;
    serviceRecords = inptServRecords;
  }



  /** 
   * This method loops through the member records and for each member, prints their
   * information, including each service they received.

   * @author Quincy Gunnerson
  */
  public void printMemberReport() { 
    System.out.println("Member Reports:");
    for (int i = 0; i < memberRecords.size(); i++) { 
      System.out.println("Member Name:  " + memberRecords.get(i).getName());
      System.out.println("Member Number: " + memberRecords.get(i).getNumber());
      System.out.println("Member Address: " + memberRecords.get(i).getAddress());
      System.out.println("Member City: " + memberRecords.get(i).getCity());
      System.out.println("Member State: " + memberRecords.get(i).getState());
      System.out.println("Member Zip Code: " + memberRecords.get(i).getZipCode());
      System.out.println("Services:");
      for (int j = 0; j < serviceRecords.size(); j++) {
        if (serviceRecords.get(j).getMemberName() == memberRecords.get(i).getName()) {
          System.out.println("Service Name: " + serviceRecords.get(j).getServiceName());
          System.out.println("Service Date: " + serviceRecords.get(j).getServiceDate());
          System.out.println("Provider Name:  " + serviceRecords.get(j).getProviderName() + "\n");
        }
      }
    }
  }
}
