package projectclasses;

/**
 * The Provider Directory Class. This allows the user to be able to print the directory
 * (Request Provider Directory), get the fee associated with a code, and verify a code's description

 * @author Zachary Michaels
 *
 */

public class ProviderDirectory {

  private String[] directory = {"Aerobics session", "Basic doctors visit", "Conditioning session", 
    "Cycling session", "Pilots Class", "Swimming session", "Visit with dietitian",
    "Weight lifting class"};
  private int[] serviceCodes = {883948, 100003, 100004, 100002, 100001, 100005, 598470, 100000};
  private int[] fees = {50, 100, 75, 35, 35, 60, 150, 40};
  private int size = 8;

 
  /**
   * Prints out the provider directory descriptions(alphabetic), codes, and fees to the terminal.

   * @author Zachary Michaels
   */
  public void requestDirectory() {
    System.out.println("Service Name | Service Code | Service Fee(USD):");
    
    for (int i = 0; i < size; i++) {

      System.out.println(directory[i] + " | " + serviceCodes[i] + " | " + fees[i]);
    }
  }
  
  /**
   * Gets the fee for a known valid code entered.

   * @author Zachary Michaels
   * @param code is used for fee finding
   * @return Fee corresponding to the entered service code as an int
   */
  public int getFee(int code) {
    for (int i = 0; i < size; i++) {
      if (serviceCodes[i] == code) {
        return fees[i];
      }
    }
    return 0;
  }

  /**
   * checks to see if code is valid. returns description if valid, else returns "INVALID CODE"

   * @author Zachary Michaels
   * @param code is entered that the user wants the description for
   * @return Description of code entered is returned as a string
   */
  
  public String verifyCode(int code) {
    for (int i = 0; i < size; i++) {
      if (serviceCodes[i] == code) {
        return directory[i];
      }
    }
    return "INVALID CODE";
  }

 
}