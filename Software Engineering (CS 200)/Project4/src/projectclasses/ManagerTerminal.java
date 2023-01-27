package projectclasses;

import java.util.ArrayList;
import java.util.Scanner;



/**
 * The ManagerTerminal class verifies the Manager and 
 * allows the Manager to request individual report(s).
 *
 * @author Sandy-Zheng
 *
 */
public class ManagerTerminal {
  private static ArrayList<Manager> managerRecords = new ArrayList<Manager>();
  private static ReportController repController;
  
  /**
   * This is the constructor for the ManagerTerminal.
   *
   * @author Sandy-Zheng
   * @param memberRecords providerRecords and serviceRecords 
   *     The different records that are needed to generate reports.
   */
  public ManagerTerminal(ArrayList<Member> memberRecords, ArrayList<Provider> providerRecords,
      ArrayList<ServiceRecords> serviceRecords) {
    for (int i = 0; i < 5; i++) {
      Manager newManager = new Manager(400000 + i);
      managerRecords.add(newManager); 
    }
    repController = new ReportController(memberRecords, providerRecords, serviceRecords);
  }
  
  int managerNumberInput;
  int validManagerNumber;
  boolean isValid = false;
  int reportOption;
  int moreReport;
  String reportString;
  int counter = 3;
  
  /**
   * The verifyManager() method compares the user's input with the valid Manager Numbers.
   *
   * @author Sandy-Zheng
   * @return The method returns true if the user's input matches with a valid Manager Number.
   *     The method returns false if the user's input does not match with a valid Manager Number.
   *
   */ 
  public boolean verifyManager() { 
    System.out.println("Enter your manager number: ");
    Scanner in = new Scanner(System.in);

    if (in.hasNextInt()) {
      managerNumberInput = in.nextInt();
    } else {
      System.out.println("Please enter numbers only");
    }
    for (int i = 0; i < 5; i++) {
      if (managerNumberInput == managerRecords.get(i).getManagerNumber()) {
        isValid = true;
        break;
      }
    }
    //in.close();
    return isValid;
  }
  
  /**
   * The requestReport() method calls the ReportController class to help generate 
   * and output different report(s) for the manager.
   *
   * @author Sandy-Zheng
   *
   */
  public void requestReport() {
    Scanner inRepOption;
    counter = 3;
    while (counter != 0) {
      System.out.println("Which report would you like to request?\n");
      System.out.println("Enter 1 for Member Report\nEnter 2 for Provider Report\n"
          + "Enter 3 for Summary Report\nEnter 4 for EFT Report\n"
          + "Enter 0 to Exit the Manager Terminal\n");
      inRepOption = new Scanner(System.in);
      reportString = inRepOption.nextLine();
      try {
        reportOption = Integer.parseInt(reportString);
        if (reportOption < 0 || reportOption > 4) {
          counter--;
          System.out.println("Invalid Input\nYou have " + counter + " attempts left");
          continue;
        }
        counter = 3;
      } catch (NumberFormatException e) {
        System.out.println("Input contains non-numeric characters. Please try again!");
        counter--;
        System.out.println("You have " + counter + " attempts left");
        continue;
      }

      switch (reportOption) {
        case 0:
          System.out.println("---You have exited the Manager Terminal---");
          return;
        case 1:
          if (!repController.selectReportToGenerate(reportOption)) {
            System.out.println("No member reports to print!");
          }
          break;
        case 2:
          if (!repController.selectReportToGenerate(reportOption)) {
            System.out.println("No provider reports to print!");
          }
          break;
        case 3:
          if (!repController.selectReportToGenerate(reportOption)) {
            System.out.println("No summary reports to print!");
          }
          break;
        case 4:
          if (!repController.selectReportToGenerate(reportOption)) {
            System.out.println("No EFT reports to print!");
          }
          break;
        default:
          System.out.println("Please only enter a number 0 through 4");
          break;
      }
    }
  }
}
