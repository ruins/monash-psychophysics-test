#ifndef   __SD_Card_H__
#define   __SD_Card_H__

#include "system.h"
#include <io.h>

//-------------------------------------------------------------------------
//  SD Card Set I/O Direction
#define SD_CMD_IN   IOWR(SD_CMD_BASE, 1, 0)
#define SD_CMD_OUT  IOWR(SD_CMD_BASE, 1, 1)
#define SD_DAT_IN   IOWR(SD_DAT_BASE, 1, 0)
#define SD_DAT_OUT  IOWR(SD_DAT_BASE, 1, 1)
//  SD Card Output High/Low
#define SD_CMD_LOW  IOWR(SD_CMD_BASE, 0, 0)
#define SD_CMD_HIGH IOWR(SD_CMD_BASE, 0, 1)
#define SD_DAT_LOW  IOWR(SD_DAT_BASE, 0, 0)
#define SD_DAT_HIGH IOWR(SD_DAT_BASE, 0, 1)
#define SD_CLK_LOW  IOWR(SD_CLK_BASE, 0, 0)
#define SD_CLK_HIGH IOWR(SD_CLK_BASE, 0, 1)
//  SD Card Input Read
#define SD_TEST_CMD IORD(SD_CMD_BASE, 0)
#define SD_TEST_DAT IORD(SD_DAT_BASE, 0)
//-------------------------------------------------------------------------
#define BYTE    unsigned char
#define UINT16  unsigned int
#define UINT32  unsigned long
//-------------------------------------------------------------------------
void Ncr(void);
void Ncc(void);
BYTE response_R(BYTE);
BYTE send_cmd(BYTE *);
BYTE SD_read_lba(BYTE *,UINT32,UINT32);
BYTE SD_card_init(void);
//-------------------------------------------------------------------------
BYTE read_status;
BYTE response_buffer[20];
BYTE RCA[2];//relative card address
BYTE cmd_buffer[5];
const BYTE cmd0[5]   = {0x40,0x00,0x00,0x00,0x00};//initialisation
const BYTE cmd55[5]  = {0x77,0x00,0x00,0x00,0x00};//tells the controller that the next command is acmd format
const BYTE cmd2[5]   = {0x42,0x00,0x00,0x00,0x00};//responds with unique card identification number (CID)
const BYTE cmd3[5]   = {0x43,0x00,0x00,0x00,0x00};//publishes a shorter version of CID named RCA, Relative Card Address, for future data transfer mode
const BYTE cmd7[5]   = {0x47,0x00,0x00,0x00,0x00};//used to select which card is to be in transfer mode
const BYTE cmd9[5]   = {0x49,0x00,0x00,0x00,0x00};//obrains card specific data ( CSD register ) eg. block length, card storage capacity
const BYTE cmd16[5]  = {0x50,0x00,0x00,0x02,0x00};//set block length
const BYTE cmd17[5]  = {0x51,0x00,0x00,0x00,0x00};//block read
const BYTE acmd6[5]  = {0x46,0x00,0x00,0x00,0x02};//
const BYTE acmd41[5] = {0x69,0x0f,0xf0,0x00,0x00};//checks that the voltage is correct
const BYTE acmd51[5] = {0x73,0x00,0x00,0x00,0x00};//
//-------------------------------------------------------------------------
void Ncr(void)
{
  SD_CMD_IN;
  SD_CLK_LOW;
  SD_CLK_HIGH;
  SD_CLK_LOW;
  SD_CLK_HIGH;
}
//-------------------------------------------------------------------------
void Ncc(void)
{
  int i;
  for(i=0;i<8;i++)
  {
    SD_CLK_LOW;
    SD_CLK_HIGH;
  }
}
//-------------------------------------------------------------------------
BYTE SD_card_init(void)
{
    BYTE x,y;
    SD_CMD_OUT;
    SD_DAT_IN;
    SD_CLK_HIGH;
    SD_CMD_HIGH;
    SD_DAT_LOW;
    read_status=0;
    for(x=0;x<40;x++)
    Ncr();
    for(x=0;x<5;x++)
    cmd_buffer[x]=cmd0[x];
    y = send_cmd(cmd_buffer);
    do
    {
      for(x=0;x<40;x++);
      Ncc();
      for(x=0;x<5;x++)
      cmd_buffer[x]=cmd55[x];
      y = send_cmd(cmd_buffer);
      Ncr();
      if(response_R(1)>1) //response too long or crc error
      return 1;
      Ncc();
      for(x=0;x<5;x++)
      cmd_buffer[x]=acmd41[x];
      y = send_cmd(cmd_buffer);
      Ncr();
    } while(response_R(3)==1);
    Ncc();
    for(x=0;x<5;x++)
    cmd_buffer[x]=cmd2[x];
    y = send_cmd(cmd_buffer);
    Ncr();
    if(response_R(2)>1)
    return 1;
    Ncc();
    for(x=0;x<5;x++)
    cmd_buffer[x]=cmd3[x];
    y = send_cmd(cmd_buffer);
    Ncr();
    if(response_R(6)>1)
    return 1;
    RCA[0]=response_buffer[1];
    RCA[1]=response_buffer[2];
    Ncc();
    for(x=0;x<5;x++)
    cmd_buffer[x]=cmd9[x];
    cmd_buffer[1] = RCA[0];
    cmd_buffer[2] = RCA[1];
    y = send_cmd(cmd_buffer);
    Ncr();
    if(response_R(2)>1)
    return 1;
    Ncc();
    for(x=0;x<5;x++)
    cmd_buffer[x]=cmd7[x];
    cmd_buffer[1] = RCA[0];
    cmd_buffer[2] = RCA[1];
    y = send_cmd(cmd_buffer);
    Ncr();
    if(response_R(1)>1)
    return 1;
    Ncc();
    for(x=0;x<5;x++)
    cmd_buffer[x]=cmd16[x];
    y = send_cmd(cmd_buffer);
    Ncr();
    if(response_R(1)>1)
    return 1;
    read_status =1; //sd card ready
    return 0;
}
//-------------------------------------------------------------------------
BYTE SD_read_lba(BYTE *buff,UINT32 lba,UINT32 seccnt)
{
  BYTE c=0;
  UINT32  i,j;
//  lba+=101;
  for(j=0;j<seccnt;j++)
  {
    {
      Ncc();
      cmd_buffer[0] = cmd17[0];
/*
      cmd_buffer[1] = (lba>>16)&0xff;
      cmd_buffer[2] = (lba>>8)&0xff;
      cmd_buffer[3] = (lba)&0xff;
      cmd_buffer[4] = 0;
*/
      cmd_buffer[1] = (lba>>15)&0xff;
      cmd_buffer[2] = (lba>>7)&0xff;
      cmd_buffer[3] = (lba<<1)&0xff;
      cmd_buffer[4] = 0;

//      lba++;
      send_cmd(cmd_buffer);
      Ncr();
    }
    while(1)
    {
      SD_CLK_LOW;
      SD_CLK_HIGH;
      if(!(SD_TEST_DAT))
      break;
    }
    for(i=0;i<512;i++)
    {
      BYTE j;
      for(j=0;j<8;j++)
      {
        SD_CLK_LOW;
        SD_CLK_HIGH;
        c <<= 1;
        if(SD_TEST_DAT)
        c |= 0x01;
      }
      *buff=c;
      buff++;
    }
    for(i=0; i<16; i++)
    {
        SD_CLK_LOW;
        SD_CLK_HIGH;
    }
  }
  read_status = 1;  //SD data next in
  return 0;
}
//-------------------------------------------------------------------------
BYTE response_R(BYTE s)
{
  BYTE a=0,b=0,c=0,r=0,crc=0;
  BYTE i,j=6,k;
  while(1)
  {
    SD_CLK_LOW;
    SD_CLK_HIGH;
    if(!(SD_TEST_CMD))
    break;
    if(crc++ >100)
    return 2;
  }
  crc =0;
  if(s == 2)
  j = 17;

  for(k=0; k<j; k++)
  {
    c = 0;
    if(k > 0)                      //for crc culcar
    b = response_buffer[k-1];
    for(i=0; i<8; i++)
    {
      SD_CLK_LOW;
      if(a > 0)
      c <<= 1;
      else
      i++;
      a++;
      SD_CLK_HIGH;
      if(SD_TEST_CMD)
      c |= 0x01;
      if(k > 0)
      {
        crc <<= 1;
        if((crc ^ b) & 0x80)
        crc ^= 0x09;
        b <<= 1;
        crc &= 0x7f;
      }
    }
    if(s==3)
    {
      if( k==1 &&(!(c&0x80)))
      r=1;
    }
    response_buffer[k] = c;
  }
  if(s==1 || s==6)
  {
    if(c != ((crc<<1)+1))
    r=2;
  }
  return r;
}
//-------------------------------------------------------------------------
BYTE send_cmd(BYTE *in)
{
  int i,j;
  BYTE b,crc=0;
  SD_CMD_OUT;
  for(i=0; i < 5; i++)
  {
    b = in[i];
    for(j=0; j<8; j++)
    {
      SD_CLK_LOW;
      if(b&0x80)
      SD_CMD_HIGH;
      else
      SD_CMD_LOW;
      crc <<= 1;
      SD_CLK_HIGH;
      if((crc ^ b) & 0x80)
      crc ^= 0x09;
      b<<=1;
    }
    crc &= 0x7f;
  }
  crc =((crc<<1)|0x01);
  b = crc;
  for(j=0; j<8; j++)
  {
    SD_CLK_LOW;
    if(crc&0x80)
    SD_CMD_HIGH;
    else
    SD_CMD_LOW;
    SD_CLK_HIGH;
    crc<<=1;
  }
  return b;
}
//-------------------------------------------------------------------------
#endif
