#include <gmp.h>
#include <gmpxx.h>
#include <stdio.h>
#include <assert.h>
#include<cstdlib>
#include<ctime>

int main(){

  //define the inputs to be used
  char inputStr[1024]; // char string for input
  mpz_t n,d;
  int flag, trial=1;
  float prob=0.25;

  printf ("Enter your number: ");
  scanf("%1023s" , inputStr); //take input value

  //Initialize the number n
  mpz_init(n);
  mpz_set_ui(n,0);

  //Parse the input string as a base 10 number
  flag = mpz_set_str(n,inputStr, 10);
  assert (flag == 0); // If flag is not 0 then the operation failed

  // Set the value of d as n-1
  mpz_init(d);
  mpz_set_ui(d,0);
  mpz_sub_ui(d,n,1);

  //use the mpz class in order to use C++ primitives for loops and arithmetic
  mpz_class v (d);
  mpz_class w (n);
  mpz_class s (0);

  //check for input correctness

 //Do not accept negative numbers or zero
  if (w<1)
     {
   printf ("Enter a positive number. ");
   return 0;
    }
 //check for trivial solution of n=1
if (w==1)
     {
   printf ("1 is not a prime ");
   return 0;
    }
 //check for trivial solution of n=2
if (w==2)
    {
   printf ("n is prime with probability=1 ");
   return 0;
    }
    //check for trivial solution of n is even
if (w%2==0)
    {
   printf ("n is an even number ");
   return 0;
    }

  //Miller-Rabin Test

 //computing n-1=(2^s)*v where v is an odd number
  do
   {
     v=v/2;
     s=s+1;
   }
   while(v% 2==0);

new_rand:

//generate a random number 'a' mod d

srand(time(0));

more_trials: //Shall use goto loop to repeat test severally at this point

mpz_class a=rand();
mpz_class expo (1);
mpz_class expo2 (2);

//Reduce a mod n
mpz_powm (a.get_mpz_t(), a.get_mpz_t(), expo.get_mpz_t(), d);

//check if the random number generator gives zero
    if(a==0)
    goto new_rand;

//Assign b=a^v mod n
mpz_class b;
mpz_powm (b.get_mpz_t(), a.get_mpz_t(), v.get_mpz_t(), n);


//check if b%n=1
mpz_class rem_1;
mpz_powm (rem_1.get_mpz_t(), b.get_mpz_t(), expo.get_mpz_t(), n);
if(rem_1==1)//probably prime
{
   goto trial_loop; //we have to repeat with different a's to reduce error
}

//for loop from 0 to s-1 where s is the exponent of 2
for(int i=0;i<s;i++)
{
mpz_class rem_2(b+1);
mpz_powm (rem_2.get_mpz_t(), rem_2.get_mpz_t(), expo.get_mpz_t(), n);

    if(rem_2==0)//probably prime
      {
       goto trial_loop;//we have to repeat with different a's to reduce error
      }
    else//Assign b=b^2 mod n
        {
         mpz_powm (b.get_mpz_t(), b.get_mpz_t(), expo2.get_mpz_t(), n);
        }
}
//If test fails
printf ("n is composite");
goto clear_mem;//deallocate memory

//For probably prime repeat test 5 times to reduce error
trial_loop:
if(trial<6)
{
    trial++;
    prob=0.25*prob;//calculate error each iteration
    goto more_trials;
}
printf ("n is prime with probability of error = ");
printf ("%0.6f", prob);

  // Clean up the mpz_t handles or else we will leak memory
  clear_mem:
  mpz_clear(n);
  mpz_clear(d);

}
