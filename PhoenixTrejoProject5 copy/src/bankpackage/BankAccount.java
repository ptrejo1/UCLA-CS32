/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bankpackage;

/**
 * @author phoenixtrejo
 * 
 * Class simulates a bank account with Customer object, account number, and 
 * account balance
 */
public class BankAccount {
    // Class variables
    private Customer cust;
    private long accountNum;
    float balance;
    
    /**
     * Constructs BankAccount object with customer and deposit amount
     * @param first Account holder first name
     * @param last Account holder last name
     * @param ssn Account holder social security number
     * @param amount Initial account deposit
    */
    public BankAccount(String first, String last, String ssn, float amount){
        cust = new Customer(first, last, ssn);
        balance = amount;
        accountNum = (long)((Math.random()*9e9)+1e9);
        System.out.println("Successfully created account for " 
                + cust.toString() + " Account Number " + accountNum);
        checkBalance();
    }
    
    /**
     * Prints Customer name and account balance
    */
    public void checkBalance(){
        System.out.println(cust.toString() + ", Balance $" + balance);
    }
    
    /**
     * Adds deposit to balance and prints customer name and account
     * balance
     * @param amount the deposit amount
    */
    public void deposit(float amount){
        balance += amount;
        System.out.println(cust.toString() + " deposited $" + amount + 
                ". Current balance " + balance);
    }
    
    /**
     * Checks if Customer is able to make requested withdraw. If not, prints 
     * notifications, otherwise allows withdraw and subtracts amount from 
     * balance.
     * @param amount requested amount for withdraw
    */
    public void withdraw(float amount){
        if(balance >= amount){
            balance -= amount;
            System.out.println(cust.toString() + " withdrew $" + amount + 
                    ". Current balance " + balance);
        } else{
            System.out.println("Unable to withdraw " + amount + " for " 
                    + cust.toString() + " due to insufficient funds" );
        }
    }
    
    /**
     * Empty method subclasses will override as necessary 
    */
    public void applyInterest(){
        
    }
}
