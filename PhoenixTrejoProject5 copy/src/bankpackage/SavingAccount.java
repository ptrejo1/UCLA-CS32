/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package bankpackage;

/**
 * @author phoenixtrejo
 * 
 * SavingAccount is a subclass of BankAccount. A saving account accrues 5% 
 * fixed interest
 */
public class SavingAccount extends BankAccount{
    static final float interestRate = 0.05f;
    
    /**
     * Constructs SavingAccount object with customer and deposit amount
     * @param first Account holder first name
     * @param last Account holder last name
     * @param ssn Account holder social security number
     * @param amount Initial account deposit
    */
    public SavingAccount(String first, String last, String ssn, float amount){
        super(first, last, ssn, amount);
    }
    
    /**
     * Calculates interest and adds it to balance
    */
    @Override
    public void applyInterest(){
        float interest = balance*interestRate;
        balance += interest;
    }
}
