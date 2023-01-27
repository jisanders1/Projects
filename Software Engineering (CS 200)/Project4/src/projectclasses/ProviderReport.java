package projectclasses;

import java.util.ArrayList;

/** This is the provider report class which will generate a report for each provider. 
 *
 * @author Cindy Qiu
 *
 */

public class ProviderReport {
  Provider tempProvider = new Provider();
  private String name;
  private String address;
  private String city;
  private String state;
  private int number;
  private int zip;
  private int consultation;
  private ArrayList<ServiceRecords> serviceRecords; 
  private ArrayList<Provider> providerList;


  //default constructor
  public ProviderReport(ArrayList<Provider> providerList, 
      ArrayList<ServiceRecords> serviceRecords) {
    this.providerList = providerList;
    this.serviceRecords = serviceRecords;
  }

  public String getName() {
    return this.name;
  }

  public int getNumber() {
    return this.number;
  }
	
  //public int getFee() {
  //  return this.fee;
  //}
	

	
  /**
   * This will print a report for all the providers.
   *
   * @author Cindy Qiu
   */
	
	
  public void printProviderReport() {
    int totalFees = 0;
    ProviderDirectory directory = new ProviderDirectory();
	
    for (int i = 0; i < providerList.size(); i++) {
      System.out.println("Provider Name: " 
          + providerList.get(i).getName());
      System.out.println("Provider Number: " 
          + providerList.get(i).getNumber());
      System.out.println("Provider Street address: " 
          + providerList.get(i).getAddress());
      System.out.println("Provider City: " 
          + providerList.get(i).getCity());
      System.out.println("Provider State: " 
          + providerList.get(i).getState());
      System.out.println("Provider ZIP Code: " + providerList.get(i).getZipCode());

      //print each services performed for this specific provider
      for (int k = 0; k < serviceRecords.size(); k++) {
        if (providerList.get(i).getName().equals(serviceRecords.get(k).getProviderName())) {

          System.out.println("Date of Service: " + serviceRecords.get(k).getServiceDate());
          System.out.println("Date and time data were received by the computer: " 
              + serviceRecords.get(i).getCurrentDateTime());
          System.out.println("Member Number: " + serviceRecords.get(k).getMemberNumber());
          System.out.println("ServiceCode: " + serviceRecords.get(k).getServiceCode());
          System.out.println("Fee to be paid: " 
              + directory.getFee(serviceRecords.get(k).getServiceCode()));
        }
      }

      for (int j = 0; j < providerList.get(i).serviceCodes.size(); j++) {
        totalFees = totalFees + directory.getFee(providerList.get(i).serviceCodes.get(j));
      }

      System.out.println("Provider Total number of consultations with members (: " 
          + providerList.get(i).serviceCodes.size() + "\n");
      System.out.println("Provider Total fee for week: " + totalFees + "\n\n");
      providerList.get(i).setTotalFee(totalFees);
      totalFees = 0;
    }

  }

	
}