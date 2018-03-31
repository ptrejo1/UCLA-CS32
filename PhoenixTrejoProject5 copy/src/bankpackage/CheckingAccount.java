/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bankpackage;

/**
 * @author phoenixtrejo
 * 
 * CheckingAccount is a subclass of BankAccount. A checking account accrues 2% 
 * fixed interest for any amount in excess of $10000 
 */
public class CheckingAccount extends BankAccount{
    static final float interestRate = 0.02f;
    
    /**
     * Constructs CheckingAccount object with customer and deposit amount
     * @param first Account holder first name
     * @param last Account holder last name
     * @param ssn Account holder social security number
     * @param amount Initial account deposit
    */
    public CheckingAccount(String first, String last, String ssn, float amount){
        super(first, last, ssn, amount);
    }
    
    /**
     * Checks for minimum amount and if condition satisfied 
     * calculates interest and adds it to balance
    */
    @Override
    public void applyInterest(){
        if(balance > 10000.0f){
            float interest = (balance-10000.0f)*interestRate;
            balance += interest; 
        }
    }
}
