#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <time.h>
#define MAXDATASIZE 1400

struct valveServInfo
{
	char ipv4[16];
	int port;
	int status;
	char hostname[64];
	char map[32];
	int players;
	int slots;
	char playername[8][32];
	int playerscore[8];
	double duration[8];
};

struct valveServInfo AnneServer[64]={0};
static int numServ=0, sortValve[64]={0};
static int sortIndex[64]={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};

int DNSquery(char* hostname)
{
	char  *ptr;
	struct hostent *hostptr;

	if ((hostptr = gethostbyname(hostname)) == NULL)
	{
		printf("DNS query failure: unable to solve %s\n",hostname);
		return -1;
	}

	if(hostptr->h_addrtype == AF_INET)
	{
		inet_ntop(hostptr->h_addrtype, *(hostptr->h_addr_list), hostname, 64);
	}else{
		puts("DNS query failure: unknown address type\n");
        return -1;
	}
    return 0;
}

int A2S_INFO(char *addrport)
{
    float time_sec;
    int sockfd, num, pf, pt, nameLen=0, min=0, hr=0, score=0;
    unsigned char buf[2048],challenge[9],player_request[9],*p_time = (unsigned char*)&time_sec;
    unsigned char info_request[29]={0xFF,0xFF,0xFF,0xFF,0x54,0x53,0x6F,0x75,0x72,0x63,0x65,0x20,0x45,0x6E,0x67,0x69,0x6E,0x65,0x20,0x51,0x75,0x65,0x72,0x79,0x00,0xff,0xff,0xff,0xff};

    //transfer arguments to declare ip,port and socket
    char *server_IP, *getport, *input_option=strdup(addrport);
    server_IP = strsep(&input_option,":");
    getport = strsep(&input_option,":");
    free(input_option);
    int PORT=(getport==NULL)?27015:atoi(getport);
    struct sockaddr_in server;

    //define protocol as UPD and initiate the socket
    if((sockfd=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
		sprintf(AnneServer[numServ].ipv4,"%s",server_IP);
		AnneServer[numServ].port=PORT;
		AnneServer[numServ].status=-1;
        return -1;
    }else{
		AnneServer[numServ].status=0;
	}
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    if((server.sin_addr.s_addr = inet_addr(server_IP))==-1)
    {
        if(DNSquery(server_IP)==-1){
			sprintf(AnneServer[numServ].ipv4,"%s",server_IP);
			AnneServer[numServ].port=PORT;
			AnneServer[numServ].status=-1;
			return -1;
		}
        server.sin_addr.s_addr = inet_addr(server_IP);
    }

    //set timeout limit to avoid stuck at recv() process
    struct timeval timeout;
    timeout.tv_sec = 3; timeout.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1)
    {
		sprintf(AnneServer[numServ].ipv4,"%s",server_IP);
		AnneServer[numServ].port=PORT;
		AnneServer[numServ].status=-1;
        return -1;
    }

    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
		sprintf(AnneServer[numServ].ipv4,"%s",server_IP);
		AnneServer[numServ].port=PORT;
		AnneServer[numServ].status=-1;
        return -1;
    }

    //Send info request to Valve server
    send(sockfd, info_request, 29, 0);
    if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    {
		sprintf(AnneServer[numServ].ipv4,"%s",server_IP);
		AnneServer[numServ].port=PORT;
		AnneServer[numServ].status=-1;
        return -1;
    }

    if(buf[4] & 0x41){
	info_request[25]=buf[5];info_request[26]=buf[6];info_request[27]=buf[7];info_request[28]=buf[8];
	send(sockfd, info_request, 29, 0);
	//Resending request with CHALLENGE number

	info_request[25]=0xff;info_request[26]=0xff;info_request[27]=0xff;info_request[28]=0xff;
	if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    	{
        	sprintf(AnneServer[numServ].ipv4,"%s",server_IP);
		AnneServer[numServ].port=PORT;
		AnneServer[numServ].status=-1;
        	return -1;
    	}

    }


    //Print server basic info
    for(pf=6;;)
    {
		pt=0;
        while(buf[pf]!=0x00){
			AnneServer[numServ].hostname[pt]=buf[pf++];
			pt=pt<63?pt+1:pt;
		}
		AnneServer[numServ].hostname[63]=0;
		sprintf(AnneServer[numServ].ipv4,"%s",server_IP);
		AnneServer[numServ].port=PORT;
        pf++;pt=0;
        while(buf[pf]!=0x00){
			AnneServer[numServ].map[pt]=buf[pf++];
			pt=pt<31?pt+1:pt;
		}
        pf++;AnneServer[numServ].map[31]=0;
        while(buf[pf++]!=0x00);while(buf[pf++]!=0x00);
        pf+=2;
		AnneServer[numServ].players=buf[pf];
		AnneServer[numServ].slots=buf[pf+1];
        break;
    }

    //Send player request to Valve server
    for(pf=0;pf<9;pf++) player_request[pf]=0xFF;
    player_request[4]=0x55;
    send(sockfd, player_request, 9, 0);

    //Receive challenge code from server
    if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    {
		AnneServer[numServ].players=0;
        return -1;
    }

    //Reply the challenge
    for(pf=0;pf<num;pf++) challenge[pf]=buf[pf];
    challenge[4]=player_request[4];
    send(sockfd, challenge, 9, 0);

    //Receive Players info
    if((num = recv(sockfd, buf, MAXDATASIZE, 0)) == -1)
    {
		AnneServer[numServ].players=0;
        return -1;
    }
        
    //Decode the bytes and print, buf[5] is total players.
    if(buf[5]>0x00)
    {
        pf=6;nameLen=0;pt=0;
        while(pf<num)
        {   
            if(buf[pf]==0x00) pf++;
            //Print player name
            while(buf[pf]!=0x00)
            {
                AnneServer[numServ].playername[pt][nameLen]=(buf[pf++]);
                nameLen=nameLen<31?nameLen+1:nameLen;
            }
			AnneServer[numServ].playername[pt][31]=0;
            if(nameLen==0) {sprintf(AnneServer[numServ].playername[pt],"Loading");}
            //Get score
            score=buf[++pf];pf+=4;
			AnneServer[numServ].playerscore[pt]=score;
            //Get time as float
            p_time[0]=buf[pf];p_time[1]=buf[pf+1];p_time[2]=buf[pf+2];p_time[3]=buf[pf+3];
			AnneServer[numServ].duration[pt]=time_sec;
            pf+=4;nameLen=0;pt++;
			if(pt>7) break;
        }
    }

    close(sockfd);
    return 0;
}

