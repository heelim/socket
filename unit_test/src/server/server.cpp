#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h> 
#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 

#include <iostream>
#include "CommunicationManager.h"
#define BUFSIZE 1024
#define MAXUSER 50
#define MAXROOM 10
struct client_info {
	int sock;
	int fsock;
	int room;
	char ip[20];
};
int main(int argc, char* argv[]) {
	CommunicationManager server;

	int port, fport;
	port = 8888; 
	fport = 9999;
	int sock, client_sock;
	struct sockaddr_in addr, client_addr;
	int fsock, client_fsock; 
	struct sockaddr_in file_addr;

	sock = server.tcp_listen(INADDR_ANY, port, 5);

	fsock = server.tcp_listen(INADDR_ANY, fport, 5);

	char rbuf[BUFSIZE]; 
	char wbuf[BUFSIZE];
	char buf[BUFSIZE];
	int readlen, addr_len; 
	
	int maxfd = 0;
	fd_set readfds;

	int i, j;

	addr_len = sizeof(client_addr); 
	printf("waiting for client..\n"); 

	int num_user=0;
	int room_info[MAXROOM][MAXUSER]={0,};
	struct client_info client[MAXUSER]; 

	while(1){
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		for (i = 0; i < num_user; i++)
			FD_SET(client[i].sock, &readfds);

		//printf("monitering\n");
		maxfd = sock;
		for (i = 0; i < num_user; i++)
			if (client[i].sock > maxfd)
				maxfd = client[i].sock;

		if(select(maxfd+1, &readfds, 0, 0, NULL) < 0) break;
		if(FD_ISSET(sock, &readfds)) {
			client_sock = accept(sock, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len); 
			// client_fsock = accept(fsock, (struct sockaddr *)&file_addr, (socklen_t*)&addr_len); 

			if(client_sock == -1) {
				printf("accept error\n");
				continue;
			}
			printf("client connected\n");
			printf("clinet ip : %s: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

			char client_id[10];
			sprintf(client_id, "%d", client_sock);
			write(client_sock, client_id, strlen(client_id));
			client[num_user].sock = client_sock;
			// client[num_user].fsock = client_fsock;			
			client[num_user].room = 0;
			room_info[0][num_user]=1;
			// strcpy(ip_list[num_user], buf);
			num_user++; //count users
		}
		for(i=0; i<num_user; i++) {
			if(FD_ISSET(client[i].sock, &readfds)) {
				memset(rbuf, 0, BUFSIZE);				
				readlen = read(client[i].sock, rbuf, sizeof(rbuf)-1);
				if(readlen == 0) {
					printf("client disconnected\n");
					FD_CLR(client[i].sock, &readfds);
					close(client[i].sock);
					// close(client[i].fsock);
                    if (i != num_user - 1) {
		                client[i].sock = client[num_user - 1].sock;
		                // client[i].fsock = client[num_user - 1].fsock;
		                //strcpy(ip_list[s], ip_list[num_user - 1]);
	                }  
                    num_user--;
					continue;
				};
				rbuf[readlen-1]='\0';
				if (rbuf[0] == '/') {
					if (strstr(rbuf, "/myroom") != NULL) {
						if (client[i].room==0)
							sprintf(wbuf, "You are in default room\n");
						else
							sprintf(wbuf, "You are in room#%d\n", client[i].room);
					}
					else if (strstr(rbuf, "/member") != NULL) {
						if (client[i].room==0)
							sprintf(wbuf, "You are in default room. Member(s) of current room\n");
						else
							sprintf(wbuf, "You are in room#%d. Member(s) of current room\n", client[i].room);
						sprintf(buf,"%c", '\0');
						for(int j=0; j<num_user; j++) {
							if(room_info[client[i].room][j]==1) {
								if(client[i].sock==client[j].sock) sprintf(buf, "%sClient(%d)(YOU)\n", buf, client[j].sock);
								else sprintf(buf, "%sClient(%d)\n", buf, client[j].sock);
							}
						}
						sprintf(wbuf, "%s%s", wbuf, buf);
					}
					else if (!strncmp(rbuf, "/moveto ", 8)) {
						if (strlen(rbuf)<9) sprintf(wbuf, "you should specify desiring chatting room #\n");
						else {
							sprintf(wbuf, "move to chatting room# %d\n", rbuf[8]-'0');
							room_info[client[i].room][i]=0;
							client[i].room=(rbuf[8]-'0')%10;
							room_info[client[i].room][i]=1;
						}
					}
					else if (!strncmp(rbuf, "/dm", 3)) {
						if (strlen(rbuf)<5) sprintf(wbuf, "you should specify receiver and type some message\n");
						else if (!strncmp(&rbuf[3], " Client", 7) & isdigit(rbuf[10])) {
							if (strlen(rbuf)<12) sprintf(wbuf, "you should type some message\n");
							else {
								sprintf(wbuf, "DM from Client(%d): %s", client[i].sock, &rbuf[12]);
								write(rbuf[10]-'0', wbuf, strlen(wbuf));
								continue;
							}
						}
					}
					else if (strstr(rbuf, "/filelist") != NULL) {
						DIR *d;
						struct dirent *dir;
						bool flag=true;
						d = opendir("./files");
						if (d) {
							sprintf(wbuf, "available file list\n");
							while ((dir = readdir(d)) != NULL) {	
								if(dir->d_name[0]=='.') continue;
								sprintf(wbuf, "%s\t-%s\n", wbuf, dir->d_name);
								flag=false;
							}
							if(flag) sprintf(wbuf, "there is no file in directory\n");
							closedir(d);
						}
						else sprintf(wbuf, "There is no available files\n");
					}
					else if (!strncmp(rbuf, "/upload", 7)) {
						char filename[BUFSIZE];
						int namelen=strlen(rbuf)-8;
						strncpy(filename, rbuf+8, namelen);
						filename[namelen-1]='\0';
						printf("receiving file: %s\n", filename);
						listen(fsock,5);
						client_fsock = accept(fsock, (struct sockaddr *)&file_addr, (socklen_t*)&addr_len); 
						if(client_fsock == -1) {
						// if(client[i].fsock == -1) {
							printf("accept error\n");
							continue;
						}
						size_t datasize;
						int res_dir=mkdir("./files",0755);
						sprintf(rbuf, "files/%s", filename);
						printf("%s\n", rbuf);
						FILE* fd = fopen(rbuf, "w");
						char buffer[BUFSIZE];
						int ind=0;
						sprintf(buffer, "file socket connected");
						write(client_fsock, buffer, 22);
						// write(client[i].fsock, buffer, 22);

						memset(buffer, 0, BUFSIZE);
						while ((datasize = read(client_fsock, buffer, BUFSIZE-1)) > 0) {
						// while ((datasize = read(client[i].fsock, buffer, BUFSIZE-1)) > 0) {
							ind = fwrite(&buffer, 1, datasize, fd);
							if(ind < datasize) {
						 		printf("File write failed.\n");;
							}
							memset(buffer, 0, BUFSIZE);
						}
						printf("writing done\n");
						fclose(fd);
						sprintf(wbuf, "file receive @ server\n");
						close(client_fsock);
						printf("file socket closed\n");
						memset(rbuf, 0, BUFSIZE);	
					}
					else if (!strncmp(rbuf, "/download", 9)) {
						char filename[BUFSIZE];
						int namelen=strlen(rbuf)-10;
						strncpy(filename, rbuf+10, namelen);
						filename[namelen]='\0';
						printf("sending file: %s\n", filename);
						listen(fsock,5);
						client_fsock = accept(fsock, (struct sockaddr *)&file_addr, (socklen_t*)&addr_len); 
						if(client_fsock == -1) {
						// if(client[i].fsock == -1) {
							printf("accept error\n");
							continue;
						}
						sprintf(rbuf, "files/%s", filename);
						FILE* fd = fopen(rbuf, "r");
						char buffer[BUFSIZE];
						sprintf(buffer, "file socket connected");
						write(client[i].fsock, buffer, 22);
						int bytes_read;

						while (feof(fd) == 0) {
							bytes_read = fread(&buffer, sizeof(char), BUFSIZE-1, fd);
							write(client_fsock, buffer, bytes_read);
							// write(client[i].fsock, buffer, bytes_read);
							// memset(buffer, 0, BUFSIZE);
						}
						printf("writing done\n");
						fclose(fd);
						sprintf(wbuf, "file send @ server\n");
						close(client_fsock);
						printf("file socket closed\n");
						memset(rbuf, 0, BUFSIZE);	
					}
					else if (strstr(rbuf, "/help") != NULL) {
						sprintf(wbuf, "/myroom : shows your current chatting room info\n");
						sprintf(wbuf, "%s/member : shows member of your current chatting room\n", wbuf);
						sprintf(wbuf, "%s/moveto (0~9) : move to specified chatting room (default:0)\n", wbuf);
						sprintf(wbuf, "%s/dm (Target client) (message) : send sirect message to specified target\n", wbuf);						
					}
					else {
						sprintf(wbuf, "it is not a valid command. use /help to see available commands\n");
					}
					write(client[i].sock, wbuf, strlen(wbuf));
				}
				else {
					for(j=0; j<num_user; j++) {
						if(i==j) continue;
						if(client[i].room==client[j].room)	write(client[j].sock, rbuf, readlen);
					}
				}
				printf("%s", rbuf);
			}
		}
	}
	return 0;
}
