#ifndef __button_H
#define __button_H
#include "stdint.h"
#include "stm32f1xx_hal.h"

#define KEY_START_Pin GPIO_PIN_8
#define KEY_START_GPIO_Port GPIOA
#define KEY_USER_Pin GPIO_PIN_11
#define KEY_USER_GPIO_Port GPIOA


#define K_LEFT_Pin GPIO_PIN_2
#define K_LEFT_GPIO_Port GPIOE
#define K_DOWN_Pin GPIO_PIN_3
#define K_DOWN_GPIO_Port GPIOE
#define K_RIGHT_Pin GPIO_PIN_4
#define K_RIGHT_GPIO_Port GPIOE
//�����ṹ��
typedef struct K_Info
{
	GPIO_TypeDef *GPIOx; //�����˿�
	uint16_t GPIO_Pin;   //��������
	uint8_t Key_count;;  //ע�ᰴ������
}Key_Message;
typedef void(*fun)(Key_Message keynum); //�ص�������ʽ

void Key_Init();
uint16_t GetKey_Value(Key_Message *t_keymessage, uint16_t *KeyValue);
void  Decide_KeyValue(uint16_t value, uint16_t *lastvalue, Key_Message *t_keymessage, fun callback);
void ScanKeys(uint16_t *KeyValue, uint16_t *lastvalue, Key_Message *t_keymessage, fun callback);
#endif // !__button_H
