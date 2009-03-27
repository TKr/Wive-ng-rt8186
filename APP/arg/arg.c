

#include <string.h>
#include <stdio.h>
//#include <unistd.h>
#include <stdlib.h>

#include "config.h"

long int position, offset;

void do_usage (){
    printf("arg Usage: \n");
    printf("    arg [set|s] <argument> <value> - To set some value to argument\n");
//    printf("or\n");    
    printf("    arg [get|g] <argument> - To get value og argument\n");
//    printf("    arg [list|l] - To list all arguments and its values\n");
}

int parse_line(char *str, int len, char *var, char *arg){

  int r,newlen,pvt;
  char c, *s;

  //Cutting all comments, that is started with '#' char
    s = strchr(str,'#');
    if(s != NULL){
      memset(s,'\0',len - (str - s));
      newlen = (s - str);
    }else{
      newlen = len;
    }

    if(str[0]=='\0')return 0;

  //Deleting all spaces and tabs before and after text
    for(r=newlen,c=str[r];((c==' ')||(c=='\t')||(c=='\0'))&&(r>=0);r--,c=str[r]);r++;
    memset(str+r,'\0',newlen-r);
    
    if(str[0]=='\0')return 0;

    newlen=r;
    for(r=0,c=str[r];((c==' ')||(c=='\t'))&&(c!='\0');r++,c=str[r]);
    if(newlen-r>0){
      memmove(str,str+r,newlen-r);
      newlen=newlen-r;
    }
    str[newlen]='\0';

    if(str[0]=='\0')return 0;

//    for(r=newlen-r,c=str[r];((c==' ')||(c=='\t'))&&(c!='\0');r++,c=str[r]);

  //Parsing Line for parameters.
    if(str[0]!='\0'){ //If string present
       memset(var,'\0',len);
       memset(arg,'\0',len);

       for(newlen=0;str[newlen]!='\0';newlen++); //New Line length - without artefacts :-)
       for(r=0;str[r]!='=' && r<=newlen;r++)var[r]=str[r];
       var[r]='\0';

       pvt=r+1;
       for(r++;str[r]!='\0' && r<=newlen;r++)arg[r-pvt]=str[r];
       arg[r]='\0';

    }else return 0; //if string is empty.
    if(var[0]!='\0' && !strncmp(arg,"%",1)) return 2; //if there's some var in arg.
    if(var[0]!='\0' && arg[0]!='\0') return 1; //If var and arg present.
    if(var[0]!='\0' && arg[0]=='\0') return 3; //If only var present,so it's only string.
    return 0;
}

int find_parameter (FILE *fd, char *arg, char *flnm, char *value){
    
    char line[MAXLINE],c,vn[MAXLINE],va[MAXLINE],*llen;
    char innerline[MAXLINE];
    int p=0,r,l=0,d;
    FILE *sd;

    memset(line,'\0',MAXLINE);
    
    fseek(fd,-1,2);
    if(fgetc(fd)!=0xA){
	printf("no newline at end in %s... bailing\n",CONFIG_LIST);
	return 1;
    }fseek(fd,0,0);
    
    while(fgetc(fd)!=EOF){
	if(line != NULL){
	    fseek(fd,-1,1);
	    memset(line,'\0',p);    
	}
	//reading all line from CONFIGLIST to line
//	for(p=0,c=fgetc(fd);c!=0xA;c=fgetc(fd),p++)line[p]=c;
        fgets(line,MAXLINE,fd);
        llen=strchr(line,'\0');
        p=llen-line-1;
        line[p]='\0';

	if(parse_line(line,p,vn,va)){
	    if( ( sd = fopen (line, "r") ) == 0){
		printf("cann't read %s\n",line);
    		exit(1);
	    }
	    memset(innerline,'\0',MAXLINE);
	    fseek(sd,-1,2);
	    if(fgetc(sd)!=0xA){
		printf("no newline at end in %s... bailing\n",line);
		return 1;
	    }fseek(sd,0,0);
    
	    while(fgetc(sd)!=EOF){
		if(innerline != NULL){
		    fseek(sd,-1,1);
		    memset(innerline,'\0',l);    
		}
		//reading all line from CONFIGLIST to line
//		for(l=0,c=fgetc(sd);c!=0xA;c=fgetc(sd),l++)innerline[l]=c;
                fgets(innerline,MAXLINE,sd);
                llen=strchr(innerline,'\0');
                l=llen-innerline-1;
                innerline[l]='\0';

		
		if(parse_line(innerline,l,vn,va)){
		    if(!strcmp(vn,arg)){
			offset = ftell(sd);
			offset -= 2;
						
			position = offset - l;
			offset += 1;
					
			strcpy(flnm,line);
			strcpy(value,va);
			return 0;
		    }
		}
	    }

	}
    }
    
    return 1;
}

