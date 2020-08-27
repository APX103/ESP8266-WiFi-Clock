#include "driver/lcdfont.h"
#include "driver/lcd_init.h"
#include "driver/lcd.h"

#define SA 		 0
#define SB 		 1
#define SC 		 2
#define SD 		 3
#define SE 		 4
#define SF 		 5
#define SG 		 6

#define SEG_WIDTH     		6       //�ο�
#define SEG_HEIGHT    		6       //�θ�
#define ANIM_SPEED			17

u8 locx_now = 1;  					//ȡʵ�����ص��ֵ
u8 locy_now = 5;					//ȡʵ�����ص��ֵ
u16 segColor = WHITE;				//�洢�ε���ɫ
u8 block_len = 3;					//��ĳ���

u8 locx[7] = {0, 5, 32, 63, 90, 119, 138};

const u8 digitBits[] = {
  0xFC, //B11111100, 0,  ABCDEF--
  0x60, //B01100000, 1,  -BC-----
  0xDA, //B11011010, 2,  AB-DE-G-
  0xF2, //B11110010, 3,  ABCD--G-
  0x66, //B01100110, 4,  -BC--FG-
  0xB6, //B10110110, 5,  A-CD-FG-
  0xBE, //B10111110, 6,  A-CDEFG-
  0xE0, //B11100000, 7,  ABC-----
  0xFE, //B11111110, 8,  ABCDEFG-
  0xF6, //B11110110, 9,  ABCD_FG-
};

void ICACHE_FLASH_ATTR DIGIT_DrawDot(void)
{
	LCD_Fill(58, 41, 61, 44, segColor);
	LCD_Fill(58, 59, 61, 62, segColor);
}

//��һ�����úõ����ؿ飬y����ʾ����2�У�
void ICACHE_FLASH_ATTR DIGIT_DrawPixelBlock(u16 x, u16 y, u16 color)	//x��y �����
{
	LCD_Fill(locx_now+x, y+locy_now, locx_now+x+block_len, y+block_len+locy_now, color);		//ʵ�ʵ�
}

//�������úõ����ؿ黭��
void ICACHE_FLASH_ATTR DIGIT_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color)	//x��y�� �����
{
	int i;
	if (x1 == x2)
	{
		/*
		 * ������:��y1�������y2Сʱ,�����κ�����ʼλ����(x1,y1);
		 * ��y1�������y2��ʱ,�����κ�����ʼλ����(x1,y2).
		 */
		if (y1 <= y2)
			for (i  = 0; i <=  y2 - y1; i++)
				DIGIT_DrawPixelBlock(x1*block_len, (y1+i)*block_len, color);	//������ʼ��, һ���ŵ�
		else if (y1 > y2)
			for (i  = 0; i <=  y1 - y2; i++)
				DIGIT_DrawPixelBlock(x1*block_len, (y2+i)*block_len, color);
	} else if (y1 == y2) {
		/*
		 * ������:��x1�������x2Сʱ,�����κ�����ʼλ����(x1,y1);
		 * ��x1�������x2��ʱ,�����κ�����ʼλ����(x2,y1).
		 */
		if (x1 <= x2)
			for (i  = 0; i <=  x2 - x1; i++)
				DIGIT_DrawPixelBlock((x1+i)*block_len, y1*block_len, color);
		else if (x1 > x2)
			for (i  = 0; i <=  x1 - x2; i++)
				DIGIT_DrawPixelBlock((x2+i)*block_len, y1*block_len, color);
	}
}

//����1�䵽2
void ICACHE_FLASH_ATTR DIGIT_Morph2(void)
{
	int i;
	for (i = 0; i <= SEG_WIDTH; i++)
	{
		if (i < SEG_WIDTH)
		{
			DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, 0, segColor);  //��A
			DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, (SEG_HEIGHT + 1)*block_len, segColor);   //��G
			DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, (2*SEG_HEIGHT + 2)*block_len, segColor); //��D
		}
		//��ƽ��E
		DIGIT_DrawLine(SEG_WIDTH - i + 1, SEG_HEIGHT + 2, SEG_WIDTH - i + 1, 2*SEG_HEIGHT + 1, SKYBLUE);
		DIGIT_DrawLine(SEG_WIDTH - i, SEG_HEIGHT + 2, SEG_WIDTH - i, 2*SEG_HEIGHT + 1, segColor);
		delay_ms(ANIM_SPEED);
	}
}

