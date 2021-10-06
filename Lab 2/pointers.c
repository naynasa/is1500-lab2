


#include <stdio.h>
#include <stdint.h>

char* text1 = "This is a string.";
char* text2 = "Yet another thing.";

int32_t list1[20]; //80 bytes in assembly = 640 bits and 640/32 = 20
int32_t list2[20]; 
int count = 0;


void printlist(const int* lst){
  printf("ASCII codes and corresponding characters.\n");
  while(*lst != 0){
    printf("0x%03X '%c' ", *lst, (char)*lst);
    lst++;
  }
  printf("\n");
}

void endian_proof(const char* c){
  printf("\nEndian experiment: 0x%02x,0x%02x,0x%02x,0x%02x\n", 
         (int)*c,(int)*(c+1), (int)*(c+2), (int)*(c+3));
  
}
void copycodes(char* text_ptr,int* list_ptr,int* count_ptr){
  //closer to assembly but harder to read code
 /*
  int t1;//lw längre ner ger t1 är int
  char t0 = *text_ptr; //mem[a0]
  while(t0 != 0){ //kollar om vi kommit till slutet av strängen / funnit null byten
    
    *list_ptr = t0;//mem[a1] = t0;
    text_ptr += 1;
    list_ptr += 1;
    t1 = *count_ptr; //t1 = mem[a2]
    t1 += 1;
    *count_ptr = t1;//mem[a2] = t1
    
    t0 = *text_ptr; //mem[a0] <=>lb	$t0,0($a0)
   }
 */

   
    while(*text_ptr != 0){//kollar om vi kommit till slutet av strängen / funnit null byten
      *list_ptr = *text_ptr; //dereferencar list_ptr <=> hämta ut värdet som sparas på pekarens adress
      text_ptr += 1;
      list_ptr += 1;//4 i assembly
      *count_ptr += 1; //incrementar count direkt ekvivalent med count += 1 (men vi har inte count i funktionen)
     }
   

}
void work(){
  /*
  //ladda in globala variabler i a0,a1,a2
  char* a0 = text1;
  int* a1 = list1;
  int* a2 = &count;
  copycodes(a0,a1,a2);

  //ladad in nya globala variabler i a0,a1,a2
  a0 = text2;
  a1 = list2;
  a2 = &count;
  copycodes(a0,a1,a2);
  */
 copycodes(text1,list1,&count); //använd redan definierade pekare till text1,text2,list1,list2 och få countpekaren med &-operatorn
 copycodes(text2,list2,&count);
}


int main(void){
  work();

  printf("\nlist1: ");
  printlist(list1);
  printf("\nlist2: ");
  printlist(list2);
  printf("\nCount = %d\n", count);

  endian_proof((char*) &count); //printar "{1:a biten av count}, {2:a biten av count}, {3:e biten av count}, {4:e biten av count}"
  //minst signifikanta biten printas först
  //alltså little endian 
}

//little endian bra eftersom:
//* lätt att läsa datatyper av olika längd med ett visst antal bitar 
//t.ex. printar man första 4 bitarna oavsett längd får man alltid talets 4 minst sig. bitar
//* lätt att göra om mellan datatyper 32 bit -> 16 bit blir bara ta första 16  bitarna

//big endian bra eftersom:
//* räcker med att testa en bit för att se om talet är positivt eller negativt
//* siffrorna lagras i samma ordning som de printas i bas 10 eller 16