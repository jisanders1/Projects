package projectclasses;

import java.util.ArrayList;
import java.util.Scanner;

/**This class's will allow the operator to perform their jobs of:
 *adding new members and providers,
 *deleting resigned members and providers, and
 *modifying member and provider records.
 *
 * @author Cindy Qiu
 *
 */

public class OperatorTerminal {
  private int[] operatorId = {300000, 300001, 300002}; 
  private ArrayList<Member> memberRecords;
  private ArrayList<Provider> providerRecords;
  private ArrayList<Operator> operators;


 
  /**
   * This is the default constructor for the operatorTerminal.
   *
   * @author Cindy Qiu
   * @param memberRecords is the ArrayList of member records in the system. 
   *     providerRecords is the ArrayList of provider records in the system.
   */
  
  public OperatorTerminal(ArrayList<Member> memberRecords, 
      ArrayList<Provider> providerRecords) {
    this.memberRecords = memberRecords;
    this.providerRecords = providerRecords;
  }

  /**
   * This is the function verifies the operator.
   *
   * @author Cindy Qiu
   * @param id is ID of the operator that needs to be verified.
   * @return True if the operator ID is found in the array of operator IDs.
   *     False is the operator ID is not found in the array.
   */
  public boolean verifyOperator(int id) {
    boolean isValid = false;
    for (int i = 0; i < operatorId.length; i++) {
      if (id == operatorId[i]) {
        isValid = true;
        break;
      }
    }

    return isValid;
  }
  //hello
	

  /**
   * This method will display the operator terminal and prompt the operator for various actions. 
   * Actions include add, delete, modify member and provider records

   * @author Cindy Qiu
   * @param memberRecords is the ArrayList of member records in the system. 
   *        providerRecords is the ArrayList of provider records in the system.
   */
  
