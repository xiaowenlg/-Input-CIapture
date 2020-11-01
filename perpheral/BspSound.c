#include "BspSound.h"
#include "WTN6.h"
#include "APPTooL.h"
#include "BspConfig.h"

uint8_t sonundArray[6] = { HUAN_YING, BEN_XIANG_MU, ZHONG_TIAO_GAO_DU, CESHI, QING_ZHAN_ZHONG_TIAO };
//uint8_t dalaytim[4] = { 0xf3,YIN_XIAO_61,0xf8,10};				//F3H + 地址 + F8H + 10H
void Firstmuis()            //开头语音
{
	WTN6_PlayArray(5, sonundArray);
	osDelay(300);
	WTN6_PlayOneByte(QING_AN_KAISHI);
	//WTN6040_PlayArray(4, dalaytim);
	/**/
}
void BeginSound()
{
	WTN6_PlayOneByte(YIN_XIAO_61);
	osDelay(100);
	WTN6_PlayOneByte(CESHI_KAISHI);
}
uint8_t ProcessHeight(double numdata)//身高
{
	uint8_t dat[16] = { 0 };
	uint8_t reslen = 0, templen = 0;
	uint8_t tdat[16] = { 0 };
	templen = DataSeparation(numdata, tdat);
	dat[0] = NINDE;
	dat[1] = ZHONG_TIAO_GAO_DU;
	dat[2] = GONG_FEN;
	reslen = insertArray(dat, 3, tdat, templen, 2);
	WTN6_PlayArray(reslen, dat);
	return reslen;
}