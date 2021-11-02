/***********************************************
公司：东莞市微宏智能科技有限公司
品牌：WHEELTEC
官网：wheeltec.net
淘宝店铺：shop114407458.taobao.com 
速卖通: https://minibalance.aliexpress.com/store/4455017
版本：5.7
修改时间：2021-04-29

Company: WeiHong Co.Ltd
Brand: WHEELTEC
Website: wheeltec.net
Taobao shop: shop114407458.taobao.com 
Aliexpress: https://minibalance.aliexpress.com/store/4455017
Version:5.7
Update：2021-04-29

All rights reserved
***********************************************/
#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
#define KEY PAin(5)
          

#define LED PCout(13)// PB5

void KEY_Init(void);//按键初始化
void LED_Init(void);//初始化
u8 click(void);               //单击按键扫描
#endif  
