package projectclasses;

import java.util.ArrayList;
import java.util.Scanner;

/**
 * The MainMenu class is the starting point for all roles.
 * Allows a user to specify their role, perform tasks, and change roles.

 * @author Everyone
 */
public class MainMenu {
  /**
   * The main class contains the code necessary to provide the functions
   * for everyone.

   * @author Everyone
   */
  public static void main(String[] args) {
    ArrayList<Member> memberRecords = new ArrayList<Member>();
    ArrayList<Provider> providerRecords = new ArrayList<Provider>();
    ArrayList<ServiceRecords> serviceRecords = new ArrayList<ServiceRecords>();
    ProviderDirectory provDir = new ProviderDirectory();
    while (true) {
      System.out.println("Enter what your role is (Manager, Provider or Operator) "
          + "or Enter 'Report' to run main accounting procedure");

      String role;
      Scanner inputScan = new Scanner(System.in);
      role = inputScan.nextLine();

      switch (role) {
        case "Manager":
          int counter = 0;
          int attemptsLeft = 3;
          boolean invalidTryThree = true;
          ManagerTerminal newManagerTerminal = new ManagerTerminal(memberRecords, 
               providerRecords, serviceRecords);
          while (counter < 3) {
            counter++;
            attemptsLeft--;
            if (newManagerTerminal.verifyManager() == true) {
              counter = 4;
              System.out.println("---Valid Manager Number---");
              invalidTryThree = false;
              break;
            }
            System.out.println("Invalid Number. You have " + attemptsLeft + " attempts left");
          }
          if (invalidTryThree) {
            System.out.println("---Failed login 3 times, exiting system---");
            break;
          } else {
            newManagerTerminal.requestReport();
            break;
          }
          //----------------------------------------------------------------------------

        case "Provider":
          int count = 0;
          int providerId = 0;
          boolean invalTryThree = true;
          ProviderTerminal provTerm = new ProviderTerminal(providerRecords);
          ProviderController provControl = new ProviderController(memberRecords, providerRecords);
          while (count < 3) {
            count++;
            System.out.println("Enter your provider number: ");
            providerId = inputScan.nextInt();
            if (provTerm.verifyProvider(providerId)) {
              count = 4;
              System.out.println("---Valid Provider Number---");
              invalTryThree = false;
              break;
            }
          }
          if (invalTryThree) {
            System.out.println("---Failed login 3 times, exiting system---");
            break;
          } else {
            while (true) {
              System.out.println("To Bill ChocAn, enter 1. To Request Provider Directory, enter 2");
              int choice = inputScan.nextInt();
              if (choice == 2) { //request provider directory
                provDir.requestDirectory();
              }
              if (choice == 1) { //Bill ChocAn
                System.out.println("Enter Date of Service");
                String dateServ = inputScan.nextLine();
                dateServ = inputScan.nextLine();
                System.out.println("Enter Member ID");
                int memId = inputScan.nextInt();
                System.out.println("Enter Service Code");
                int servCode = inputScan.nextInt();
                
                Provider usedProvider = null;
                //finds provider instance that will be sent to billChocAn
                for (int x = 0; x < providerRecords.size(); x++) {
                  if (providerRecords.get(x).getNumber() == providerId) {
                    usedProvider = providerRecords.get(x);
                    break;
                  }
                }
                boolean billChocAnCheck = provControl.billChocAn(dateServ, servCode, memId, 
                     providerId, serviceRecords, usedProvider);

                if (billChocAnCheck) {
                  System.out.println("Bill ChocAn Completed");
                } else {
                  System.out.println("Bill ChocAn Exited"); //check

                }
              }
              System.out.println("To exit provider terminal, press 1. To stay press 2.");
              int choice2 = inputScan.nextInt();
              if (choice2 == 1) {
                break;
              }
            }
            break;
          }
        case "Operator":
          int counterOp = 0;
          int attemptsLeftOp = 3;
          boolean invalidTryThree1 = true;
          OperatorTerminal opTerm = new OperatorTerminal(memberRecords, providerRecords);
          while (counterOp < 3) {
            counterOp++;
            System.out.println("Enter operator number:");
            Scanner opNum = new Scanner(System.in);
            int optNum = opNum.nextInt();

            attemptsLeftOp--;
            if (opTerm.verifyOperator(optNum) == true) {
              counter = 4;
              System.out.println("---Valid Operator Number---");
              invalidTryThree1 = false;
              counterOp = 3;
              break;
            }
            System.out.println("Invalid Number. You have " + attemptsLeftOp + " attempts left");
          }
          if (invalidTryThree1) {
            System.out.println("---Failed login 3 times, exiting system---");
            break;
          } else {
            opTerm.display(memberRecords, providerRecords);
            break;
          }
        default:
          System.out.println("Invalid input please try again");
          break;

        case "Report":
          ReportController repController = new ReportController(memberRecords, 
               providerRecords, serviceRecords);
          repController.selectReportToGenerate(1);
          repController.selectReportToGenerate(2);
          repController.selectReportToGenerate(3);
          repController.selectReportToGenerate(4);
      }
      
    
      System.out.println("Would you like to quit the system? Enter 1 to quit, "
          + "2 to continue as a Manager, Provider, Operator, "
          + "or to Run the Main Accounting Procedure");
      int endChoice = inputScan.nextInt();
    
      if (endChoice == 1) {
        System.out.println("You have exited the system.");
        break;
      }
    }
  }
}