//����2�䵽3
void ICACHE_FLASH_ATTR DIGIT_Morph3(void)
{
	int i;
	for (i = 0; i <= SEG_WIDTH; i++)
	{
		//��ƽ��E
		DIGIT_DrawLine(i, SEG_HEIGHT + 2, i, 2*SEG_HEIGHT + 1, SKYBLUE);
		DIGIT_DrawLine(i + 1, SEG_HEIGHT + 2, i + 1, 2*SEG_HEIGHT + 1, segColor);
		delay_ms(ANIM_SPEED);
	}
}

void ICACHE_FLASH_ATTR DIGIT_Morph4(void)
{
	int i;
	for (i = 0; i < SEG_WIDTH; i++)
	{
		DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, 0, SKYBLUE);  					//��A
		DIGIT_DrawPixelBlock(0, (1 + i)*block_len, segColor);							//��F
		DIGIT_DrawPixelBlock((1 + i)*block_len, (2*SEG_HEIGHT + 2)*block_len, SKYBLUE); 	//��D
		delay_ms(ANIM_SPEED);
	}
}

void ICACHE_FLASH_ATTR DIGIT_Morph5(void)
{
	int i;
	for (i = 0; i < SEG_WIDTH; i++)
	{
		DIGIT_DrawPixelBlock((SEG_WIDTH + 1)*block_len, (SEG_HEIGHT - i)*block_len, SKYBLUE);  	//��B
		DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, 0, segColor); 				 			//��A
		DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, (2*SEG_HEIGHT + 2)*block_len, segColor);//��D
		delay_ms(ANIM_SPEED);
	}
}

void ICACHE_FLASH_ATTR DIGIT_Morph6(void)
{
	int i;
	for (i = 0; i <= SEG_WIDTH; i++)
	{
		//��ƽ��C
		if (i > 0)
			DIGIT_DrawLine(SEG_WIDTH - i + 1, SEG_HEIGHT + 2, SEG_WIDTH - i + 1, 2*SEG_HEIGHT + 1, SKYBLUE);
		DIGIT_DrawLine(SEG_WIDTH - i, SEG_HEIGHT + 2, SEG_WIDTH - i, 2*SEG_HEIGHT + 1, segColor);
		delay_ms(ANIM_SPEED);
	}
}

void ICACHE_FLASH_ATTR DIGIT_Morph7(void)
{
	int i;
	for (i = 0; i <= SEG_WIDTH; i++)
	{
		//��ƽ��B
		DIGIT_DrawLine(i, 1, i, SEG_HEIGHT, SKYBLUE);
		DIGIT_DrawLine(i + 1, 1, i + 1, SEG_HEIGHT, segColor);

		//��ƽ��E
		DIGIT_DrawLine(i, SEG_HEIGHT + 2, i, 2*SEG_HEIGHT + 1, SKYBLUE);
		DIGIT_DrawLine(i + 1, SEG_HEIGHT + 2, i + 1, 2*SEG_HEIGHT + 1, segColor);

		DIGIT_DrawPixelBlock((1 + i)*block_len, (SEG_HEIGHT + 1)*block_len, SKYBLUE);  	//��G
		DIGIT_DrawPixelBlock((1 + i)*block_len, (2*SEG_HEIGHT + 2)*block_len, SKYBLUE);   //��D

		delay_ms(ANIM_SPEED);
	}
}

void ICACHE_FLASH_ATTR DIGIT_Morph8(void)
{
	int i;
	for (i = 0; i <= SEG_WIDTH; i++)
	{
		//��ƽ��B
		if (i > 0)
			DIGIT_DrawLine(SEG_WIDTH - i + 1, 1, SEG_WIDTH - i + 1, SEG_HEIGHT, SKYBLUE);
		DIGIT_DrawLine(SEG_WIDTH - i, 1, SEG_WIDTH - i, SEG_HEIGHT, segColor);

		//��ƽ��C
		if (i > 0)
			DIGIT_DrawLine(SEG_WIDTH - i + 1, SEG_HEIGHT + 2, SEG_WIDTH - i + 1, 2*SEG_HEIGHT + 1, SKYBLUE);
		DIGIT_DrawLine(SEG_WIDTH - i, SEG_HEIGHT + 2, SEG_WIDTH - i, 2*SEG_HEIGHT + 1, segColor);

		if (i < SEG_WIDTH)
		{
			DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, (SEG_HEIGHT + 1)*block_len, segColor); 	 //��G
			DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, (2*SEG_HEIGHT + 2)*block_len, segColor); //��D
		}
		delay_ms(ANIM_SPEED);
	}
}

