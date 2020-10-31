/*
===============================================================
*����:��������
*Ӳ��:STM32F103ZET6
*�ļ�:button.c  
*����:2020/5/23      
*����: xiaowenlg   ԭ��
*����ԭ���ж����λ�ȡ�İ���ֵ�Ƿ���ͬ����ͬ�Ͳ���������ͬ�Ͷ���

*ʹ�÷���=======================================================
1����Ҫʹ�õ��ļ���#include "button.h"
2,����Key_Message �ṹ�� ���飬������Ϣ����
3����������ȫ�ֱ������ڴ洢���κ��ϴΰ�����ֵ
4,��ʼ��������Ϣ
5�����尴������ص�����
6�����ð���ɨ�躯��
===============================================================
*/
#include "button.h"
void Key_Init()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	/*Configure GPIO pins : PAPin PAPin */
	GPIO_InitStruct.Pin = KEY_START_Pin | KEY_USER_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = K_LEFT_Pin | K_DOWN_Pin | K_RIGHT_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}
//ȡ����ֵ
//************************************
// ����:    GetKey_Value
// ����ȫ��:  GetKey_Value
// ��������:    public 
// ����ֵ:   uint16_t                     ���ؼ�ֵ
// Qualifier://ȡ����ֵ
// ����: Key_Message * t_keymessage       �����ṹ��
// ����: uint16_t * KeyValue              ���水��ֵ        ����Ϊȫ�ֱ�����ָ����ʽ��Ŀ���Ǵ����ݵĵ�ַ
//************************************
uint16_t GetKey_Value(Key_Message *t_keymessage, uint16_t *KeyValue)
{
	uint8_t Key_Count = t_keymessage[0].Key_count;
	for (int i = 0; i < Key_Count; i++)
	{
		if (HAL_GPIO_ReadPin(t_keymessage[i].GPIOx, t_keymessage[i].GPIO_Pin))
			*KeyValue |= (1 << i);                    //������       ��λΪ1
		else
			*KeyValue &= ~(1 << i);                  //ȡ������������   ��λΪ0
		//Uartx_printf(&huart1, "value is %x\r\n", *KeyValue);
	}
	return *KeyValue;
}
//************************************
// ����:    Decide_KeyValue
// ����ȫ��:  Decide_KeyValue
// ��������:    public 
// ����ֵ:   void
// Qualifier:  ɨ�谴��ֵ��ִ�а����ص�����
// ����: uint16_t value          ��ȡ���İ���ֵ
// ����: uint16_t * lastvalue    �洢��һ�εİ���ֵ               ����Ϊȫ�ֱ�����ָ����ʽ��Ŀ���Ǵ����ݵĵ�ַ
// ����: fun callback            �ص�����
//************************************
void  Decide_KeyValue(uint16_t value, uint16_t *lastvalue, Key_Message *t_keymessage, fun callback)
{
	char s[16] = { 0 };
	uint16_t tempvalue = 0;
	if (value^*lastvalue)
	{
		tempvalue = value;
		for (uint8_t i = 0; i < 16; i++)
		{
			if (!(tempvalue & 0x0001))
			{
				//ִ�лص�����
				(*callback)(t_keymessage[i]);        //���ذ����ṹ��
			}
			tempvalue >>= 1;
		}
		*lastvalue = value;
	}
	HAL_Delay(10);
}
//����ɨ�躯��
void ScanKeys(uint16_t *KeyValue, uint16_t *lastvalue, Key_Message *t_keymessage, fun callback)
{
	Decide_KeyValue(GetKey_Value(t_keymessage,KeyValue), lastvalue, t_keymessage, callback);
}