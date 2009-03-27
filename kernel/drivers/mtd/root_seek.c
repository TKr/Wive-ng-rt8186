/* This file contain code for true searching ROOT signature in 
   RTL8186 flash.
*/
#include <linux/mtd/mtd.h>
#include <linux/mtd/partitions.h>
#include <linux/mtd/doc2000.h>
#include <linux/squashfs_fs.h>
#include <linux/slab.h>

#define JFFS_PADDING 0x10000
#define TGZFS_PART 0x20000

#define DWORD_SWAP(v) ( (((v&0xff)<<24)&0xff000000) | ((((v>>8)&0xff)<<16)&0xff0000) | \
				((((v>>16)&0xff)<<8)&0xff00) | (((v>>24)&0xff)&0xff) )
#define WORD_SWAP(v) ((unsigned short)(((v>>8)&0xff) | ((v<<8)&0xff00)))

#define SQFS_PAGE_SIZE 4096

struct rootInfo {
   unsigned int partitionlen;
   unsigned int checksumm;
};   

struct rootHeader {
   char   signature[4];
   struct rootInfo info;
};

static char RootSignature[4]= {'R','O','O','T'};
// ------------------------------------------------------------------------
static unsigned short calculateChecksum(volatile unsigned char *buf, int len)
{
 int i, j;
 unsigned short sum=0;

 j = (len/2)*2;
 
 for(i=0;i<j;i+=2) sum += *((unsigned short *)(buf + i));
 
 if(len%2) sum += buf[len-1];
 
 return ~sum+1;
}
// ------------------------------------------------------------------------
static int SeekSignature(struct mtd_info * mtd,char * signature,int signaturelen,unsigned int * currentFlashPos)
{
 struct DiskOnChip *doc = (struct DiskOnChip *)mtd->priv;
 unsigned long docptr = doc->virtadr;
 unsigned flashSize = doc->totlen;
 int signPos=0;

 for(;(*currentFlashPos)<flashSize;(*currentFlashPos)++)
   {
    if(((volatile char * )docptr)[*currentFlashPos]==signature[signPos])
      {
       if(signPos==(signaturelen-1)) {(*currentFlashPos)++; return 1;}
       else ++signPos;
      }
    else signPos=0;
   }
 return 0;  
}
// ------------------------------------------------------------------------
static int CheckRootChecksumm(struct mtd_info * mtd,unsigned int startFlashPos)
{
 struct DiskOnChip *doc = (struct DiskOnChip *)mtd->priv;
 unsigned long docptr = doc->virtadr;
 unsigned flashSize = doc->totlen;
 volatile struct rootInfo * info = (volatile struct rootInfo *)(docptr+startFlashPos);
 unsigned short calced_checksumm;
 
 // Check partition size
 if(startFlashPos+info->partitionlen > flashSize)
   {
    printk("    Partition lenght greate than flash size. Skip.\n");
    return 0;
   }
 // Calculate checksumm
 calced_checksumm = calculateChecksum((volatile char *)(info+1),info->partitionlen);
 if(info->checksumm != calced_checksumm)
   {
    printk("    Checksumm missmatch (for 0x%06X bytes calced=0x%04X, stored=0x%04X) ! Skip.\n",
               info->partitionlen,(int)calced_checksumm,(int)info->checksumm);
    return 0;
   }
 printk("    Checksumm ok for 0x%06X bytes calced=0x%04X, stored=0x%04X)",
               info->partitionlen,(int)calced_checksumm,(int)info->checksumm);
 return 1;  
}
// ------------------------------------------------------------------------
static int FillMTDPartitions(struct mtd_info * mtd,unsigned int startFlashPos,struct mtd_partition * rtl8186_partitions)
{
 struct DiskOnChip *doc = (struct DiskOnChip *)mtd->priv;
 unsigned long docptr = doc->virtadr;
 unsigned flashSize = doc->totlen;
 volatile struct rootInfo * info = (volatile struct rootInfo *)(docptr+startFlashPos);

 printk("Reallocate partition for root size=0x%06X bytes\n",info->partitionlen);
 // Linux
 rtl8186_partitions[0].offset = 0; 
 rtl8186_partitions[0].size   = startFlashPos+sizeof(struct rootInfo); 
 // Root
 rtl8186_partitions[1].offset = startFlashPos+sizeof(struct rootInfo); 
 if(info->partitionlen % JFFS_PADDING)
   rtl8186_partitions[1].size = (info->partitionlen/JFFS_PADDING+1)*JFFS_PADDING; 
 else
   rtl8186_partitions[1].size = info->partitionlen; 
 
   //Pade for 65536 bytes
 // Jffs2
#ifdef CONFIG_MTD_TRUE_ROOT_4JFFS 
 rtl8186_partitions[2].offset = rtl8186_partitions[1].offset+rtl8186_partitions[1].size;
 rtl8186_partitions[2].size   = flashSize-rtl8186_partitions[2].offset;
#else
 rtl8186_partitions[2].offset = flashSize-TGZFS_PART;
 rtl8186_partitions[2].size   = TGZFS_PART;
#endif
 rtl8186_partitions[3].offset = rtl8186_partitions[3].offset;
 rtl8186_partitions[3].size   = rtl8186_partitions[3].size;
 return 4;
}
// ------------------------------------------------------------------------
int RTL8186_ROOT_Seek(struct mtd_info *mtd,struct mtd_partition * rtl8186_partitions)
{
 struct DiskOnChip *this = (struct DiskOnChip *)mtd->priv;
 unsigned int flashSize = this->totlen;
 unsigned int currentFlashPos=0;

// Seek ROOT signature
 do  {
    printk("=> Scan for ROOT signature in flash from pos=0x%06X\n",currentFlashPos);
    if(SeekSignature(mtd,RootSignature,sizeof(RootSignature),&currentFlashPos)) 
      {
       printk("   Founded at pos=0x%06X\n",currentFlashPos-sizeof(RootSignature));
       if(CheckRootChecksumm(mtd,currentFlashPos)) 
         {
	  printk("   Checksumm OK. Use as ROOT partition (/dev/mtd1)\n");
	  break;
	 } 
      }
    else
      {
       currentFlashPos = 0;
       break;
      }
 } while(currentFlashPos < flashSize);

 if(currentFlashPos != 0)
   return FillMTDPartitions(mtd,currentFlashPos,rtl8186_partitions);
 printk("=> ROOT signature not found. Realocate RTL root partition from 0x100000\n");
 return 2;
}