void ICACHE_FLASH_ATTR DIGIT_Morph9(void)
{
	int i;
	for (i = 0; i <= SEG_WIDTH; i++)
	{
		//��ƽ��E
		DIGIT_DrawLine(i, SEG_HEIGHT + 2, i, 2*SEG_HEIGHT + 1, SKYBLUE);
		DIGIT_DrawLine(i + 1, SEG_HEIGHT + 2, i + 1, 2*SEG_HEIGHT + 1, segColor);
		delay_ms(ANIM_SPEED);
	}
}

//_valueΪ��һ�ε�����ֵ�������ϴε�����ֵ�������˴εĶ���Ч��
void ICACHE_FLASH_ATTR DIGIT_Morph0(u8 _value)
{
	int i;
	for (i = 0; i <= SEG_WIDTH; i++)
	{
		if (_value == 1)
		{
			//��ƽ��B
			if (i > 0)
				DIGIT_DrawLine(SEG_WIDTH - i + 1, 1, SEG_WIDTH - i + 1, SEG_HEIGHT, SKYBLUE);
			DIGIT_DrawLine(SEG_WIDTH - i, 1, SEG_WIDTH - i, SEG_HEIGHT, segColor);

			//��ƽ��C
			if (i > 0)
				DIGIT_DrawLine(SEG_WIDTH - i + 1, SEG_HEIGHT + 2, SEG_WIDTH - i + 1, 2*SEG_HEIGHT + 1, SKYBLUE);
			DIGIT_DrawLine(SEG_WIDTH - i, SEG_HEIGHT + 2, SEG_WIDTH - i, 2*SEG_HEIGHT + 1, segColor);

			if (i < SEG_WIDTH)
			{
				DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, 0, segColor); 							//��A
				DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, (2*SEG_HEIGHT + 2)*block_len, segColor);//��D
			}
		}

		if (_value == 2)
		{
			//��ƽ��B
			if (i > 0)
				DIGIT_DrawLine(SEG_WIDTH - i + 1, 1, SEG_WIDTH - i + 1, SEG_HEIGHT, SKYBLUE);
			DIGIT_DrawLine(SEG_WIDTH - i, 1, SEG_WIDTH - i, SEG_HEIGHT, segColor);

			//��ƽ��E
			if (i > 0)
				DIGIT_DrawLine(i, SEG_HEIGHT + 2, i, 2*SEG_HEIGHT + 1, SKYBLUE);
			DIGIT_DrawLine(i + 1, SEG_HEIGHT + 2, i + 1, 2*SEG_HEIGHT + 1, segColor);

			if (i < SEG_WIDTH)
			{
				DIGIT_DrawPixelBlock((i + 1)*block_len, (SEG_HEIGHT + 1)*block_len, SKYBLUE); //��G
			}
		}

		if (_value == 3)
		{
			//��ƽ��B
			if (i > 0)
				DIGIT_DrawLine(SEG_WIDTH - i + 1, 1, SEG_WIDTH - i + 1, SEG_HEIGHT, SKYBLUE);
			DIGIT_DrawLine(SEG_WIDTH - i, 1, SEG_WIDTH - i, SEG_HEIGHT, segColor);

			//��ƽ��C
			if (i > 0)
				DIGIT_DrawLine(SEG_WIDTH - i + 1, SEG_HEIGHT + 2, SEG_WIDTH - i + 1, 2*SEG_HEIGHT + 1, SKYBLUE);
			DIGIT_DrawLine(SEG_WIDTH - i, SEG_HEIGHT + 2, SEG_WIDTH - i, 2*SEG_HEIGHT + 1, segColor);

			if (i < SEG_WIDTH)
			{
				DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, (SEG_HEIGHT + 1)*block_len, SKYBLUE);  //��G
			}
		}

		if (_value == 5)
		{
			if (i <= SEG_WIDTH)
			{
				//��ƽ��F
				if (i > 0)
					DIGIT_DrawLine(i, 1, i, SEG_HEIGHT, SKYBLUE);
				DIGIT_DrawLine(i + 1, 1, i + 1, SEG_HEIGHT, segColor);
			}
		}

		if (_value == 5 || _value == 9)
		{
			if (i < SEG_WIDTH)
			{
				DIGIT_DrawPixelBlock((SEG_WIDTH - i)*block_len, (SEG_HEIGHT + 1)*block_len, SKYBLUE); //��G
				DIGIT_DrawPixelBlock(0, (SEG_HEIGHT + 2 + i)*block_len, segColor); 		//��E
			}
		}
		delay_ms(ANIM_SPEED);
	}
}

