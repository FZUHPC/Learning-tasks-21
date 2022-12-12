#include <stdio.h>
#include<math.h>
int main()
{
    printf("CHECK:\n");
    FILE * f1;
    FILE * f2;
    f1 = fopen("result.txt","r");
    f2 = fopen("refer-2dim-5h.txt","r");
    int flag=0,x1,x2;
    fscanf(f1,"%d",&x1);
    fscanf(f2,"%d",&x2);
    
    while(fscanf(f1,"%d",&x1)!=EOF&&fscanf(f2,"%d",&x2)!=EOF){       
        if(x1!=x2){		
            flag= 1;
            break;
        }
    }
    if(flag==0) printf("-RIGHT-\n");
    else printf("-WRONG-\n");
    fclose(f1);
    fclose(f2);
}
