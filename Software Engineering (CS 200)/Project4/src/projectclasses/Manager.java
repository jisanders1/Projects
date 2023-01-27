package projectclasses;

/**
 * This is the Manager class and it allows an Manager to be created.
 * It also has the get and set functions for the Manager Number.
 *
 * @author Sandy-Zheng
 *
 */
public class Manager {

  private int managerNumber;

  Manager(int managerNumber) {
    this.managerNumber = managerNumber;
  }
  
  public void setManagerNumber(int newManagerNumber) {
    this.managerNumber = newManagerNumber;
  }
  
  public int getManagerNumber() {
    return managerNumber;
  }
}