  public void display(ArrayList<Member> memberRecords, ArrayList<Provider> providerRecords) {
    String choice;
    String actor1;
 
    //for new member 
    int memberNum;
    String memName;
    String memberAddress;
    String memberCity;
    String memberState;
    String memberEmail;
    String inputString;
    
    String providerName;
    String providerAddress;
    String providerCity;
    String providerState;
    String providerEmail;
    int providerNum;
   
    
    ModifyMemberRecords member = new ModifyMemberRecords(memberRecords);
    ModifyProviderRecords provider = new ModifyProviderRecords(providerRecords);
    Member newMember; 
    Provider newProvider;

    System.out.println("Do you want to perform an action on a member or provider?\n "
        + "Enter 'member', 'provider', or 'exit' to go back to main menu.");
    Scanner scan = new Scanner(System.in);
    actor1 = scan.nextLine();

    while (!actor1.equals("exit")) {
      switch (actor1) {
        case "member": 
          System.out.println("Would you like to add, "
              + "delete a member, modify a member's records, "
              + "or "
              + "go back to actions? Enter "
              + "'add', 'delete', 'modify', or 'back'.");
          scan = new Scanner(System.in);
          choice = scan.nextLine();


          while (!choice.equals("back")) {
            switch (choice) {

              case "add":
                newMember = new Member();
                newMember.setStatus(false);
                while (true) {
                  System.out.println("Enter member number:");
                  scan = new Scanner(System.in);
                  inputString = scan.nextLine();
                  if (inputString.length() > 9) {
                    System.out.println("Input is longer than 9 digits. Please try again!");
                  } else if (inputString.length() < 9) {
                    System.out.println("Input is shorter than 9 digits. Please try again!");
                  } else {
                    try {
                      int stringValue = Integer.parseInt(inputString);
                      newMember.setNumber(stringValue);

                      break;
                    } catch (NumberFormatException e) {
                      System.out.println("Input contains non-numeric character. Please try again!");
                    }
                  }

                }


                System.out.println("Enter member name:");
                scan = new Scanner(System.in);
                //scan.close();
                memName = scan.nextLine();
                //Perform error checks
                while (member.containsDigit(memName) || member.containsSpecialCharacter(memName)) {
                  System.out.println("Invalid. Input contains special "
                      + "characters or number. Please only "
                      + "enter letters.");
                  scan = new Scanner(System.in);
                  memName = scan.nextLine();
                }

                newMember.setName(memName);
                //scan.close();


                System.out.println("Enter member street address:");
                scan = new Scanner(System.in);
                memberAddress = scan.nextLine();
                //Perform error checks
                while (member.containsSpecialAddress(memberAddress)) {
                  System.out.println("Invalid. Input should not contains special characters.");
                  scan = new Scanner(System.in);
                  memberAddress = scan.nextLine();
                }
                newMember.setAddress(memberAddress);


                System.out.println("Enter member city:");
                scan = new Scanner(System.in);
                memberCity = scan.nextLine();

                while (member.containsDigit(memberCity) 
                      || member.containsSpecialCharacter(memberCity)) {
                  System.out.println("Invalid. Input contains special characters or number."
                         + "Please only enter letters.");
                  scan = new Scanner(System.in);
                  memberCity = scan.nextLine();
                }
                newMember.setCity(memberCity);
                System.out.println("Enter member state:");
                scan = new Scanner(System.in);
                memberState = scan.nextLine();

                while (member.containsDigit(memberState) 
                        || member.containsSpecialCharacter(memberState) 
                        || memberState.length() != 2) {
                  System.out.println("Invalid. Input contains special characters or number."
                          + " Please only enter 2 letters.");
                  scan = new Scanner(System.in);
                  memberState = scan.nextLine();
                }
                newMember.setState(memberState);
                //scan.close();
                while (true) {
                  System.out.println("Enter member ZIP Code:");
                  scan = new Scanner(System.in);
                  inputString = scan.nextLine();
                  if (inputString.length() > 5) {
                    System.out.println("Input is longer than 5 digits. Please try again!");
                  } else if (inputString.length() < 5) {
                    System.out.println("Input is shorter than 5 digits. Please try again!");
                  } else {
                    try {
                      int stringValue = Integer.parseInt(inputString);
                      newMember.setZipCode(stringValue);
                      System.out.println("Member ZIP Code updated!");
                      break;
                    } catch (NumberFormatException e) {
                      System.out.println("Input contains non-numeric character. Please try again!");
                    }
                  }
                }



                System.out.println("Enter member Email:");
                scan = new Scanner(System.in);
                memberEmail = scan.nextLine();
                while (member.containsEmailError(memberEmail)) {
                  System.out.println("Invalid. Make sure to only include 1 '@' for email address.");
                  scan = new Scanner(System.in);
                  memberEmail = scan.nextLine();
                }
                newMember.setEmail(memberEmail);

                //boolean - true if successful, false if they exist in records already
                if (member.addNewMember(newMember)) {
                  System.out.println("Member has been successfully added.\n");
                } else {
                  System.out.println("Member could not be added since member "
                      + "number already exists.");
                }
                System.out.println("Would you like to add, delete a member, modify a member's "
                    + "records, or go back to actions? "
                    + "Enter 'add', 'delete', 'modify', or 'back'.");

                scan = new Scanner(System.in);
                choice = scan.nextLine();
                break;

              case "delete":
                System.out.println("Enter member number:");
                providerNum = scan.nextInt();

                if (member.deleteResignedMember(providerNum)) {
                  System.out.println("Member has been successfully deleted.\n");
                } else {
                  System.out.println("Member could not be deleted since member"
                      + "does not exists or member is not marked as resigned.");
                }
                System.out.println("Would you like to add, delete a member, modify a "
                      + "member's records, or go back to actions? Enter 'add', "
                      + "'delete', 'modify', or 'back'.");
                scan = new Scanner(System.in);
                choice = scan.nextLine();
                //scan.close();
                break;
              case "modify":
                //call modify
                //boolean - true if member was successfully found and false if not in system
                System.out.println("Enter member number:");
                scan = new Scanner(System.in);
                memberNum = scan.nextInt();
                //scan.close();

                if (member.modifyMemberRecords(memberNum)) {
                  System.out.println("Member records has been successfully modified.\n");
                } else {
                  System.out.println("Member records could not be modified since member "
                       + "does not exists.");
                }
                System.out.println("Would you like to add, delete a member, modify a member"
                    + "'s records, or go back to actions?"
                    + " Enter 'add', 'delete', 'modify', or 'back'.");
                scan = new Scanner(System.in);
                choice = scan.next();
                break;
              case "back": 
                choice = "back";
                break;
              default: //print something to let the operator known that their choice was invalid
                System.out.println("Invalid Input.\n");
                System.out.println("Would you like to add, delete a member, modify a "
                    + "member's records, or go back to actions? Enter 'add', "
                    + "'delete', 'modify', or 'back'.");
                scan = new Scanner(System.in);
                choice = scan.nextLine();
                //scan.close();
                break;
            }


          }
          break;

        case "provider": 
          System.out.println("Would you like to add, delete a provider, "
              + "modify a provider's records, or go back to actions? "
              + "Enter 'add', 'delete', 'modify', or 'back'.");
          scan = new Scanner(System.in);
          choice = scan.nextLine();
          while (!choice.equals("back")) {
            switch (choice) {
              case "add":
                newProvider = new Provider();
                newProvider.setStatus(false);

                while (true) {
                  System.out.println("Enter provider number:");
                  scan = new Scanner(System.in);
                  inputString = scan.nextLine();
                  if (inputString.length() > 9) {
                    System.out.println("Input is longer than 9 digits. Please try again!");
                  } else if (inputString.length() < 9) {
                    System.out.println("Input is shorter than 9 digits. Please try again!");
                  } else {
                    try {
                      int stringValue = Integer.parseInt(inputString);
                      newProvider.setNumber(stringValue);
                      break;
                    } catch (NumberFormatException e) {
                      System.out.println("Input contains non-numeric character. Please try again!");
                    }
                  }

                }
                System.out.println("Enter provider name:");
                scan = new Scanner(System.in);
                providerName = scan.nextLine();
                while (provider.containsDigit(providerName) 
                  || provider.containsSpecialCharacter(providerName)) {
                  System.out.println("Invalid. Input contains special "
                      + "characters or number. Please only enter letters.");
                  scan = new Scanner(System.in);
                  providerName = scan.nextLine();
                }

                newProvider.setName(providerName);

                System.out.println("Enter provider street address:");
                scan = new Scanner(System.in);
                providerAddress = scan.nextLine();

                while (provider.containsSpecialAddress(providerAddress)) {
                  System.out.println("Invalid Input. Retry");
                  scan = new Scanner(System.in);
                  providerAddress = scan.nextLine();
                }
                newProvider.setAddress(providerAddress);


                System.out.println("Enter provider city:");
                scan = new Scanner(System.in);
                providerCity = scan.nextLine();

                while  (provider.containsDigit(providerCity) 
                      || provider.containsSpecialAddress(providerCity)) {
                  System.out.println("Invalid. Input contains special "
                      + "characters or number. Please only enter letters.");
                  scan = new Scanner(System.in);
                  providerCity = scan.nextLine();
                }
                newProvider.setCity(providerCity);


                System.out.println("Enter provider state:");
                scan = new Scanner(System.in);
                providerState = scan.nextLine();

                while (provider.containsDigit(providerState) 
                    || provider.containsSpecialAddress(providerState) 
                    || providerState.length() != 2) {
                  System.out.println("Invalid. Input contains special characters or number. "
                      + "Please only enter 2 letters.");
                  scan = new Scanner(System.in);
                  memberState = scan.nextLine();
                }
                newProvider.setState(providerState);


                while (true) {
                  System.out.println("Enter provider ZIP Code:");
                  scan = new Scanner(System.in);
                  inputString = scan.nextLine();
                  if (inputString.length() > 5) {
                    System.out.println("Input is longer than 5 digits. Please try again!");
                  } else if (inputString.length() < 5) {
                    System.out.println("Input is shorter than 5 digits. Please try again!");
                  } else {
                    try {
                      int stringValue = Integer.parseInt(inputString);
                      newProvider.setZipCode(stringValue);
                      System.out.println("Provider ZIP Code updated!");
                      break;
                    } catch (NumberFormatException e) {
                      System.out.println("Input contains non-numeric character. Please try again!");
                    }
                  }
                }

                System.out.println("Enter provider Email:");
                scan = new Scanner(System.in);
                providerEmail = scan.nextLine();

                while (provider.containsEmailError(providerEmail)) {
                  System.out.println("Invalid. Make sure to only include 1 '@' for email address.");
                  scan = new Scanner(System.in);
                  providerEmail = scan.nextLine();
                }
                newProvider.setEmail(providerEmail);

                newProvider.setTotalFee(0);
                newProvider.setConsultations(0);


                if (provider.addNewProvider(newProvider)) {
                  System.out.println("Provider has been successfully added.\n");
                } else {
                  System.out.println("Provider could not be added since provider already exists.");
                }
                System.out.println("Would you like to add, delete a provider, modify a provider's "
                    + "records, or go back to actions?\n"
                    + " Enter 'add', 'delete', 'modify', or 'back'.");
                scan = new Scanner(System.in);
                choice = scan.nextLine();

                break;

              case "delete":


                System.out.println("Enter provider number:");
                providerNum = scan.nextInt();

                if (provider.deleteResignedMember(providerNum)) {
                  System.out.println("Provider has been successfully deleted.\n");
                } else {
                  System.out.println("Provider could not be deleted since provider"
                      + " does not exists or provider is not marked as resigned.");
                }	
                System.out.println("Would you like to add, delete a provider, modify "
                    + "a provider's records, or go back to actions?\n"
                    + " Enter 'add', 'delete', 'modify', or 'back'.");
                scan = new Scanner(System.in);
                choice = scan.next();

                break;

              case "modify":

                System.out.println("Enter provider number:");
                providerNum = scan.nextInt();

                if (provider.modifyMemberRecords(providerNum)) {
                  System.out.println("Provider records has been successfully modified.\n");
                }  else {
                  System.out.println("Provider records could not be modified since provider "
                      + "does not exists.");
                }
                System.out.println("Would you like to add, delete a provider, modify a "
                     + "provider's records, or go back to actions?\n"
                     + " Enter 'add', 'delete', 'modify', or 'back'.");
                scan = new Scanner(System.in);
                choice = scan.next();
                break;

              case "back": 
                break;

              default: //print something to let the operator known that their choice was invalid
                System.out.println("Invalid Input.\n");
                System.out.println("Would you like to add, delete a provider, modify a "
                     + "provider's records, or go back to actions? Enter 'add',"
                     + " 'delete', 'modify', or 'back'.");
                scan = new Scanner(System.in);
                choice = scan.nextLine();
                break;

            }

          }
          break;

        default: choice = "exit";
        break;	



      }

      System.out.println("Do you want to perform an action on a member or provider? "
           + "Enter 'member', 'provider', or 'exit' to go back to main menu.");
      scan = new Scanner(System.in);
      actor1 = scan.nextLine();

    }
  }

	
}