int print_tablerow(int Index)
{
	int pf, players=AnneServer[Index].players, hr, min;
	players=players<8?players:8;
	players=players>0?players:0;
	printf("<table cellspacing=\"0\">\n");
    if(AnneServer[Index].status)
    {
        printf("<tr class=\"jamhost\"><td><span class=\"blockred\"></span>%s</td><td colspan=\"3\">time out.</td><td></td></tr>\n</table>\n",AnneServer[Index].ipv4);
        return -1;
    }

    //Print server basic info
	printf("<tr><th colspan=\"3\">%s</th></tr>\n",AnneServer[Index].hostname);
	if(players){
		if(AnneServer[Index].slots>players){
			if(players<4){
				printf("<tr class=\"hostinfo\">");
			}else{
				printf("<tr class=\"hostinfo spechost\">");
			}
		}else{
			printf("<tr class=\"hostinfo fullhost\">");
		}
	}else{
		printf("<tr class=\"emptyhost\">");
	}
	printf("<td class=\"addr\"><span class=\"blockgreen\"></span>%s:%d</td><td>%s</td><td><span class=\"slotinfo\">%d</span>/%d</td></tr>\n",AnneServer[Index].ipv4,AnneServer[Index].port,AnneServer[Index].map,AnneServer[Index].players,AnneServer[Index].slots);

	//Print player info
    for(pf=0;pf<players;pf++){
		printf("<tr class=\"playerinfo\">");
        //Print player name
        printf("<td class=\"playername\">%s</td>",AnneServer[Index].playername[pf]);
		//Print play time and score
		hr=AnneServer[Index].duration[pf]/3600;min=(AnneServer[Index].duration[pf]-hr*3600)/60;
        printf("<td>");
		if(hr) printf("%dh",hr);
		if(min) printf("%dm",min);
		printf("%.0fs</td>",AnneServer[Index].duration[pf]-3600*hr-60*min);
		printf("<td>%d</td>",AnneServer[Index].playerscore[pf]);
		printf("</tr>");

	}
    
	printf("</table>\n");
    return 0;
}

