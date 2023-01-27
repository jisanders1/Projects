package projectclasses;

import java.util.ArrayList;

/**
 * The Provider Terminal Allows adds the functionality of verifying the provider before being 
 * given access to the provider controller.

 * @author Zack Michaels
 * 
 */

public class ProviderTerminal {
  private ArrayList<Integer> providerIds = new ArrayList<>();
  private int size;
  /**
* Provider Controller constructor that fills private array with up-to-date data.

* @author Zachary Michaels
* @param providerList of providers is entered that will be stored in the Provider Terminal that will
*     be used later for provider verification purposes
*/

  public ProviderTerminal(ArrayList<Provider> providerList) { //need provider records instead
    size = providerList.size();

    for (int i = 0; i < size; i++) {
      providerIds.add(providerList.get(i).getNumber());
    }
  }
  /**
* Verify Provider uses previously stored data to verify the provider.

* @author Zachary Michaels
* @param id entered as an integer for verification
* @return Returns true if the provider ID is valid, else false if invalid. Returns a boolean value
*/
  public boolean verifyProvider(int id) {
    for (int i = 0; i < size; i++) {
      if (providerIds.get(i) == id) {
        return true;
      }
    }
    return false;
  }
}