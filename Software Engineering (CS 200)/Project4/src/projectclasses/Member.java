package projectclasses;

/**
 * This class is for the members and allows setting and getting 
 * of member information.

 * @author Quincy Gunnerson
 */

public class Member {
  private
      String memberName;
  int memberNumber;
  String memberAddress;
  String memberCity;
  String memberState;
  int memberZipCode;
  String memberEmail;
  boolean isResigned;
  
  public  

      void setName(String inputName) {
    memberName = inputName;
  }

  void setNumber(int inputNumber) {
    memberNumber = inputNumber;
  }

  /**
 * This method takes in an input member address and sets the class's variable
 * equal to the input variable. All the other set functions follow the same 
 * straightforward structure.

 * @author Quincy Gunnerson
 * @param inputMemberAddress this is the input values.
 * 
 */

  void setAddress(String inputMemberAddress) {
    memberAddress = inputMemberAddress;
  }

  void setCity(String inputMemberCity) {
    memberCity = inputMemberCity;
  }

  void setState(String inputMemberState) {
    memberState = inputMemberState;
  }

  void setZipCode(int inputZipCode) {
    memberZipCode = inputZipCode;
  }

  void setEmail(String inputMemberEmail) {
    memberEmail = inputMemberEmail;
  }

  void setStatus(boolean inputIsResigned) {
    isResigned = inputIsResigned;
  }


  /**
   * This method, as well as all other get methods, simply returns the 
   * requested variable.

   * @author Quincy Gunnerson
   * @return Simply Returns the requested variable.
   */
  
  String getName() {
    return memberName;
  }

  int getNumber() {
    return memberNumber;
  }

  String getAddress() {
    return memberAddress;
  }

  String getCity() {
    return memberCity;
  }

  String getState() {
    return memberState;
  }

  int getZipCode() {
    return memberZipCode;
  }

  String getEmail() {
    return memberEmail;
  }

  boolean getStatus() {
    return isResigned;
  }
        
}
