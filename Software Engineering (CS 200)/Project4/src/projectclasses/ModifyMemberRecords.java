package projectclasses;

import java.util.ArrayList;
import java.util.Scanner;

/**
 * This class is the controller for an Operator to manipulate member records.

 * @author Jaylon Sanders
 *
 */
public class ModifyMemberRecords {
  private ArrayList<Member> memberRecords;
  
  public ModifyMemberRecords() {
    
  }

  //Default Constructor
  public ModifyMemberRecords(ArrayList<Member> inputMemberRecords) {
    memberRecords = inputMemberRecords;
  }
  /**
   * This method adds a new member to the records if the member has not already been added.

   * @author Jaylon Sanders
   * @param memberToAdd The member that is being added to the records.
   * @return Returns true if adding was successful,returns false when the member is in the system.
   */
  public boolean addNewMember(Member memberToAdd) {
    Member searchMember = searchByNum(memberToAdd.getNumber());
    if (searchMember == null) {
      memberRecords.add(memberToAdd);
      return true;
    }
    return false;
  }

  /**
   * This method deletes a resigned member from the records given their member number.

   * @author Jaylon Sanders
   * @param memNum The member number of the member being deleted.
   * @return Returns true when the member was successfully deleted. 
   *     Returns false if the member is not in the system or if they did not resign.
   */
  public boolean deleteResignedMember(int memNum) {
    Member searchMember = searchByNum(memNum);
    if (searchMember != null) {
      if (searchMember.isResigned) {
        memberRecords.remove(searchMember);
        return true;
      } else {
        return false;
      }
    }
    return false;
  }

  /**
   * This method modifies a member given their member number.
   * It also allows the user to save or not save their changes

   * @author Jaylon Sanders
   * @param memNum The member number of the member being deleted.
   * @return Returns true if the member is successfully modified.
   *     Returns false if the member is not in the system. 
   */
  public boolean modifyMemberRecords(int memNum) {
    Member searchMember = searchByNum(memNum);
    if (searchMember != null) {
      //Storing Previous Information in case of errors or 
      //if the operator decides to not save the changes
      Member tempMember = new Member();
      tempMember.setName(searchMember.getName());
      tempMember.setNumber(searchMember.getNumber());
      tempMember.setAddress(searchMember.getAddress());
      tempMember.setCity(searchMember.getCity());
      tempMember.setState(searchMember.getState());
      tempMember.setZipCode(searchMember.getZipCode());
      tempMember.setName(searchMember.getName());
      tempMember.setEmail(searchMember.getEmail());
      tempMember.setStatus(searchMember.getStatus());
      
      Scanner input;
      while (true) {
        System.out.println("Fields for modication:");
        System.out.println("1 - For Member Name\n2 - For Member Number");
        System.out.println("3 - For Member Street Address\n4 - For Member City");
        System.out.println("5 - For Member State\n6 - For Member Zip Code");
        System.out.println("7 - For Member Email\n8 - For Resignation Status");
        System.out.println("9 - To Save and Exit");
        System.out.print("Enter which field you would like to modify: ");
        input = new Scanner(System.in);
        int inputNum;
        String inputString;
        inputString = input.nextLine();
        
        try {
          inputNum = Integer.parseInt(inputString);
        } catch (NumberFormatException e) {
          System.out.println("Input contains non-numeric characters. Please try again!");
          continue;
        }
        
        switch (inputNum) {
          case 1:
            while (true) {
              System.out.print("Enter name to update member name (up to 25 characters): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 25) {
                System.out.println("Input is longer than 25 characters. Please try again!"); 
              } else if (containsDigit(inputString)) {
                System.out.println("Input contains a number. Please try again!");
              } else if (containsSpecialCharacter(inputString)) {
                System.out.println("Input contains a special character. Please try again!");
              } else {
                break;
              }
            }
            searchMember.setName(inputString);
            System.out.println("Member name updated!");
            break;
          case 2:
            while (true) {
              System.out.print("Enter number to update member number (9 digits): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 9) {
                System.out.println("Input is longer than 9 digits. Please try again!");
              } else if (inputString.length() < 9) {
                System.out.println("Input is shorter than 9 digits. Please try again!");
              } else {
                try {
                  int stringValue = Integer.parseInt(inputString);
                  searchMember.setNumber(stringValue);
                  System.out.println("Member number updated!");
                  break;
                } catch (NumberFormatException e) {
                  System.out.println("Input contains non-numeric characters. Please try again!");
                }
              }
            }
            break;
          case 3:
            while (true) {
              System.out.print("Enter address to update member address (up to 25 characters):");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 25) {
                System.out.println("Input is longer than 25 characters. Please try again!"); 
              } else if (containsSpecialAddress(inputString)) {
                System.out.println("Input contains a special character. Please try again!");
              } else {
                break;
              }
            }
            searchMember.setAddress(inputString);
            System.out.println("Member street address updated!");
            break;
          case 4:
            while (true) {
              System.out.println("Enter city to update member city (14 characters)");
              System.out.print("First letter must be capitalized: ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 14) {
                System.out.println("Input is longer than 14 characters. Please try again!"); 
              } else if (containsDigit(inputString)) {
                System.out.println("Input contains a number. Please try again!");
              } else if (containsSpecialCharacter(inputString)) {
                System.out.println("Input contains a special character. Please try again!");
              } else if (!Character.isUpperCase(inputString.charAt(0))) {
                System.out.println("First letter is not capitalized. Please try again!");
              } else {
                break;
              }
            }
            searchMember.setCity(inputString);
            System.out.println("Member city updated!");
            break;
          case 5:
            while (true) {
              System.out.print("Enter state to update member state (2 letters): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 2) {
                System.out.println("Input is longer than 2 letters. Please try again!"); 
              } else if (inputString.length() < 2) {
                System.out.println("Input is shorter than 2 letters. Please try again!");
              } else if (containsDigit(inputString)) {
                System.out.println("Input contains a number. Please try again!");
              } else if (containsSpecialCharacter(inputString)) {
                System.out.println("Input contains a special character. Please try again!");
              } else {
                break;
              }
            }
            inputString.toUpperCase();
            searchMember.setState(inputString);
            System.out.println("Member state updated!");
            break;
          case 6:
            while (true) {
              System.out.print("Enter zip code to update member zip code (5 digits): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 5) {
                System.out.println("Input is longer than 5 digits. Please try again!");
              } else if (inputString.length() < 5) {
                System.out.println("Input is shorter than 5 digits. Please try again!");
              } else {
                try {
                  int stringValue = Integer.parseInt(inputString);
                  searchMember.setZipCode(stringValue);
                  System.out.println("Member zip code updated!");
                  break;
                } catch (NumberFormatException e) {
                  System.out.println("Input contains non-numeric characters. Please try again!");
                }
              }
            }
            break;
          case 7:
            while (true) {
              System.out.println("Enter email to update member email");
              System.out.print("Format - username@gmail.com: ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (containsEmailError(inputString)) {
                System.out.println("Input contains multiple '@' symbols. Please try again!"); 
              } else {
                break;
              }
            }
            searchMember.setEmail(inputString);
            System.out.println("Member email updated!");
            break;
          case 8:
            while (true) {
              System.out.print("Enter 0 or 1 (for not resigned or resigned, respectively): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 1) {
                System.out.println("Input is longer than 1 digit. Please try again!");
              } else if (inputString.charAt(0) != '0' && inputString.charAt(0) != '1') {
                System.out.println("Invalid input. Please try again!");
              } else {
                break;
              }
            }
            
            if (inputString.charAt(0) == '0') {
              searchMember.setStatus(false);
            } else {
              searchMember.setStatus(true);
            }
            System.out.println("Member resignation status updated!");
            break;  
          case 9:
            while (true) {
              System.out.println("Do you want to save your changes?");
              System.out.print("Enter yes or no : ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              inputString.toLowerCase();
              
              if (inputString.equals("yes")) {
                break;
              } else if (inputString.equals("no")) {
                searchMember.setName(tempMember.getName());
                searchMember.setNumber(tempMember.getNumber());
                searchMember.setAddress(tempMember.getAddress());
                searchMember.setCity(tempMember.getCity());
                searchMember.setState(tempMember.getState());
                searchMember.setZipCode(tempMember.getZipCode());
                searchMember.setName(tempMember.getName());
                searchMember.setEmail(tempMember.getEmail());
                searchMember.setStatus(tempMember.getStatus());
                System.out.println("Changes reverted!");
                break;
              } else {
                System.out.println("Invalid input. Please try again!");
              }
            }
            //input.close();
            return true;
          default:
            System.out.println("Invalid input. Please try again!");
            break;
        }
      }
    }
    return false;
  }

