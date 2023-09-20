package files;

import java.util.ArrayList;
import java.util.Scanner;

public class MainMenu {
    private static boolean verifyUser(String role) {

        return false;
    }
    public static void main(String[] args) {
        // How the records are stored
        // ArrayList<Member> memberRecords = new ArrayList<Member>();
        // ArrayList<Provider> providerRecords = new ArrayList<Provider>();
        // ArrayList<ServiceRecords> serviceRecords = new ArrayList<ServiceRecords>();
        // ProviderDirectory provDir = new ProviderDirectory();
        while (true) {
            System.out.println("Enter what your role is (Manager, Provider or Operator) or Enter 'Report' to run main accounting procedure");
            String role;
            Scanner inputScan = new Scanner(System.in);
            role = inputScan.nextLine();
            boolean passedValidation = false;

            switch (role) {
                case "Manager":
                    int counter = 0;
                    int attemptsLeft = 3;
                    //ManagerTerminal newManagerTerminal = new ManagerTerminal(memberRecords, providerRecords, serviceRecords);

                    while (counter < 3) {
                        counter++;
                        attemptsLeft--;
                        //newManagerTerminal.verifyManager() == true 
                        //! This goes inside the if statement below
                        if (verifyUser(role)) {
                            System.out.println("---Valid Manager Number---");
                            passedValidation = true;
                            break;
                        }
                        System.out.println("Invalid Number. You have " + attemptsLeft + " attempts left");
                    }

                    if (passedValidation) {
                        //newManagerTerminal.requestReport();
                    } else {
                        System.out.println("---Failed login 3 times, exiting system---");
                    }
                    break;
                case "Provider":
                    counter = 0;
                    int providerId = 0;
                    attemptsLeft = 3;
                    // ProviderTerminal provTerm = new ProviderTerminal(providerRecords);
                    // ProviderController provControl = new ProviderController(memberRecords, providerRecords);
                    while (counter < 3) {
                        counter++;
                        System.out.println("Enter your provider number: ");
                        providerId = inputScan.nextInt();
                        //provTerm.verifyProvider(providerId)
                        //! This goes inside the if statement below
                        if (verifyUser(role)) {
                            System.out.println("---Valid Provider Number---");
                            passedValidation = true;
                            break;
                        }
                        System.out.println("Invalid Number. You have " + attemptsLeft + " attempts left");
                    }
                    break;
            }



            inputScan.close();
        }
    }
}