package projectclasses;

/**
 * This is the Operator class and it allows an Operator to be created.
 * It also has the get and set functions for the Operator Number.
 *
 * @author Sandy-Zheng
 *
 */

public class Operator {
  private int operatorNumber;
  
  Operator(int operatorNumber) {
    this.operatorNumber = operatorNumber;
  }
  
  public int getOperatorNumber() {
    return operatorNumber;
  }	
  
  public void setOperatorNumber(int newOperatorNumber) {
    this.operatorNumber = newOperatorNumber;
  }
}