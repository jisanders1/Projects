package projectclasses;

import java.util.ArrayList;

/** Report lists every provider to be paid that week
 *  the number of consultations each had
 *  his or her totalfee for that week  
 *  the total number of providers who provided services 
 *  the totalnumber of consultations, and the overall fee total are printed. 
 *
 * @author Tyler Coleman
 *
 */


// int total provider, consultation, and fees
public class SummaryReport {
  private ArrayList<Provider> providerArray;
	
	
  public SummaryReport() {
	
  }
  
  public SummaryReport(ArrayList<Provider> providerArray) {
    this.providerArray = providerArray;
  }

  // Total number of consultations(for specified provider)
  private void writeConsultations(int numConsultations) {	
    System.out.println("\tNumber of consultations is " + numConsultations);	
  }
  //prints total fee to be sent over
  
  private void writeAmountDue(double a) {
    System.out.print(" = $");
    System.out.printf("%.2f\n", a);
  }
  // prints out report
  
  /** The summaryReport() method lists every provider to be paid that week
   *  the number of consultations each had
   *  his or her totalfee for that week  
   *  the total number of providers who provided services 
   *  the totalnumber of consultations, and the overall fee total are printed. 
   *
   * @author Tyler Coleman
   *
   */
  public void summaryReport() {
    System.out.println("\n||~~Summary Report~~||");
    int totalNumberOfConsultations = 0;
    int totalNumberOfProviders = 0;
    double totalFeeForAll = 0.0;
    for (int i = 0; i < providerArray.size(); i++) {
      if (providerArray.get(i).getConsultations() != 0) {
        totalNumberOfConsultations += providerArray.get(i).getConsultations();
        totalFeeForAll += providerArray.get(i).getTotalFee();
        totalNumberOfProviders++;
        System.out.println(providerArray.get(i).getName());
        writeConsultations(providerArray.get(i).getConsultations());
        writeAmountDue(providerArray.get(i).getTotalFee());
        System.out.println();
      }
    }
    System.out.println("\tTotal number of consultations: " + totalNumberOfConsultations);	
    System.out.println("\tTotal number of providers who provided services: " 
        + totalNumberOfProviders);
    System.out.println("\nTotals for the week:");
    System.out.print("\tOverall fee: $");
    System.out.printf("%.2f\n", totalFeeForAll);
  }
}