c/* main.c

   This file written 2015 by F Lundevall and David Broman

   Latest update 2015-09-15 by David Broman

   For copyright and licensing, see file COPYING */

#include <stddef.h>   /* Declarations of integer sizes and the like, part 1 */
#include <stdint.h>   /* Declarations of integer sizes and the like, part 2 */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */

void saveinfo(char s[], void * a, size_t ns );
void showinfo(void);
void u32init(void);

int gv; /* Global variable. */
int in = 3; /* Global variable, initialized to 4711. */

void fun(int param) //kallar med param=m
{
  param++;
  saveword( "AF1: param", &param );
  //addr: &a e.g. A0003FD0
  //data: *a e.g. 00000009
  
  gv = param; /* Change the value of a global variable. */
}

/* This is the main function */
int main()
{
  /* Local variables. */
  int m;
  int * p; /* Declare p as pointer, so that p can hold an address. */
  char cs[9] = "Bonjour!";
  char * cp = cs; /* Declare cp as pointer, initialise cp to point to cs */ //first elem of cs aka 'B'

  /* Do some calculation. */
  gv = 4;
  m = gv + in; //m = 7

  /* Check the addresses and values of some variables and stuff */
  saveword( "AM1: gv", &gv ); //saves a string name str and displays this as a separate page on on the ChipKIT display.
  //prints are on the format:
  //[string] e.g. AM1: gv
  //addr: &a e.g. A000000C *could change between runs
  //data: *a e.g. 00000004
  //n of 20  e.g. 1 of 20
  saveword( "AM2: in", &in );
  //addr: &a e.g. A0000008 *could change between runs
  //data: *a e.g. 00000003
  saveword( "AM3: fun", &fun );
  //addr: &a e.g. 9D001180
  //data: *a e.g. 27BDFFE8
  saveword( "AM4: main", &main );
  //addr: &a e.g. 9D0011D8
  //data: *a e.g. 27BDFFD0
  p = &m; //p är adressen av m

  /* Check p and m */
  saveword( "AM5: p", &p );
  //addr: &a e.g. A0003FE8
  //data: *a e.g. A0003FE4
  saveword( "AM6: m", &m );
  //addr: &a e.g. A0003FE4
  //data: *a e.g. 00000007

  /* Change *p */

  *p = *p + 1; // <=> *p = m+1 <=> m = m+1

  //*p <=> mem[A0003FE4] --> går till minnesaddressen A0003FE4 ger oss värdet 00000008
  /* Check p and m */
  saveword( "AM7: p", &p ); //adressen är samma värdet är samma eftersom den fortfarande pekar på m
  //addr: &a e.g. A0003FE8 
  //data: *a e.g. A0003FE4
  saveword( "AM8: m", &m ); //m värdet är ändrat
  //addr: &a e.g. A0003FE4
  //data: *a e.g. 00000008
  p = (int*)cp;   /* Casting a char pointer to an integer pointer */


  

  saveword( "AM9: p", &p );
  //addr: &a e.g. A0003FE8
  //data: *a e.g. A0003FEC

  savebyte( "AM10: cs[0]", &cs[0] ); //ascii representation of 'B' in hex
  //addr: &a e.g. A0003FEC
  //data: *a e.g. 00000042
  savebyte( "AM11: cs[1]", &cs[1] );//ascii representation of 'o' in hex
  //addr: &a e.g. A0003FED
  //data: *a e.g. 0000006F
  savebyte( "AM12: cs[2]", &cs[2] );//ascii representation of 'n' in hex
  //addr: &a e.g. A0003FEE
  //data: *a e.g. 0000006E
  savebyte( "AM13: cs[3]", &cs[3] );//ascii representation of 'j' in hex
  //addr: &a e.g. A0003FEF
  //data: *a e.g. 0000006A

  *p = 0x1234abcd; /* It starts to get interesting... */
  //    1*16^7 + 2*16^6...

  savebyte( "AM14: cs[0]", &cs[0] );
  //addr: &a e.g. A0003FEC
  //data: *a e.g. CD
  savebyte( "AM15: cs[1]", &cs[1] ); 
  //addr: &a e.g. A0003FED
  //data: *a e.g. AB
  savebyte( "AM16: cs[2]", &cs[2] );
  //addr: &a e.g. A0003FEE
  //data: *a e.g. 34
  savebyte( "AM17: cs[3]", &cs[3] );
  //addr: &a e.g. A0003FEF
  //data: *a e.g. 12

  
  fun(m); //pass by value (pbv) m==8
  //since its pbv m is not effected by icrements in fun

  /* Re-check the addresses and values of m and gv */
  saveword( "AM18: m", &m );
  //addr: &a e.g. A0003FE4
  //data: *a e.g. 00000008

  saveword( "AM19: gv", &gv );
  //addr: &a e.g. A000000C
  //data: *a e.g. 00000009

  showinfo();
}

//registers in mips are 32 bit in size hence chipkit is a 32 bit system
//pointers are then generally 4 bytes = 32 bits in size on 32 bit systems (although not guaranteed by C standards)
//https://stackoverflow.com/questions/399003/is-the-sizeofsome-pointer-always-equal-to-four

//9 bytes eftersom vi ska få plats med nullbyten som avslutar alla textsträngar i C
//8 bytes för 8 karaktärer + 1 för null byte

//funktionerna var på adress 9D001180 fun, 9D0011D8 main är i program flash eftersom deras adresser, A, är:
// 0x9D000000 < A < 0x9D007FFF
//
//adressen för main är dit man hoppar för att köra koden i main?

//in och gv är globala variabler och ligger i RAM minnet

//p och m ligger i Reserved
//vet ej varför de har större adresser - pga att de är i en funktion?

//varför kommer CD först när vi deklarerar *p = 0x1234abcd

//i AM5 är p en pekare på p. Adressen på p är där den är lagrad, likt vilken variabel som helst. Värdet
//på p är adressen för M

//i AM7 pekar p fortfarande på m. Adressen och värdet är oförändrat eftersom vi använder *p = *p +1 <=> m = m +1


//i AM17/AM14 visar det att chipkittet är little endian eftersom MSB CD kommer i index 0, alltså adressen med 0 offset
