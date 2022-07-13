#include<regx52.h> 
#define SIZE 12
#define RS P1_0    //Resgister Selector  , 0 : 寫入指令暫存器 ; 1 : 寫入資料暫存器
#define RW P1_1    //Read/Write   0 : 指令/資料寫入操作 ; 1 : 讀取lcd內部資料
#define E  P1_2    //致能接腳 ， E = 1 時 ，寫入或讀取的指令才有效

unsigned char buf;
unsigned char IsMember;
unsigned char lock;
unsigned char dat [12];
char count;
int i;
int j;

code char string1[]={ "Put your card to" };               //設定LCD顯示字串
code char string2[]={ "the reader." };
code char string3[]={ "ID:" };
code char string4[]={ "Authorize access:" };
code char string5[]={ "Authorize denied:" };
code char member[2][12]={"1B00344A5431","1B00288D10AE"};  //設定Member的ID
int timer,cnt=9990,temp; 	                                
/**** 延遲10us 副程式 ****/
void delay_10us(unsigned int time)
  { while(time>0) time--; }
/**** 寫入指令 副程式 ****/
void write_c(char lcd_data)
  { RS=0;RW=0;E=1;
    P0=lcd_data;
    delay_10us(100);
    E=0;delay_10us(100);
  }
/**** 寫入資料 副程式 ****/
void write_d(char lcd_data)
  { RS=1;RW=0;E=1;
    P0=lcd_data;
    delay_10us(100);
    E=0;delay_10us(100);
  }
/**** LCM初始化 副程式 ****/
void init(void)
  { delay_10us(2000);//延遲20ms
    write_c(0x30);	 //開機啟動   
    write_c(0x30);	 //開機啟動 
	  write_c(0x38);	 //開機啟動 
    write_c(0x08);  // 顯示器 OFF 
    write_c(0x01);  // 清除顯示器 
    write_c(0x06);  // 輸入模式設定 
    write_c(0x0c);  // 顯示器 ON
  }
/**** 顯示位置 副程式 ****/
void disp_p(char row,word)
  { char change_p;
    row--;word--;                  //初始值為0開始 ， 故減1 
    change_p=0x80+(row*0x40+word); 
    write_c(change_p);
  } 
/**** 顯示字串 副程式 ****/
void disp_s(char *ptr)
  { while(*ptr!=0)
      { write_d(*ptr);
        ptr++;
      }
  }

void main (void)
{
    TMOD=0x20;          //設定T1的工作模式,Mode2(8bits傳輸) 自動載入
    TH1=TL1=0xF9;       //設定鮑率為9600
    TR1=1;              //啟動Timer 1
    PCON=0x80;          //SMOD 設為1 搭配TH1=TL1=0xF9,使鮑率為9600
    SCON=0x50;          //設定串列埠為Mode 1 ， 且致能接收腳位
    ES=EA=1;            //設定UART中斷致能
		lock=0;
    IsMember=0;
		count=0;
    init();		          //	LCM初始化 


IDLE:                   //待機顯示畫面
  write_c(0x01);
  disp_p(1,1);          //設定顯示位置 
  disp_s(string1);      //顯示字串1
  disp_p(2,1);	        //設定顯示位置 
  disp_s(string2);      //顯示字串2

	while(1)
	{
    if(lock == 1)
    {
      write_c(0x01);
      disp_p(1,1); //設定顯示位置第1行第1格
      disp_s(string3);//顯示字串3
      disp_p(1,4); //設定顯示位置第1行第4格
      disp_s(dat);//顯示ID
    for(j=0;j<2;j++)    //判斷是否為member
    {
      if(IsMember == 1)
        break;
      for (i=0;i<SIZE;i++)
      {
        if(member[j][i]==dat[i] ) 
          IsMember=1;
        
        else if(member[j][i]!=dat[i])
        {
            IsMember=0;
            break;
        }
      }
    }
    
      disp_p(2,1);	 //設定顯示位置第2行第1格 
      if(IsMember==1)
      {
        disp_s(string4);//顯示字串4
        IsMember=0;
      }
        
      else 
        disp_s(string5);//顯示字串5
      delay_10us(50000);
 
      lock=0;
      goto IDLE;     //已顯示完認證資訊，返回待機畫面
    }
	
	}
	


}
void uart (void) interrupt 4
{
    if(RI==1 && lock == 0)
    {
		  buf=SBUF;              //讀取RFID模組傳回的data

      dat[count]=buf & ~128; //ASCII Code MSB=0
      buf=0;
      count++;
      if(count>=SIZE )
      {
        count=0;
        lock=1;              //接受的byte數量大於12時上鎖(RFID為12碼)
      }
      RI=0;                  //將接收中斷旗標歸零
      
    }
    else
      RI=0; 
}