#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define STR_FLAG    " segment "
#define STR_STACK "Stack size:"


enum
{
    TYPE_UNKNOWN,
    TYPE_RAM,
    TYPE_FLASH,
    TYPE_RAM_FLASH,
    TYPE_EEPROM,
TYPE_INFO,
TYPE_BIT,
TYPE_DEBUG
};


int get_len(char *pos);
int get_type(char *pos);


int main(int argc, char* argv[])
{
    char fn[2048] = "\0";
    char *buf = NULL;


    for(int i=1;i<argc;i++)
    {
        strcat(fn,argv[i]);
    }
    //
    //
    //
    FILE *fp;


    fp = fopen(fn,"r");
    if( NULL==fp )
    {
        printf("mapinfo error: Can not read \"%s\"",fn);
        return -1;
    }
fseek(fp,0,SEEK_END);
long flen = ftell(fp);
buf = (char*)malloc(flen);
if( NULL==buf )
{
printf("mapinfo error: malloc failed!");
        return -1;
}
    fseek(fp,0,SEEK_SET);
    fread(buf,1,flen,fp);
    fclose(fp);
    buf[flen-1]='\0';
    //
    //
    //
    char *pos = buf;
char *next = buf;
int byte_unknown = 0;
    int byte_flash = 0;
    int byte_ram = 0;
    int byte_ram_flash = 0;
    int byte_eeprom = 0;
int byte_stack = 0;
    for(;;)
    {
        pos = strstr(pos, STR_FLAG);
        if( NULL==pos )
        {
            break;
        }
        int len = get_len(pos);
        pos += sizeof(STR_FLAG);
next = pos;
        int type = get_type(pos);
        //
        //
        //
        switch( type )
        {
case TYPE_UNKNOWN:
byte_unknown += len;
break;
        case TYPE_FLASH:
            byte_flash += len;
            break;
        case TYPE_RAM:
            byte_ram += len;
            break;
        case TYPE_RAM_FLASH:
            byte_ram += len;
            byte_flash += len;
            break;
        case TYPE_EEPROM:
            byte_eeprom += len;
            break;
        }
    }
pos = strstr(next,STR_STACK);
if( pos!=NULL )
{
byte_stack = atoi(&pos[sizeof(STR_STACK)]);
}
    //
    //
    //
if( byte_unknown!=0 )
{
printf("\nram:%d   flash:%d   eeprom:%d   stack:%d   unknown:%d", byte_ram,byte_flash,byte_eeprom,byte_stack,byte_unknown);
}
else
{
printf("\nram:%d   flash:%d   eeprom:%d   stack:%d", byte_ram,byte_flash,byte_eeprom,byte_stack);
}
    return 0;
}


int get_len(char *pos)
{
    *pos = '\0';
    while(*--pos!=' ')
    {
    }
    return atoi(pos+1);
}


#define    STRCMP(str1,str2)    strncmp(str1,str2,sizeof(str2)-1)
int get_type(char *pos)
{
    if( 0==STRCMP(pos,"const") )
    {
        return TYPE_FLASH;
    }
    else
    if( 0==STRCMP(pos,"text") )
    {
        return TYPE_FLASH;
    }
    else
    if( 0==STRCMP(pos,"eeprom") )
    {
        return TYPE_EEPROM;
    }
else
    if( 0==STRCMP(pos,"bsct, initialized") )
    {
        return TYPE_BIT;
    }
    else
    if( 0==STRCMP(pos,"bsct, from") )
    {
        return TYPE_RAM_FLASH;
    }
    else
    if( 0==STRCMP(pos,"ubsct") )
    {
        return TYPE_RAM;
    }
else
    if( 0==STRCMP(pos,"bit, initialized") )
    {
        return TYPE_RAM;
    }
    else
    if( 0==STRCMP(pos,"bit, from") )
    {
        return TYPE_RAM_FLASH;
    }
    else
    if( 0==STRCMP(pos,"share") )
    {
        return TYPE_RAM;
    }
else
    if( 0==STRCMP(pos,"data, initialized") )
    {
        return TYPE_RAM;
    }
    else
    if( 0==STRCMP(pos,"data, from") )
    {
        return TYPE_FLASH;
    }
    else
    if( 0==STRCMP(pos,"bss") )
    {
        return TYPE_RAM;
    }
else
if( 0==STRCMP(pos,"info.") )
{
return TYPE_INFO;
}
else
if( 0==STRCMP(pos,"debug") )
{
return TYPE_DEBUG;
}
    else
    if( 0==STRCMP(pos,"init") )
    {
        return TYPE_FLASH;
    }
    return TYPE_UNKNOWN;
}
