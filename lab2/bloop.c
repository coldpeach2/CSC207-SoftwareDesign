#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int hare = 5;
int tortoise = 11;
int j = 0;

int main(){
for(j=0;j<20;j++)
{
    if(hare < tortoise)
        hare *= 2;
    else if(hare == tortoise){
        break;
    }
    else{
        tortoise += 1;
    }
}
int total = hare + tortoise;
printf("%d\n", total);
return total;

}