int load_css(){
	printf("\t@import\"css/serverlist.css\";\n");
	return 0;
}

int print_head()
{
    printf("<head>\n<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=10.0\">\n<meta http-equiv=\"cache-control\" content=\"max-age=90\">\n");
    printf("\t<style>\n");
    load_css();
    printf("\t</style>\n</head>\n");
    return 0;
}

int print_footer()
{
	printf("<br />\n<div class=\"footer\"><pre>This L4D2 CO-OP plugin is developed by <b>Anne</b>.\nUnzip the <a href=\"http://47.115.132.92/AnneHappy/\">AnneHappy plugin</a> to your dedicated server and join us!\nThank <b>HoongDou</b> for updating the script and <a href=\"https://www.hoongdou.com/index.php/2021/05/22/anne/\">README</a>.</pre></div>\n");
	return 0;
}

int PivotSort(int head, int tail, int* index, int* data)
{
    int pivot=data[index[head]], swap;
    int phead=head, ptail=tail;
    while(phead<ptail)
    {
        while(data[index[ptail]]>=pivot && ptail>phead){ptail--;}
        swap=index[ptail];index[ptail]=index[phead];index[phead]=swap;
        while(data[index[phead]]<=pivot && phead<ptail){phead++;}
        swap=index[phead];index[phead]=index[ptail];index[ptail]=swap;
    }
    return phead;
}

int QuickSort(int head, int tail, int* index, int* data)
{
    int pivot;
    if(tail<2+head){return 0;}
    pivot=PivotSort(head, tail, index, data);
    QuickSort(head, pivot, index, data);
    QuickSort(pivot+1, tail, index, data);
}

int main(int argc, char * argv[])
{
    FILE *serverlist;
    char addrport[64], str_time[128];
    time_t var_time_t=time(NULL);
    const struct tm *ptr_local_time=localtime(&var_time_t);
	int pf,pv,openslots,players;

    strftime(str_time,128,"%B %d %Y <b class=\"time\">%H:%M</b> %Z",ptr_local_time);
    if(argc!=2){printf("\nUsage: %s filename > test.html\n\n",argv[0]);return -1;}
    if((serverlist=fopen(argv[1],"r"))==NULL){printf("Can not open file %s\n",argv[1]);return -1;}

    printf("<!DOCTYPE html>\n<html>");
    print_head();
    printf("<body class=\"main\">");
    printf("<h2 id=\"title\">Anne Happy Group Server List</h2><br>\n");
    printf("<p class=\"date\">%s</p><br>\n",str_time);
    while(fscanf(serverlist,"%s",addrport)>0 && numServ<64){
        A2S_INFO(addrport);
		/*define sort value*/
		players=AnneServer[numServ].players;
		openslots=AnneServer[numServ].slots-players;
		openslots=openslots>0?openslots:0;
		openslots=openslots<10?openslots:10;
		pv=players>0?(players<4?(700+64*players):(64*openslots+64)):0;
		pv=openslots>0?pv:64;
		sortValve[numServ]=numServ+pv+999*AnneServer[numServ].status;
		numServ++;
    }
	QuickSort(0,numServ-1,sortIndex,sortValve);
	for(pf=numServ;pf>0;pf--){
		print_tablerow(sortIndex[pf-1]);
	}

	print_footer();
    printf("</body>\n</html>\n");
    fclose(serverlist);
    return 0;
}
