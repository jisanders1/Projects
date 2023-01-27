package projectclasses;

import java.util.ArrayList;
import java.util.Scanner;

/**
 * This class is the controller for an Operator to manipulate provider records.

 * @author Jaylon Sanders
 *
 */
public class ModifyProviderRecords {
  private ArrayList<Provider> providerRecords;

  public ModifyProviderRecords() {
    
  }
  
  //Default Constructor
  public ModifyProviderRecords(ArrayList<Provider> inputProviderRecords) {
    providerRecords = inputProviderRecords;
  }
  /**
   * This method adds a new provider to the records if the provider has not already been added.

   * @author Jaylon Sanders
   * @param providerToAdd The provider that is being added to the records.
   * @return Returns true if adding was successful,returns false when the provider is in the system.
   */
  public boolean addNewProvider(Provider providerToAdd) {
    Provider searchProvider = searchByNum(providerToAdd.getNumber());
    if (searchProvider == null) {
      providerRecords.add(providerToAdd);
      return true;
    }
    return false;
  }

  /**
   * This method deletes a resigned provider from the records given their provider number.

   * @author Jaylon Sanders
   * @param providerNum The provider number of the provider being deleted.
   * @return Returns true when the provider was successfully deleted. 
   *     Returns false if the provider is not in the system or if they did not resign.
   */
  public boolean deleteResignedMember(int providerNum) {
    Provider searchProvider = searchByNum(providerNum);
    if (searchProvider != null) {
      if (searchProvider.isResigned) {
        providerRecords.remove(searchProvider);
        return true;
      } else {
        return false;
      }
    }
    return false;
  }

