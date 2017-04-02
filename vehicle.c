#include <regx51.H> 
#define lcd_port P2
sbit vibr=P0^2;
sbit motion=P0^1;
sbit start=P1^6;
sbit motor=P1^7;
sbit buzzer=P1^0;
sbit rs=P1^1;
sbit rw=P1^2;
sbit en=P1^3;
unsigned int k,s;
void delay(unsigned int msec)
{
 unsigned int j;
 for(j=0;j<msec;j++)
 {
  TMOD=0x01;
    TH0=0xFC;
   TL0=0x66;
   TR0=1;
   while(TF0==0);
   TF0=0;
   TR0=0;
 }
}
void lcd_cmd(unsigned char dat)
 {
   rw=0;
   rs=0;
   P2=dat;
   en=1;
   delay(5);
   en=0;
   delay(5);
 }
 void lcd_data(unsigned char dat)
 {
  rs=1;
  rw=0;
  P2=dat;
  en=1;
  delay(5);
  en=0;
  delay(5);
 } 
void lcd_ini(void)
 {
   lcd_cmd(0x38);
   lcd_cmd(0x0c);
   lcd_cmd(0x01);
   lcd_cmd(0x80);
}
void lcd_str(unsigned char *str)
 {
  while(*str!='\0')
  {
   lcd_data(*str);
   str++;
   delay(100);
  }
 }
void serialinit(unsigned int b)
				{SCON=0X050;
				 TMOD|=0X020;
				 TH1=256-(28800/b);
					TR1=1;
					}

/*unsigned char serialrx()
{
unsigned char r;
while(RI==0);
r=SBUF;
RI=0;
return r;
}  */
void serialtx(unsigned char x)
{
serialinit(9600);
  SBUF=x;
 while(TI==0);
 TI=0;
 }
void Send2Gsm(char *aaa)
 {
 unsigned int i;
 for(i=0;aaa[i]!=0;i++)
  {
  serialtx(aaa[i]); 
  }
 }

void main()
{ 	 
lcd_ini();
serialinit(9600);
P2=0x00;

 vibr=0; motion=0; start=1; buzzer=0;	
 
lcd_cmd(0x81);

lcd_str("THIS PROJECT");
lcd_cmd(0xc0);

lcd_str("DONE BY :");
delay(1000);
lcd_cmd(0x01);
lcd_str("EYSHIKA   GAURAV");
lcd_cmd(0xC0);
lcd_str("GOVINDA  AVINASH");
delay(1000);
//lcd_cmd(0x01); 
Send2Gsm("AT\r\n");
delay(2);
Send2Gsm("AT+CMGF=1\r\n");
delay(2);
Send2Gsm("AT+CMGS=\"+919027272248\"\r\n");
delay(2);
Send2Gsm("GSM WORKING ");
serialtx(0x1a);

Send2Gsm("AT+CMGD=1\r\n");
start: delay(2);
	
while(1)
{ 
if(motor==1)
{
  if(start==0 )//&& motor==1)
{
 lcd_cmd(0x01);
  lcd_cmd(0x82);
 lcd_str("car  lock");
 lcd_cmd(0xc0);
 lcd_str("Travelling...");
}
else
{
 lcd_cmd(0x01);
 lcd_cmd(0x82);
 lcd_str("...Travelling...");
} 
}
else if(motor==0)	 //(start==1 &&
{
if(start==1)
{
lcd_cmd(0x01);
lcd_cmd(0x82);
lcd_str(" CAR IS NOT");
lcd_cmd(0xc0);
lcd_str("   LOCKED  ");
Send2Gsm("AT+CMGF=1\r\n");
delay(2);
Send2Gsm("AT+CMGS=\"+919027272248\"\r\n");
delay(2);
Send2Gsm("YOUR CAR IS NOT LOCKED ");
Send2Gsm("\n Please lock it");
serialtx(0x1a);
delay(30000);
}
else if(start==0)
{
 lcd_cmd(0x01);
 lcd_cmd(0x82);
 lcd_str(" CAR IS LOCKED");
/* Send2Gsm("AT+CMGF=1\r\n");
delay(2);
Send2Gsm("AT+CMGS=\"+919027272248\"\r\n");
delay(2);
Send2Gsm("YOUR CAR IS LOCKED AND SAFE NOW");
serialtx(0x1a);
Send2Gsm("AT+CMGD=1\r\n"); 			 
delay(1000);
*/ 	   while(start==0)
{
 if(vibr!=0 || motion!=0)
 {
 buzzer=1;
 motor=0;
    lcd_cmd(0x01);
 lcd_cmd(0x82);
 lcd_str("  CAR  UNSAFE  ");
  Send2Gsm("AT+CMGF=1\r\n");
delay(2);
Send2Gsm("AT+CMGS=\"+919027272248\"\r\n");
delay(2);
Send2Gsm("YOUR CAR IS UNSAFE .");
Send2Gsm("\n PLEASE COME FAST ");
serialtx(0x1a);
delay(9000);
 if(vibr!=0)
 {
  buzzer=1;
  lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_str("someone sitting");
 for(k=0;k<=1;k++)
 {
  Send2Gsm("AT+CMGF=1\r\n");
delay(2);
Send2Gsm("AT+CMGS=\"+919027272248\"\r\n");
delay(2);
Send2Gsm("SOMEONE IS SITTING IN YOUR CAR .");
Send2Gsm("\n LEAVE YOUR WORK AND COME FAST ");
serialtx(0x1a);
delay(10000);
if(k==1)
 goto start;
 else if(motion!=0 || motor==1)
 break;
}
  }
else if(motor==1)
{
 buzzer=1;
    lcd_cmd(0x01);
 lcd_cmd(0x80);
 lcd_str("TRYING TO START");
 Send2Gsm("AT+CMGF=1\r\n");
delay(2);
Send2Gsm("AT+CMGS=\"+919027272248\"\r\n");
delay(2);
Send2Gsm("SOMEONE IS TRYING TO START YOUR CAR.");
Send2Gsm("\n PLEASE COME FAST ");
serialtx(0x1a);
delay(10000);
motor=0;
}
else
{
  buzzer=1;
    lcd_cmd(0x01);
 lcd_cmd(0x82);
 lcd_str("someone step in");
 for(k=0;k<=2;k++)
 {
  Send2Gsm("AT+CMGF=1\r\n");
delay(2);
Send2Gsm("AT+CMGS=\"+919027272248\"\r\n");
delay(2);
Send2Gsm("SOMEONE IS IN THE CAR .");
Send2Gsm("\n PLEASE COME FAST ");
serialtx(0x1a);
delay(10000);
if(k==2)
goto start;
}
}
}
}
}
 }
}
 }
