#include <iostream>

#include "common.h"
#include "CommunicationManager.h"

#define MAXUSER 50
#define MAXROOM 10


char rbuf[BUFSIZE]; 
char wbuf[BUFSIZE];
char buf[BUFSIZE];

int num_user=0;
int room_info[MAXROOM][MAXUSER]={0,};
struct client_info client[MAXUSER];
int addr_len; 

void room(int);
void list_files(char*);
void upload(int);
void download(int);
void help();

CommunicationManager server;

int main(int argc, char* argv[]) {


	int port, fport;
	int sock, client_sock;
	struct sockaddr_in client_addr;
	int fsock; 

	port = 8888; 
	fport = 9999;
	sock = server.tcp_listen(INADDR_ANY, port, 5);
	fsock = server.tcp_listen(INADDR_ANY, fport, 5);
	
	int readlen;
	int maxfd = 0;
	fd_set readfds;
	int i, j;
	addr_len = sizeof(client_addr); 
	printf("waiting for client..\n"); 

	while(1){
		FD_ZERO(&readfds);
		FD_SET(sock, &readfds);
		for (i = 0; i < num_user; i++)
			FD_SET(client[i].sock, &readfds);

		maxfd = sock;
		for (i = 0; i < num_user; i++)
			if (client[i].sock > maxfd)
				maxfd = client[i].sock;

		if(select(maxfd+1, &readfds, 0, 0, NULL) < 0) break;
		if(FD_ISSET(sock, &readfds)) {
			client_sock = accept(sock, (struct sockaddr *)&client_addr, (socklen_t*)&addr_len); 
			if(client_sock == -1) {
				printf("accept error\n");
				continue;
			}
			printf("client connected\n");
			printf("clinet ip : %s: %d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
			char client_id[10];
			sprintf(client_id, "%d", client_sock);
			send(client_sock, client_id, strlen(client_id), 0);
			client[num_user].sock = client_sock;
			client[num_user].room = 0;
			room_info[0][num_user]=1;
			// strcpy(ip_list[num_user], buf);
			num_user++; //count users
		}
		for(i=0; i<num_user; i++) {
			if(FD_ISSET(client[i].sock, &readfds)) {
				memset(rbuf, 0, BUFSIZE);				
				readlen = recv(client[i].sock, rbuf, sizeof(rbuf)-1, 0);
				if(readlen == 0) {
					printf("client disconnected\n");
					FD_CLR(client[i].sock, &readfds);
					close(client[i].sock);
					if (i != num_user - 1) {
						client[i].sock = client[num_user - 1].sock;
						// client[i].fsock = client[num_user - 1].fsock;
						// strcpy(client[i].ip, client[num_user - 1].ip);
					}
					num_user--;
					continue;
				};
				rbuf[readlen-1]='\0';
				if (rbuf[0] == '/') {
					if (strstr(rbuf, "/myroom") != NULL) {
						room(client[i].room);
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
								send(rbuf[10]-'0', wbuf, strlen(wbuf), 0);
								continue;
							}
						}
					}
					else if (strstr(rbuf, "/filelist") != NULL) {
						list_files("./files");
					}
					else if (!strncmp(rbuf, "/upload", 7)) {
						upload(fsock);
					}
					else if (!strncmp(rbuf, "/download", 9)) {
						download(fsock);
					}
					else if (strstr(rbuf, "/help") != NULL) {
						help();
					}
					else {
						sprintf(wbuf, "it is not a valid command. use /help to see available commands\n");
					}
					send(client[i].sock, wbuf, strlen(wbuf), 0);
				}
				else {
					for(int j=0; j<num_user; j++) {
						if(i==j) continue;
						if(client[i].room==client[j].room)	send(client[j].sock, rbuf, readlen, 0);
					}
				}
				printf("%s", rbuf);
			}
		}
	}
	return 0;
}

void room(int room_num) {
	if (room_num==0)
		sprintf(wbuf, "You are in default room\n");
	else
		sprintf(wbuf, "You are in room#%d\n", room_num);
}
void list_files(char* target_dir) {
	DIR *d;
	struct dirent *dir;
	bool flag=true;
	d = opendir(target_dir);
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
void upload(int fsock) {
	int client_fsock;
	struct sockaddr_in file_addr;
	char filename[BUFSIZE];
	int namelen=strlen(rbuf)-8;
	strncpy(filename, rbuf+8, namelen);
	filename[namelen-1]='\0';
	printf("receiving file: %s\n", filename);

	size_t datasize;
	int res_dir = mkdir("./files", 0755);

	sprintf(rbuf, "files/%s", filename);
	printf("%s\n", rbuf);

	client_fsock = accept(fsock, (struct sockaddr *)&file_addr, (socklen_t*)&addr_len); 
	if(client_fsock == -1) {
		printf("accept error\n");
	}

	server.tcp_send_msg(client_fsock, "file socket connected");

	server.tcp_recv_file(client_fsock, rbuf);

	sprintf(wbuf, "file receive @ server\n");
	printf("file socket closed\n");
	memset(rbuf, 0, BUFSIZE);
}
void download(int fsock) {
	int client_fsock;
	struct sockaddr_in file_addr;
	char filename[BUFSIZE];
	int namelen=strlen(rbuf)-10;
	strncpy(filename, rbuf+10, namelen);
	filename[namelen]='\0';
	printf("sending file: %s\n", filename);
	client_fsock = accept(fsock, (struct sockaddr *)&file_addr, (socklen_t*)&addr_len); 
	if(client_fsock == -1) {
		printf("accept error\n");
	}
	sprintf(rbuf, "files/%s", filename);

	server.tcp_send_msg(client_fsock, "file socket connected");

	server.tcp_send_file(client_fsock, rbuf);

	sprintf(wbuf, "file send @ server\n");
	printf("file socket closed\n");
	memset(rbuf, 0, BUFSIZE);	
}
void help() {
	sprintf(wbuf, "/myroom : shows your current chatting room info\n");
	sprintf(wbuf, "%s/member : shows member of your current chatting room\n", wbuf);
	sprintf(wbuf, "%s/moveto (0~9) : move to specified chatting room (default:0)\n", wbuf);
	sprintf(wbuf, "%s/dm (Target client) (message) : send sirect message to specified target\n", wbuf);
	sprintf(wbuf, "%s/filelist : shows downloadable file list\n", wbuf);
	sprintf(wbuf, "%s/upload : shows uploadable file list and upload selected file\n", wbuf);
	sprintf(wbuf, "%s/download (filename) : download file\n", wbuf);
}