void ICACHE_FLASH_ATTR DIGIT_Morph1(void)
{
	int i;
	for (i = 0; i <= SEG_WIDTH; i++)
	{
		//��ƽ��F
		DIGIT_DrawLine(i, 1, i, SEG_HEIGHT, SKYBLUE);
		DIGIT_DrawLine(i + 1, 1, i + 1, SEG_HEIGHT, segColor);

		//��ƽ��E
		DIGIT_DrawLine(i, SEG_HEIGHT + 2, i, 2*SEG_HEIGHT + 1, SKYBLUE);
		DIGIT_DrawLine(i + 1, SEG_HEIGHT + 2, i + 1, 2*SEG_HEIGHT + 1, segColor);

		if (i < SEG_WIDTH)
		{
			DIGIT_DrawPixelBlock((i + 1)*block_len, 0, SKYBLUE);  			   				//��A
			DIGIT_DrawPixelBlock((i + 1)*block_len, (SEG_HEIGHT + 1)*block_len, SKYBLUE);   //��G
			DIGIT_DrawPixelBlock((i + 1)*block_len, (2*SEG_HEIGHT + 2)*block_len, SKYBLUE); //��D
		}
		delay_ms(ANIM_SPEED);
	}
}

//�����ֵ�ĳһ�Σ����ַ�Ϊ7�Σ�segΪ�κ�
void ICACHE_FLASH_ATTR DIGIT_DrawSeg(u8 seg)
{
	switch (seg)
	{
		case SA: DIGIT_DrawLine(1, 0, SEG_WIDTH, 0, segColor); break;
		case SB: DIGIT_DrawLine(SEG_WIDTH+1, 1, SEG_WIDTH+1, SEG_HEIGHT, segColor); break;
		case SC: DIGIT_DrawLine(SEG_WIDTH+1, SEG_HEIGHT+2, SEG_WIDTH+1, 2*SEG_HEIGHT+1, segColor); break;
		case SD: DIGIT_DrawLine(1, 2*SEG_HEIGHT+2, SEG_WIDTH, 2*SEG_HEIGHT+2, segColor); break;
		case SE: DIGIT_DrawLine(0, SEG_HEIGHT+2, 0, 2*SEG_HEIGHT+1, segColor); break;
		case SF: DIGIT_DrawLine(0, 1, 0, SEG_HEIGHT, segColor); break;
		case SG: DIGIT_DrawLine(1, SEG_HEIGHT+1, SEG_WIDTH, SEG_HEIGHT+1, segColor); break;
		default: break;
	}
}

//������
void ICACHE_FLASH_ATTR DIGIT_DrawDigit(u8 num)  //locx����ʼλ��
{
	u8 value;
	if (num > 9)  //��������Χ֮�����˳�
		return;
	value = digitBits[num];
	if (value & 0x80)	DIGIT_DrawSeg(SA);
	if (value & 0x40)	DIGIT_DrawSeg(SB);
	if (value & 0x20)	DIGIT_DrawSeg(SC);
	if (value & 0x10)	DIGIT_DrawSeg(SD);
	if (value & 0x08)	DIGIT_DrawSeg(SE);
	if (value & 0x04)	DIGIT_DrawSeg(SF);
	if (value & 0x02)	DIGIT_DrawSeg(SG);
}

/* ����: ���ֱ任
 * ����: value->��ǰ������ֵ
 *		 _value->�ϴε�����ֵ
 * ����ֵ: none
 */
void ICACHE_FLASH_ATTR DIGIT_Morph(u8 value, u8 _value)
{
	switch (value)
	{
		case 0: DIGIT_Morph0(_value); break;
		case 1: DIGIT_Morph1(); break;
		case 2: DIGIT_Morph2(); break;
		case 3: DIGIT_Morph3(); break;
		case 4: DIGIT_Morph4(); break;
		case 5: DIGIT_Morph5(); break;
		case 6: DIGIT_Morph6(); break;
		case 7: DIGIT_Morph7(); break;
		case 8: DIGIT_Morph8(); break;
		case 9: DIGIT_Morph9(); break;
	}
}

