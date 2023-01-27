package projectclasses;

import java.util.ArrayList;

/** This is the provider class which allows a provider to be created.
 *
 * @author Cindy Qiu
 *
 */
public class Provider {
  private
      String providerName;
  String providerAddress;
  String providerCity;
  String providerState;
  String providerEmail;
  int providerNumber;
  int providerZipCode;
  boolean isResigned;
  int numberOfConsultations;
  double totalFeeForWeek;
  ArrayList<Integer> serviceCodes = new ArrayList<Integer>();

  public
      void setName(String inputName) {
    providerName = inputName;
  }

  void setAddress(String inputAddress) {
    providerAddress = inputAddress;
  }

  void setEmail(String inputEmail) {
    providerEmail = inputEmail;
  }

  void setCity(String inputCity) {
    providerCity = inputCity;
  }

  void setState(String inputState) {
    providerState = inputState;
  }

  void setNumber(int inputNumber) {
    providerNumber = inputNumber;
  }

  void setZipCode(int inputZipCode) {
    providerZipCode = inputZipCode;
  }

  void setStatus(boolean inputStatus) {
    isResigned = inputStatus;
  }

  void setConsultations(int inputNumOfConsultations) {
    numberOfConsultations = inputNumOfConsultations;
  }

  void setTotalFee(double inputTotalFee) {
    totalFeeForWeek = inputTotalFee;
  }


  String getName() {
    return providerName;
  }
	
  String getAddress() {
    return providerAddress;
  }

  String getEmail() {
    return providerEmail;
  }

  int getZipCode() {
    return providerZipCode;
  }

  String getCity() {
    return providerCity;
  }

  String getState() {
    return providerState;
  }

  int getNumber() {
    return providerNumber;
  }

  boolean getStatus() {
    return isResigned;
  }

  int getConsultations() {
    return numberOfConsultations;
  }

  double getTotalFee() {
    return totalFeeForWeek;
  }

  void addServiceCode(int code) {
    serviceCodes.add(code);	
  }

  void addTotalFeeForWeek(double fee) {
    totalFeeForWeek = fee;
  }
	
}