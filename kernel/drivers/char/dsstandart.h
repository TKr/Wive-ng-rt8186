#ifndef DSSTANDART_H
#define DSSTANDART_H

int             TouchReset(void);
void            TouchWriteByte(unsigned char byte);
unsigned char   TouchReadByte(void);
void            TouchPullUp(int state);
int             TouchSearchRom(void);
int             TouchDevicesCount(void);
unsigned char * TouchDevice(int devno);
void            TouchMatchRom(unsigned char *Lasered);

#endif //DSSTANDART_H