void ICACHE_FLASH_ATTR DIGIT_DrawStartTime(u8 hour, u8 minute, u8 second)
{
	block_len = 2;
	locx_now = locx[6];			 //��λ
	locy_now = 44;
	DIGIT_DrawDigit(second%10);  //����ĸ�λ
	locx_now = locx[5];			 //��λ
	DIGIT_DrawDigit(second/10);  //�����ʮλ

	block_len = 3;
	locx_now = locx[4];			 //��λ
	locy_now = 30;
	DIGIT_DrawDigit(minute%10);  //���ֵĸ�λ
	locx_now = locx[3];		 	 //��λ
	DIGIT_DrawDigit(minute/10);  //���ֵ�ʮλ
	locx_now = locx[2];			 //��λ
	DIGIT_DrawDigit(hour%10);    //��ʱ�ĸ�λ
	locx_now = locx[1];			 //��λ
	DIGIT_DrawDigit(hour/10);    //��ʱ��ʮλ
}

/******************************************************************************
      ����˵������ָ�����������ɫ
      ������ݣ�xsta,ysta   ��ʼ����
                xend,yend   ��ֹ����
								color       Ҫ������ɫ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_Fill(u16 xsta,u16 ysta,u16 xend,u16 yend,u16 color)
{
	u16 i, j;
	LCD_Address_Set(xsta, ysta, xend-1, yend-1);//������ʾ��Χ��������(xend,yend)������ʾ������
	for(i = ysta; i < yend; i++)
	{
		for(j = xsta; j < xend; j++)
		{
			LCD_WR_DATA(color);
		}
	}
}

/******************************************************************************
      ����˵������ָ��λ�û���
      ������ݣ�x,y ��������
                color �����ɫ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//���ù��λ��
	LCD_WR_DATA(color);
}


/******************************************************************************
      ����˵��������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   �ߵ���ɫ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_DrawLine(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1;
	uRow=x1;//�����������
	uCol=y1;
	if(delta_x>0)incx=1; //���õ������� 
	else if (delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ˮƽ�� 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		LCD_DrawPoint(uRow,uCol,color);//����
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}


/******************************************************************************
      ����˵����������
      ������ݣ�x1,y1   ��ʼ����
                x2,y2   ��ֹ����
                color   ���ε���ɫ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}


/******************************************************************************
      ����˵������Բ
      ������ݣ�x0,y0   Բ������
                r       �뾶
                color   Բ����ɫ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a,b;
	a=0;b=r;	  
	while(a<=b)
	{
		LCD_DrawPoint(x0-b,y0-a,color);             //3           
		LCD_DrawPoint(x0+b,y0-a,color);             //0           
		LCD_DrawPoint(x0-a,y0+b,color);             //1                
		LCD_DrawPoint(x0-a,y0-b,color);             //2             
		LCD_DrawPoint(x0+b,y0+a,color);             //4               
		LCD_DrawPoint(x0+a,y0-b,color);             //5
		LCD_DrawPoint(x0+a,y0+b,color);             //6 
		LCD_DrawPoint(x0-b,y0+a,color);             //7
		a++;
		if((a*a+b*b)>(r*r))//�ж�Ҫ���ĵ��Ƿ��Զ
		{
			b--;
		}
	}
}

/******************************************************************************
      ����˵������ʾ���ִ�
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ��ִ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ� ��ѡ 16 24 32
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==16) LCD_ShowChinese16x16(x,y,s,fc,bc,sizey,mode);
		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

/******************************************************************************
      ����˵������ʾ����16x16����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_ShowChinese16x16(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
					}
					else//���ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 


/******************************************************************************
      ����˵������ʾ����24x24����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_ShowChinese24x24(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
					}
					else//���ӷ�ʽ
					{
						if(tfont24[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
} 

/******************************************************************************
      ����˵������ʾ����32x32����
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_ShowChinese32x32(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 i,j;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{ 	
			LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
					if(!mode)//�ǵ��ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))LCD_WR_DATA(fc);
						else LCD_WR_DATA(bc);
					}
					else//���ӷ�ʽ
					{
						if(tfont32[k].Msk[i]&(0x01<<j))	LCD_DrawPoint(x,y,fc);//��һ����
						x++;
						if((x-x0)==sizey)
						{
							x=x0;
							y++;
							break;
						}
					}
				}
			}
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}
}


/******************************************************************************
      ����˵������ʾ�����ַ�
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ���ַ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp,sizex,t;
	u16 i, TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0 = x;
	sizex=sizey/2;
	TypefaceNum=sizex/8*sizey;
	num=num-' ';    //�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x,y,x+sizex-1,y+sizey-1);  //���ù��λ�� 
	for(i=0;i<TypefaceNum;i++)
	{ 
		if(sizey==16)
			temp = ascii_1608[num][i];		       //����8x16����
		else if(sizey==32)
			temp = ascii_3216[num][i];		 //����16x32����
		else return;
		for(t=0;t<8;t++)
		{
			if(!mode)//�ǵ���ģʽ
			{
				if(temp&(0x01<<t))LCD_WR_DATA(fc);
				else LCD_WR_DATA(bc);
			}
			else//����ģʽ
			{
				if(temp&(0x01<<t))LCD_DrawPoint(x,y,fc);//��һ����
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y++;
					break;
				}
			}
		}
	}   	 	  
}

//ֻ������ʾ24x12����
void ICACHE_FLASH_ATTR MY_LCD_ShowChar(u16 x,u16 y,u8 num,u16 fc,u16 bc,u8 sizey,u8 mode)
{
	u8 temp, sizex, t, i;
	sizex = sizey/2 + 4;	//x������Ϊ12+4=16
	num=num-' ';    		//�õ�ƫ�ƺ��ֵ
	LCD_Address_Set(x, y, x+sizex-1, y+sizey-1);  //���ù��λ��
	for(i = 0; i < 48; i++)			//��ģ��48���ֽ�
	{
		temp = ascii_2412[num][i];	//����24x12����
		for(t = 0; t < 8; t++)
		{
			if(temp&(0x01<<t))
				LCD_WR_DATA(fc);
			else
				LCD_WR_DATA(bc);
		}
	}
}

/******************************************************************************
      ����˵������ʾ�ַ���
      ������ݣ�x,y��ʾ����
                *p Ҫ��ʾ���ַ���
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_ShowString(u16 x,u16 y,const u8 *p,u16 fc,u16 bc,u8 sizey,u8 mode)
{         
	while(*p != '\0')
	{       
//		LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		MY_LCD_ShowChar(x,y,*p,fc,bc,sizey,mode);
		x += sizey / 2;
		p++;
	}  
}


/******************************************************************************
      ����˵������ʾ����
      ������ݣ�m������nָ��
      ����ֵ��  ��
******************************************************************************/
u32 ICACHE_FLASH_ATTR mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;
	return result;
}