  /**
   * This method modifies a provider given their provider number.
   * It also allows the user to save or not save their changes

   * @author Jaylon Sanders
   * @param providerNum The provider number of the provider being deleted.
   * @return Returns true if the provider is successfully modified.
   *     Returns false if the provider is not in the system. 
   */
  public boolean modifyMemberRecords(int providerNum) {
    Provider searchProvider = searchByNum(providerNum);
    if (searchProvider != null) {
      //Storing Previous Information in case of errors or 
      //if the operator decides to not save the changes
      Provider tempProvider = new Provider();
      tempProvider.setName(searchProvider.getName());
      tempProvider.setNumber(searchProvider.getNumber());
      tempProvider.setAddress(searchProvider.getAddress());
      tempProvider.setCity(searchProvider.getCity());
      tempProvider.setState(searchProvider.getState());
      tempProvider.setZipCode(searchProvider.getZipCode());
      tempProvider.setName(searchProvider.getName());
      tempProvider.setEmail(searchProvider.getEmail());
      tempProvider.setStatus(searchProvider.getStatus());
      tempProvider.setConsultations(searchProvider.getConsultations());
      tempProvider.setTotalFee(searchProvider.getTotalFee());
      
      Scanner input;
      while (true) {
        System.out.println("Fields for modication:");
        System.out.println("1 - For Provider Name\n2 - For Provider Number");
        System.out.println("3 - For Provider Street Address\n4 - For Provider City");
        System.out.println("5 - For Provider State\n6 - For Provider Zip Code");
        System.out.println("7 - For Provider Email\n8 - For Provider Status");
        System.out.println("9 - For Provider Consultations\n10 - For Provider Fees");
        System.out.println("11 - To Save and Exit");
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
              System.out.print("Enter name to update provider name (up to 25 characters): ");
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
            searchProvider.setName(inputString);
            System.out.println("Provider name updated!");
            break;
          case 2:
            while (true) {
              System.out.print("Enter number to update provider number (9 digits): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 9) {
                System.out.println("Input is longer than 9 digits. Please try again!");
              } else if (inputString.length() < 9) {
                System.out.println("Input is shorter than 9 digits. Please try again!");
              } else {
                try {
                  int stringValue = Integer.parseInt(inputString);
                  searchProvider.setNumber(stringValue);
                  System.out.println("Provider number updated!");
                  break;
                } catch (NumberFormatException e) {
                  System.out.println("Input contains non-numeric characters. Please try again!");
                }
              }
            }
            break;
          case 3:
            while (true) {
              System.out.print("Enter address to update provider address (up to 25 characters):");
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
            searchProvider.setAddress(inputString);
            System.out.println("Provider street address updated!");
            break;
          case 4:
            while (true) {
              System.out.println("Enter city to update provider city (14 characters)");
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
            searchProvider.setCity(inputString);
            System.out.println("Provider city updated!");
            break;
          case 5:
            while (true) {
              System.out.print("Enter state to update provider state (2 letters): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 2) {
                System.out.println("Input is longer than 2 characters. Please try again!"); 
              } else if (containsDigit(inputString)) {
                System.out.println("Input contains a number. Please try again!");
              } else if (containsSpecialCharacter(inputString)) {
                System.out.println("Input contains a special character. Please try again!");
              } else {
                break;
              }
            }
            inputString.toUpperCase();
            searchProvider.setState(inputString);
            System.out.println("Provider state updated!");
            break;
          case 6:
            while (true) {
              System.out.print("Enter zip code to update provider zip code (5 digits): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 5) {
                System.out.println("Input is longer than 5 digits. Please try again!");
              } else if (inputString.length() < 5) {
                System.out.println("Input is shorter than 5 digits. Please try again!");
              } else {
                try {
                  int stringValue = Integer.parseInt(inputString);
                  searchProvider.setZipCode(stringValue);
                  System.out.println("Provider zip code updated!");
                  break;
                } catch (NumberFormatException e) {
                  System.out.println("Input contains non-numeric characters. Please try again!");
                }
              }
            }
            break;
          case 7:
            while (true) {
              System.out.println("Enter email to update provider email");
              System.out.print("Format: username@gmail.com): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (containsEmailError(inputString)) {
                System.out.println("Input contains multiple '@' symbols. Please try again!"); 
              } else {
                break;
              }
            }
            searchProvider.setEmail(inputString);
            System.out.println("Provider email updated!");
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
              searchProvider.setStatus(false);
            } else {
              searchProvider.setStatus(true);
            }
            System.out.println("Provider resignation status updated!");
            break;  
          case 9:
            while (true) {
              System.out.print("Enter number to update provider consultations (3 digits): ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 3) {
                System.out.println("Input is longer than 3 digits. Please try again!");
              } else if (inputString.length() < 3) {
                System.out.println("Input is shorter than 3 digits. Please try again!");
              } else {
                try {
                  int stringValue = Integer.parseInt(inputString);
                  searchProvider.setNumber(stringValue);
                  System.out.println("Provider consultations updated!");
                  break;
                } catch (NumberFormatException e) {
                  System.out.println("Input contains non-numeric characters. Please try again!");
                }
              }
            }
            break;
          case 10:
            while (true) {
              System.out.println("Enter number to update provider fee owed for the week");
              System.out.print("Format - xxxxx.xx: ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              if (inputString.length() > 8) {
                System.out.println("Input is longer than 8 characters. Please try again!");
              } else if (inputString.length() < 8) {
                System.out.println("Input is shorter than 8 characters. Please try again!");
              } else if (containsDecimalPoints(inputString)) {
                System.out.println("Input contains multiple decmial points. Please try again!");
              } else if (inputString.charAt(5) != '.') {
                System.out.println("Decimal point in the incorrect position. Please try again!");
              } else {
                try {
                  double stringValue = Double.parseDouble(inputString);
                  searchProvider.setTotalFee(stringValue);
                  System.out.println("Provider consultations updated!");
                  break;
                } catch (NumberFormatException e) {
                  System.out.println("Input contains non-numeric characters. Please try again!");
                }
              }
            }
            break;
          case 11:
            while (true) {
              System.out.println("Do you want to save your changes?");
              System.out.print("Enter yes or no : ");
              input = new Scanner(System.in);
              inputString = input.nextLine();
              inputString.toLowerCase();
              
              if (inputString.equals("yes")) {
                break;
              } else if (inputString.equals("no")) {
                searchProvider.setName(tempProvider.getName());
                searchProvider.setNumber(tempProvider.getNumber());
                searchProvider.setAddress(tempProvider.getAddress());
                searchProvider.setCity(tempProvider.getCity());
                searchProvider.setState(tempProvider.getState());
                searchProvider.setZipCode(tempProvider.getZipCode());
                searchProvider.setName(tempProvider.getName());
                searchProvider.setEmail(tempProvider.getEmail());
                searchProvider.setConsultations(tempProvider.getConsultations());
                searchProvider.setTotalFee(tempProvider.getTotalFee());
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
   * This method searches for a provider in the providerRecords array list.

   * @author Jaylon Sanders
   * @param num The number of a provider to search for.
   * @return Returns the provider if found, returns null if provider is not found.
   */
  private Provider searchByNum(int num) {
    for (int i = 0; i < providerRecords.size(); i++) {
      if (num == providerRecords.get(i).getNumber()) {
        return providerRecords.get(i);
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
   * This method verifies if there is exactly one decimal point.

   * @author Jaylon Sanders
   * @param searchString The string that is being searched through.
   * @return Returns true if there are multiple or no decimal points.
   *     Returns false if there is exactly one decimal point.
   */
  public boolean containsDecimalPoints(String searchString) {
    int numberOfSymbols = 0;
    for (int i = 0; i < searchString.length(); i++) {
      if (searchString.charAt(i) == '.') {
        numberOfSymbols++;
      }
    }

    if (numberOfSymbols == 1) {
      return false;
    } else {
      return true;
    }
  }
  
  /**
   * This method verifies if each char in a string is a letter.

   * @author Jaylon Sanders
   * @param searchString The string that is being searched through.
   * @return Returns true if a special character is found in the string (bar '.' and ' ').
   *     Returns false if the string does not have any errors.
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