  /**
   * This method searches for a member in the memberRecords array list.

   * @author Jaylon Sanders
   * @param num The number of a member to search for.
   * @return Returns the member if found, returns null if member is not found.
   */
  private Member searchByNum(int num) {
    for (int i = 0; i < memberRecords.size(); i++) {
      if (num == memberRecords.get(i).getNumber()) {
        return memberRecords.get(i);
      }
    }
    return null;
  }

  /**
   * This method searches for an digit inside of the given string.

   * @author Jaylon Sanders
   * @param searchString The string that is being searched through.
   * @return Returns true if the string contains a digit. 
   *     Returns false if the string does not contain a digit.
   */
  public boolean containsDigit(String searchString) {
    for (int i = 0; i < searchString.length(); i++) {
      if (Character.isDigit(searchString.charAt(i))) {
        return true;
      }
    }
    return false;
  }
  
  /**
   * This method verifies if each char in a string is a letter.

   * @author Jaylon Sanders
   * @param searchString The string that is being searched through.
   * @return Returns true if a special character is found in the string (bar '.' and ' ').
   *     Returns false if there are no special characters.
   */
  public boolean containsSpecialCharacter(String searchString) {
    for (int i = 0; i < searchString.length(); i++) {
      if (!Character.isLetter(searchString.charAt(i))) {
        if (searchString.charAt(i) != '.' && searchString.charAt(i) != ' ') {
          return true;
        }
      }
    }
    return false;
  }
  
  /**
   * This method verifies if each char in a string is a letter (Not including numbers).

   * @author Jaylon Sanders
   * @param searchString The string that is being searched through.
   * @return Returns true if a special character is found in the string (bar '.' and ' ').
   *     Returns false if the string does not have any errors
   */
  public boolean containsSpecialAddress(String searchString) {
    for (int i = 0; i < searchString.length(); i++) {
      if (!Character.isLetter(searchString.charAt(i))) {
        if (!Character.isDigit(searchString.charAt(i))) {
          if (searchString.charAt(i) != '.' && searchString.charAt(i) != ' ') {
            return true;
          }
        }
      }
    }
    return false;
  }
  
  /**
   * This method verifies if there is exactly one '@' in a string
   * and returns true there are multiple '@' found, returns false otherwise.

   * @author Jaylon Sanders
   * @param searchString The string that is being searched through.
   * @return Returns true if there are multiple '@' found. 
   *     Returns false only one '@' is found.
   */
  public boolean containsEmailError(String searchString) {
    int numberOfSymbols = 0;
    for (int i = 0; i < searchString.length(); i++) {
      if (searchString.charAt(i) == '@') {
        numberOfSymbols++;
      }
    }
    
    if (numberOfSymbols == 1) {
      return false;
    } else {
      return true;
    }
  }
}