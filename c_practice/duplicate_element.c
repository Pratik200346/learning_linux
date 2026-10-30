#include <stdio.h>

int main()
{
 int arr[] = {3,3,2,4,4,1,2,3,4,7,9};

 int i=0,j=0,k=0,count;
  
  count = sizeof(arr) / sizeof(arr[0]);
  
  for( i= 0; i < count ; i++)
  {
    
    for(j = i +1; j < count ;j++) 
    { 
        if(arr[i] == arr[j])
        {
           for(k = j; k < count; k++)
           {
             arr[k] = arr[k +1];
           }
             
        }
        
    }
  }
   
  for(int z =0 ; z < 6; z++)
     printf("%d ",arr[z]);
  printf("\n");
 return 0;
}