void do_set ( char *arg, char *val ){

    char filename[MAXLINE], value[MAXLINE], temp_file[MAXLINE];
    FILE *fd, *td;
    
    if( ( fd = fopen (CONFIG_LIST, "r") ) == 0){
      printf("cann't read %s\n",CONFIG_LIST);
      exit(1);
    }
    
    if(find_parameter(fd,arg,filename,value)){
      printf("Not Found.\n");
      exit(2);
    }

    fclose(fd);
    
    if( ( fd = fopen (filename, "r") ) == 0){
      printf("cann't read %s\n",filename);
      exit(1);
    }
    
    memset(temp_file,'\0',MAXLINE);
    strcpy(temp_file,filename);
    strcat(temp_file,TEMP_POSTFIX);
    
    if( ( td = fopen (temp_file, "w") ) == 0){
      printf("cann't open for write %s\n",temp_file);
      exit(4);
    }
    
    char line[MAXLINE],c;
    int r;
    
    memset(line,'\0',MAXLINE);
    
    while(ftell(fd) <= position){
	c=fgetc(fd);
	fputc(c,td);
    }
    strcpy(line,arg);
    strcat(line,"=");
    strcat(line,val);

    for(r=1,c=line[0];c!='\0';c=line[r],r++)fputc(c,td);

    fseek(fd,offset,0);
    while(fgetc(fd) != EOF){
	fseek(fd,-1,1);
	c=fgetc(fd);
	
	fputc(c,td);
    }
    
    fclose(td);
    fclose(fd);

    unlink(filename);
    link(temp_file,filename);
    unlink(temp_file);
    
    printf("Was %s: %s\nNow %s: %s\n",arg,value,arg,val);

}

void do_get ( char *arg ){

    char filename[MAXLINE], value[MAXLINE];
    FILE *fd;
    
    if(!strcmp(arg,"truth") || !strcmp(arg,"TRUTH")){printf("The Universe is Infinite!\n");exit(0);}
    
    if( ( fd = fopen (CONFIG_LIST, "r") ) == 0){
      printf("cann't read %s\n",CONFIG_LIST);
      exit(1);
    }
    
    if(find_parameter(fd,arg,filename,value)){
      printf("Not Found.\n");
      exit(2);
    }

    printf("%s: %s\n",arg,value);

}

int main ( int argc, char **argv ){

    int notok=1;
    
    if ( argc > 1 ){
    
        if ( !strcmp(argv[1], "set") || !strcmp(argv[1], "s") ){

	    if ( argv[2] == NULL || argv[3] == NULL ){
		printf("missing parameter\n");
	    }else{
		do_set (argv[2],argv[3]);
	    }
	    notok--;

	}

	if ( !strcmp(argv[1], "get") || !strcmp(argv[1], "g") ){

    	    if ( argv[2] == NULL ){
		printf("missing parameter\n");
	    }else{
		do_get (argv[2]);
	    }
	    notok--;

	}

    }
    
    if(notok == 1) do_usage();
    
    return 0;
}
