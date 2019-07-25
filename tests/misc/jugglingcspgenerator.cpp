#include <cstdio>
#include <cstdlib>

// script that generates variations of the juggling stream csp
// compile as g++ jugglingcspgenerator.cpp -o jugglingcspgenerator
// run as ./jugglingcspgenerator <num balls> <force> > <outfile>

int main(int argc, char* argv[]){
   char* bs = argv[1];
   char* fs = argv[2];
    
    //printf("%s\n", bs);
    //printf("%s\n", fs);

   int b;
   int f;

   sscanf(bs, "%d", &b);
   sscanf(fs, "%d", &f);

   printf("var A : [0, %d];\n", f);
   for(int c = 0; c < b; c++){
      printf("var B%d : [0, %d];\n", c, f);
   }
   printf("\n");

   for(int c = 0; c < b; c++){
      printf("next B%d == if B%d eq 1 then A else (B%d - 1);\n", c, c, c);
   }
   printf("\n");

   for(int c = 0; c < b; c++){
      for(int d = c+1; d < b; d++){
         printf("B%d != B%d;\n", c, d);
      }
   }
   printf("\n");

   printf("A == ");
   for(int c = 0; c < b; c++){
      printf("if B%d eq 1 then next B%d\nelse ", c, c);
   }
   printf("0;\n\n");

   printf("first B0 == 1;\n");
   for(int c = 0; c < b-1; c++){
      printf("first B%d < first B%d;\n", c, c+1);
   }
   return 0;
}