/******************************************************************************
      ����˵������ʾ��������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾ��������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_ShowIntNum(u16 x,u16 y,u16 num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp;
	u8 enshow=0;
	u8 sizex=sizey/2;
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+t*sizex,y,' ',fc,bc,sizey,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
} 


/******************************************************************************
      ����˵������ʾ��λС������
      ������ݣ�x,y��ʾ����
                num Ҫ��ʾС������
                len Ҫ��ʾ��λ��
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ�
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_ShowFloatNum1(u16 x,u16 y,float num,u8 len,u16 fc,u16 bc,u8 sizey)
{         	
	u8 t,temp,sizex;
	u16 num1;
	sizex=sizey/2;
	num1=num*100;
	for(t=0;t<len;t++)
	{
		temp=(num1/mypow(10,len-t-1))%10;
		if(t==(len-2))
		{
			LCD_ShowChar(x+(len-2)*sizex,y,'.',fc,bc,sizey,0);
			t++;
			len+=1;
		}
	 	LCD_ShowChar(x+t*sizex,y,temp+48,fc,bc,sizey,0);
	}
}


/******************************************************************************
      ����˵������ʾͼƬ
      ������ݣ�x,y�������
                length ͼƬ����
                width  ͼƬ���
                pic[]  ͼƬ����    
      ����ֵ��  ��
******************************************************************************/
void ICACHE_FLASH_ATTR LCD_ShowPicture(u16 x,u16 y,u16 length,u16 width,const u8 pic[])
{
	u16 i,j,k=0;
	LCD_Address_Set(x,y,x+length-1,y+width-1);
	for(i=0;i<length;i++)
	{
		for(j=0;j<width;j++)
		{
			LCD_WR_DATA8(pic[k*2]);
			LCD_WR_DATA8(pic[k*2+1]);
			k++;
		}
	}			
}


