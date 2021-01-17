int main() {
int a,s;
a = 10;
s = 0;
char ch;
scanf("%c", &ch);
printf("%c", ch);
while(a>0 && a<=10 || a%100==10) {
a -= 1;
//a = 10;
s += a;
if(s > -10) {
printf("result is: %d\n", s);
int b;
b = 10;
int i;
for(i=0; i<b; i=i+1) {
printf("Have fun: %d\n", i);
}
}
}
return 1;
}
// No more compilation err