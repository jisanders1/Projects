package projectclasses;

//Tyler classes

/**
 * This class is a single entry of a Service.
 *
 * @author Tyler Coleman
 *
 */
public class ServiceRecords {
  private String currentDateTime;
  private String serviceDate;
  private int providerNumber;
  private String providerName;
  private String memberName;
  private int memberNumber;
  private int serviceCode;
  private String serviceName;
  private String comments;

  public ServiceRecords() {
    currentDateTime = "11-23-2022 08:19:00";
  }

  /**
   * This method is a default constructor and allows the user to set all service record values.
   *
   * @author Tyler Coleman
   *
   */
  public ServiceRecords(String currentDateTime, String serviceDate, 
        int providerNumber, String providerName, int memberNumber, 
        String memberName, int serviceCode, String serviceName, 
        String com) {
    this.memberNumber = memberNumber;
    this.memberName = memberName;
    this.providerNumber = providerNumber;
    this.providerName = providerName;
    this.currentDateTime = currentDateTime;
    this.serviceDate = serviceDate;
    this.serviceCode = serviceCode;
    this.serviceName = serviceName;
    comments = com;
  }

  //Get member number
  public int getMemberNumber() {
    return memberNumber;
    //test
  }
  
  public String getMemberName() {
    return memberName;
  }
  
  //Get Provider Number
  public int getProviderNumber() {
    return providerNumber;
  }
  
  public String getProviderName() {
    return providerName;
  }
  
  // Date/Time the record was officially made
  public String getCurrentDateTime() {
    return currentDateTime;
  }
 
  //Date service was issued
  public String getServiceDate() {
    return serviceDate;
  }

  public int getServiceCode() {
    return serviceCode;
  }
  
  public String getServiceName() {
    return serviceName;
  }

  public String getComments() {
    return comments;
  }

}


