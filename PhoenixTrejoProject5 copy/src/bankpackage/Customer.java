/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bankpackage;

import java.util.regex.Pattern;
import java.util.regex.Matcher;

/**
 * @author phoenixtrejo
 * 
 * Class simulates customer by assigning first and last name and social 
 * security number 
 */
public class Customer {
    // Class variables
    private String firstName;
    private String lastName;
    private String ssn;
    
    /**
     * Constructs Customer object with name and social security number and 
     * checks if valid social security number 
     * @param first Customer first name
     * @param last Customer last name
     * @param sn Customer social security number
    */
    public Customer(String first, String last, String sn){
        firstName = first;
        lastName = last;
        
        Pattern p = Pattern.compile("\\d{3}-\\d{2}-\\d{4}");
        Matcher m = p.matcher(sn);
        boolean isValid = m.matches();
        
        if(isValid){
            ssn = sn;
        } else{
            ssn = "xxx-xx-xxxx";
            System.out.println("Successfully created account for " + first 
                    + " " + last + ". Invalid SSN!");
        } 
    }
    
    /**
     * Override toString for Customer class
     * @return firstName and lastName
    */
    @Override
    public String toString(){
        return firstName + " " + lastName;
    }
    
}
