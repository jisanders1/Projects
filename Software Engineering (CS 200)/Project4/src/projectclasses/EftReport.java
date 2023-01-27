package projectclasses;

import java.util.ArrayList;


/**
 *The EftReport Class will help print out the provider name, provider number, 
 *and the amount to be transferred to the console.
 *
 * @author Tyler Coleman
 *
 */
public class EftReport {
  private String providerName;
  private String providerNumber;
  private String amount;
  private ArrayList<Provider> providerArray;
  
  public EftReport() {

  }
  
  public EftReport(ArrayList<Provider> providerArray) {
    this.providerArray = providerArray;
  }
  //prints out the provider information
  
  public void writeProviderName(String a, int b) {	
    System.out.println("" + a + "(" + b + "):");
  }
  //prints total fee to be sent over
  
  public void writeAmountDue(double a) {
    System.out.print(" = $");
    System.out.printf("%.2f\n", a);
  }
  /**
   *printEftReport() prints the Provider name, number and total fee that is owed.
   *
   * @author Tyler Coleman
   */
  
  public void printEftReport() {
    for (int i = 0; i < providerArray.size(); i++) {
      writeProviderName(providerArray.get(i).getName(), providerArray.get(i).getNumber());
      writeAmountDue(providerArray.get(i).getTotalFee());
    }
  }
}
