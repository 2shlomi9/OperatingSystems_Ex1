#include <stdio.h>
#include <math.h>

int main(){
    double a,b,c;
    printf("Enter 3 numbers :");
    scanf("%lf,%lf,%lf",&a,&b,&c);

    double edge1 = a*a+b*b;
    double edge2 = a*a+c*c;
    double edge3 = b*b+c*c;
    if(edge1 == c*c || edge2 == b*b || edge3 == a*a){

        double angle1 = asin(a/c);
        double angle2 = asin(b/c);
        double angle3 = asin(c/c);
        
        printf("The angles (in radians) A = %.4f , B = %.4f , C = %.4f " ,angle1,angle2,angle3);
    }
    else{
        printf("Error - Not a pythagorean trinangle !");
    }
    return 0;
}