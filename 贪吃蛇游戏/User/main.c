#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "usart.h"
#include "tftlcd.h"
#include "time.h"
#include "key.h"
#include "touch.h"
#include "snake.h"
#include "rtc.h"
#include "stdlib.h"


#define SNAKE_Max_Long 50//�ߵ���󳤶� 
u8 pause=0;
u8 start=0;

//�߽ṹ��
struct Snake
{
	s16 X[SNAKE_Max_Long];
	s16 Y[SNAKE_Max_Long];
	u8 Long;//�ߵĳ���
	u8 Life;//�ߵ����� 0���� 1����
	u8 Direction;//���ƶ��ķ���
}snake;

//ʳ��ṹ��
struct Food
{
	u8 X;//ʳ�������
	u8 Y;//ʳ��������
	u8 Yes;//�ж��Ƿ�Ҫ����ʳ��ı��� 0��ʳ�� 1��Ҫ����ʳ��
}food;

//��Ϸ�ȼ�����
struct Game
{
	u16 Score;//����
	u8 Life;//��Ϸ�ȼ�	
}game;

void touch(void)
{	
	key=KEY_Scan(0);	
	
	if(key==KEY_UP&&snake.Direction!=4)		//��
	{
		snake.Direction=3;
	} 
	if(key==KEY_DOWN&&snake.Direction!=3)	   //��
	{
		snake.Direction=4;
	} 
	if(key==KEY_LEFT&&snake.Direction!=1)	   //��
	{
		snake.Direction=2;
	} 
	if(key==KEY_RIGHT&&snake.Direction!=2)	  //��
	{
		snake.Direction=1;
	}
}
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)
	{
		touch();
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	
	}
}  


//��Ϸ����
void gameover()
{
	start=0;//ֹͣ��Ϸ
	Test_Show_CH_Font24(80,65,0,RED);		
	Test_Show_CH_Font24(104,65,1,RED);		
	Test_Show_CH_Font24(128,65,2,RED);		
	Test_Show_CH_Font24(152,65,3,RED);		
	Test_Show_CH_Font24(176,65,4,RED);		
	FRONT_COLOR=BLACK;
	BACK_COLOR=GRAY;
	LCD_ShowString(224,165,tftlcd_data.width,tftlcd_data.height,16,"0");//��ʾ����ֵ	
}

//����Ϸ
void play()
{
	u16 i,n;//i�ߵĹؽ��� n�����ж�ʳ����ߵ������Ƿ��غ�
	u8 life_buf[2];
	u8 socre_buf[4];
	snake.Long=2;//�����ߵĳ���
	snake.Life=0;//�߻�����
	snake.Direction=1;//�ߵ���ʼ������Ϊ��
	game.Score=0;//����Ϊ0
	game.Life=4;//�ߵ�����ֵ
	food.Yes=1;//������ʳ��
	snake.X[0]=12;snake.Y[0]=24;
	snake.X[1]=12;snake.Y[1]=24;

	while(1)
	{
			if(food.Yes==1)//�����µ�ʳ��
			{
				while(1)
				{
						//���趨����������ʾʳ��		
						//food.X=12+rand()%(240/12)*12;
						//food.Y=12+rand()%(160/12)*12;
						srand(calendar.sec);//���������� ���õ�RTCʱ��
						food.X=12+rand()%(228/12)*12;
						food.Y=12+rand()%(148/12)*12;
						for(n=0;n<snake.Long;n++)
						{
							if(food.X==snake.X[n]&&food.Y==snake.Y[n])
								break;
						}
						if(n==snake.Long)
						food.Yes=0;	
						break;
					}
			}
				
				if(food.Yes==0)//��ʳ���Ҫ��ʾ
				{	
					LCD_Fill(food.X,food.Y,food.X+10,food.Y+10,RED);
				}
				//ȡ����Ҫ���»����ߵĽ���
				for(i=snake.Long-1;i>0;i--)
				{
					snake.X[i]=snake.X[i-1];
					snake.Y[i]=snake.Y[i-1];
				}
				//ͨ�������������ߵ��˶�����
				switch(snake.Direction)
				{
					case 1:snake.X[0]+=12;break;//�����˶�
					case 2:snake.X[0]-=12;break;//�����˶�
					case 3:snake.Y[0]-=12;break;//�����˶�
					case 4:snake.Y[0]+=12;break;//�����˶�
				}
				for(i=0;i<snake.Long;i++)//������	
				LCD_Fill(snake.X[i],snake.Y[i],snake.X[i]+10,snake.Y[i]+10,BLUE);//��������
				while(pause==1){};
				delay_ms(500);//��ʱ
				LCD_Fill(snake.X[snake.Long-1],snake.Y[snake.Long-1],snake.X[snake.Long-1]+10,snake.Y[snake.Long-1]+10,GRAY);//��������		
						
					
				//�ж��Ƿ�ײǽ
				if(snake.X[0]<0||snake.X[0]>240||snake.Y[0]<0||snake.Y[0]>150)
					snake.Life=1;//��������
		
				//���ߵ����峬��3�ں��ж����������ײ
				for(i=3;i<snake.Long;i++)
				{
					if(snake.X[i]==snake.X[0]&&snake.Y[i]==snake.Y[0])//�������һ����ֵ����ͷ������Ⱦ���Ϊ��������ײ
						game.Life-=1;
				}
				if(snake.Life==1||game.Life==0)//���������ж��Ժ������������������ѭ�������¿�ʼ
				{
					gameover();
					break;
				}	
				//�ж����Ƿ�Ե���ʳ��
				if(snake.X[0]==food.X&&snake.Y[0]==food.Y)
				{
					LCD_Fill(food.X,food.Y,food.X+10,food.Y+10,GRAY);//�ѳԵ���ʳ������
					if(!((snake.Long==SNAKE_Max_Long)&&(snake.Long==SNAKE_Max_Long)))
					snake.Long++;//�ߵ����峤һ��
					game.Score+=10;
					socre_buf[0]=game.Score/100+0x30;
					socre_buf[1]=game.Score%100/10+0x30;
					socre_buf[2]=game.Score%100%10+0x30;
					socre_buf[3]='\0';
					FRONT_COLOR=BLACK;
					BACK_COLOR=GRAY;
		
					LCD_ShowString(40,165,tftlcd_data.width,tftlcd_data.height,16,socre_buf);//��ʾ�ɼ�	
					food.Yes=1;//��Ҫ������ʾʳ��
				}
				life_buf[0]=game.Life%10+0x30;
				life_buf[1]='\0';
			
				LCD_ShowString(224,165,tftlcd_data.width,tftlcd_data.height,16,life_buf);//��ʾ����ֵ	
		}	
}

int main()
{
	u8 i;
	
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	USART1_Init(9600);
	TFTLCD_Init();			//LCD��ʼ��
	KEY_Init();
	
	TOUCH_Init();
	LCD_ShowPictureEx(0, 0, 240, 400); 
	while(TOUCH_Scan() == 0xff); //�ȴ����´���
	TIM3_Init(50,7199);//������ʱ��
	RTC_Init();
	LCD_Clear(GRAY);
	show();//����Ϸ����
	play();//����Ϸ
}
