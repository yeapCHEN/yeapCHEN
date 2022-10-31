#include "snake.h"
#include "tftlcd.h"
#include "picture.h"
#include "ascii.h"

u8 key;


void LCD_ShowPictureEx(u16 x, u16 y, u16 wide, u16 high)
{
	u16 temp = 0;
	long tmp=0,num=0;
	LCD_Set_Window(x, y, x+wide-1, y+high-1);
	num = wide * high*2 ;
	do
	{  
		temp = pic[tmp + 1];
		temp = temp << 8;
		temp = temp | pic[tmp];
		LCD_WriteData_Color(temp);//�����ʾ
		tmp += 2;
	}
	while(tmp < num);	
}

//��ָ��λ�� ��ʾ1��16*16�ĺ���
//(x,y):������ʾ��λ��
//index:tfont��������ĵڼ�������
//color:������ֵ���ɫ
void Test_Show_CH_Font16(u16 x,u16 y,u8 index,u16 color)
{   			    
	u8 temp,t,t1;
	u16 y0=y;				   
    for(t=0;t<32;t++)//ÿ��16*16�ĺ��ֵ��� ��32���ֽ�
    {   
		if(t<16)temp=tfont16[index*2][t];      //ǰ16���ֽ�
		else temp=tfont16[index*2+1][t-16];    //��16���ֽ�	                          
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_DrawFRONT_COLOR(x,y,color);//��ʵ�ĵ�
			else LCD_DrawFRONT_COLOR(x,y,GBLUE);   //���հ׵㣨ʹ�ñ���ɫ��
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}

//��ָ��λ�� ��ʾ1��24*24�ĺ���
//(x,y):������ʾ��λ��
//index:tfont��������ĵڼ�������
//color:������ֵ���ɫ
void Test_Show_CH_Font24(u16 x,u16 y,u8 index,u16 color)
{   			    
	u8 temp,t,t1;
	u16 y0=y;				   
    for(t=0;t<72;t++)//ÿ��24*24�ĺ��ֵ��� ��72���ֽ�
    {   
		if(t<24)temp=tfont24[index*3][t];           //ǰ24���ֽ�
		else if(t<48)temp=tfont24[index*3+1][t-24]; //��24���ֽ�	                          
        else temp=tfont24[index*3+2][t-48];         //��24���ֽ�
	    for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)LCD_DrawFRONT_COLOR(x,y,color);//��ʵ�ĵ�
			else LCD_DrawFRONT_COLOR(x,y,GRAY);   //���հ׵㣨ʹ�ñ���ɫ��
			temp<<=1;
			y++;
			if((y-y0)==24)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}


//��Ϸ����
//һ��12�����֣��ɼ����ؿ����ϣ��£����ң���ʼ����ͣ
void show(void)
{
	//��Χǽ ���Ϊ5
	//����һ��((150/5)*(230/5))=(30*46)����Ϸ���� ǽ�ں�Ϊ5 ����Ϊ5
  	
	LCD_DrawLine_Color(0, 161, 240, 161,BLACK);
	LCD_DrawLine_Color(240, 0, 240, 161,BLACK);

	
	//�ɼ�
	Test_Show_CH_Font16(0,165,0,RED);
	Test_Show_CH_Font16(16,165,1,RED);
	FRONT_COLOR=RED;
	BACK_COLOR=GRAY;
	LCD_ShowString(32,165,tftlcd_data.width,tftlcd_data.height,16,":");
	LCD_ShowString(40,165,tftlcd_data.width,tftlcd_data.height,16,"  0");	
	//����
	Test_Show_CH_Font16(184,165,2,RED);
	Test_Show_CH_Font16(200,165,3,RED);
	LCD_ShowString(216,165,tftlcd_data.width,tftlcd_data.height,16,":");
	LCD_ShowString(224,165,tftlcd_data.width,tftlcd_data.height,16," ");	
	
	//��Ϸ����
	LCD_ShowString(98,237,tftlcd_data.width,tftlcd_data.height,16,"Snaker");

	LCD_ShowString(10,320,tftlcd_data.width,tftlcd_data.height,16,"K_UP:    UP");
	LCD_ShowString(10,340,tftlcd_data.width,tftlcd_data.height,16,"K_DOWN:  DOWN");
	LCD_ShowString(10,360,tftlcd_data.width,tftlcd_data.height,16,"K_LEFT:  LEFT");
	LCD_ShowString(10,380,tftlcd_data.width,tftlcd_data.height,16,"K_RIGHT: RIGHT");
}
