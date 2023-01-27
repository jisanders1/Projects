package projectclasses;

import java.util.ArrayList;
import java.util.Scanner;

/**
 * This class is the controller for a manager or a timer to generate and print reports.

 * @author Jaylon Sanders
 *
 */
public class ReportController {
  private ArrayList<Member> memberRecords;
  private ArrayList<Provider> providerRecords;
  private ArrayList<ServiceRecords> serviceRecords;
  private MemberReport memberReportGenerator;
  private ProviderReport providerReportGenerator;
  private SummaryReport summaryReportGenerator;
  private EftReport eftReportGenerator;
  
  public ReportController() {
    
  }
  /**
   * This is the Default Constructor for the report controller.

   * @author Jaylon Sanders
   * @param memberRecords An array list of member records that are being passed in.
   * @param providerRecords An array list of provider records that are being passed in.
   * @param serviceRecords An array list of provider records that are being passed in.
   */
  public ReportController(ArrayList<Member> memberRecords, ArrayList<Provider> providerRecords,
      ArrayList<ServiceRecords> serviceRecords) {
    
	this.memberRecords = memberRecords;
	this.providerRecords = providerRecords;
	this.serviceRecords = serviceRecords;
	
    memberReportGenerator = new MemberReport(memberRecords, serviceRecords);
    providerReportGenerator = new ProviderReport(providerRecords, serviceRecords);
    summaryReportGenerator = new SummaryReport(providerRecords);
    eftReportGenerator = new EftReport(providerRecords);
  }
  
  /**
   * This method generates a report given an input.

   * @author Jaylon Sanders
   * @param reportType Indicates the type of report to generate:
   *     1 for a member report, 2 for a provider report, 3 for a summary report
   *     and 4 for an ETF report
   * @return Returns true if generating the report was successful. 
   *     Returns false if generating the reports were unsuccessful.
   */
  public boolean selectReportToGenerate(int reportType) {
    boolean isSuccessful = false;
    switch (reportType) {
      case 1:
        if (memberRecords.size() != 0) {
          memberReportGenerator.printMemberReport();
          isSuccessful = true;
        }
        break;
      case 2:
    	if (providerRecords.size() != 0) {
          providerReportGenerator.printProviderReport();
          isSuccessful = true;
    	}
        break;
      case 3:
    	if (providerRecords.size() != 0) {
    	  summaryReportGenerator.summaryReport();
          isSuccessful = true;
        }
        break;
      case 4:
        if (providerRecords.size() != 0) {
          eftReportGenerator.printEftReport();
          isSuccessful = true;
        }
        break;
      default:
        System.out.println("Incorrect integer passed.");
        break;
    }
    return isSuccessful;
  }
}