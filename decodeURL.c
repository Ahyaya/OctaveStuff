#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct weblog {
	char remote_addr[32];
	char time_local[32];
	char name[128];
	char comments[2048];
	int namenum;
	int commentsnum;
}; 
struct weblog msgbox[32]={0};

static int msgnum=0,tunc=0;

int isHex(char c){
	if(c>47&&c<58) return 1;
	if(c>64&&c<71) return 1;
	if(c>96&&c<103) return 1;
	return 0;
}

int ctoHex(char c){
	if(c>47&&c<58) return c-48;
	if(c>64&&c<71) return c-55;
	if(c>96&&c<103) return c-87;
	return -1;
}

int load_css(FILE *fp){
	fprintf(fp,"\t@import\"css/msgbox.css\";\n");
	return 0;
}

int printhead(FILE *fp){
	fprintf(fp,"\t<head>\n\t\t<title>Comments</title>\n\t\t<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
	fprintf(fp,"\t<style>\n");
	load_css(fp);
	fprintf(fp,"\t</style>\n\t</head>\n");
	return 0;
}

int add_header(FILE *fp){
	fprintf(fp,"<!DOCTYPE html>\n<html>\n");
	printhead(fp);
	fprintf(fp,"<body class=\"main\"><p id=\"title\">Captain's message box</p><br>\n<div class=\"content\">\n");
	return 0;
}

int add_footer(FILE *fp){
	fprintf(fp,"\n</div>\n<a id=\"backtohome\" href=\"https://arxiv.cloud\">back to home</a><br><br><a id=\"backtotop\" href=\"#title\">back to top</a>\n</body>\n</html>\n");
	return 0;
}

int strcopy(char *buff, int len, char *origin){
	int pf;
	for(pf=0;pf<len;pf++){
		if(0==(*(buff+pf)=*(origin+pf))){
			break;
		}
	}
	return 0;
}

int decode(char *buff_decode, int len, char *origin){
	int pf=0,pc=0,strend=0;
	char buff[4];
	while(pf<len){
		buff[0]=origin[pf];
		switch(buff[0]){
		    case '+':
			buff_decode[pc]=' ';pc++;break;
		    case 37:
			buff[1]=(++pf<len)?origin[pf]:0;buff[2]=(++pf<len)?origin[pf]:0;
			if(isHex(buff[1])&&isHex(buff[2])){
				buff_decode[pc]=16*ctoHex(buff[1])+ctoHex(buff[2]);pc++;break;
			}else{
				if((buff_decode[pc]=buff[0])==0){strend=1;}pc++;
				if((buff_decode[pc]=buff[1])==0){strend=1;}pc++;
				if((buff_decode[pc]=buff[2])==0){strend=1;}pc++;
			}
			break;
		    case 38:
		    case 60:
		    case 62:
		    case '\n':
		    case 0:
			buff_decode[pc]=0;strend=1;break;
		    default:
			if((buff_decode[pc]=buff[0])==0){strend=1;}pc++;
			break;
		}
		pf++;
		if(strend){break;}
	}
	buff_decode[pc]=0;
	return pc;
}


int form_body(FILE *inf, FILE *outf){
	char buff[2048], buff_decode[2048];
	int pf, cmtnum;
	while(fgets(buff,2048,inf)!=NULL){
		if(msgnum>=32){msgnum-=32;tunc=1;}
		if(buff[0]=='>'){
			strcopy(msgbox[msgnum].remote_addr, 32, buff+pf+1);
			if(fgets(buff,2048,inf)==NULL){msgnum++;continue;}
			strcopy(msgbox[msgnum].time_local, 32, buff);
			if(fgets(buff,2048,inf)==NULL){msgnum++;continue;}
			if(fgets(buff,2048,inf)==NULL){msgnum++;continue;}
			for(cmtnum=0;cmtnum<128;cmtnum++){
				if(buff[cmtnum]=='&'&&buff[cmtnum+1]=='c') break;
			}
			msgbox[msgnum].namenum=decode(msgbox[msgnum].name,cmtnum-5,buff+5);
			msgbox[msgnum].commentsnum=decode(msgbox[msgnum].comments,2048,buff+cmtnum+10);
			msgnum++;
		}
		
	}
	if(--msgnum>=0){	
	    for(pf=msgnum;pf>=0;pf--){
		fprintf(outf,"<div class=\"info\"><b class=\"name\">%s</b> from%s\n<br /><span class=\"time\">%s</span></div>\n<pre class=\"comments\">%s</pre><br />\n",msgbox[pf].name,msgbox[pf].remote_addr,msgbox[pf].time_local,msgbox[pf].comments);
	    }
	}
	if(tunc){
	    for(pf=31;pf>msgnum;pf--){
		fprintf(outf,"<div class=\"info\"><b class=\"name\">%s</b> from%s\n<br /><span class=\"time\">%s</span></div>\n<pre class=\"comments\">%s</pre><br />\n",msgbox[pf].name,msgbox[pf].remote_addr,msgbox[pf].time_local,msgbox[pf].comments);
	    }
	}


    return 0;
}

int main(int argc, char *argv[]){
	FILE *inf, *outf;
	if(argc<3){fprintf(stdout,"Usage: %s input.txt output.txt\n",argv[0]);return -1;}
	if((inf=fopen(argv[1],"r"))==NULL){fprintf(stdout,"Unable to read %s\n",argv[1]);return -1;}
	if((outf=fopen(argv[2],"w"))==NULL){fprintf(stdout,"Unable to write %s\n",argv[2]);return -1;}
	add_header(outf);
	form_body(inf,outf);
	add_footer(outf);
	fclose(outf);
	fclose(inf);
	return 0;
}

