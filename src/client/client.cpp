#include "common.h"
#include "CommunicationManager.h"

char rbuf[BUFSIZE];
char wbuf[BUFSIZE];
char buf[BUFSIZE];

int main(int argc, char* argv[]) {

	CommunicationManager client;

	char ip_addr[] = "127.0.0.1"; //server ip addr
	int port = 8888; //server port num
	int fport = 9999;
	int sock;
	int fsock;
	sock = client.tcp_connect(ip_addr, port);

	int readlen;

	int maxfds;
	fd_set fds;

	char str[10];
	readlen=read(sock, str, 10-1);
	str[readlen] = '\0';
	fprintf(stderr, "Server connected, you are Client(%s)\n", str);

	maxfds = sock + 1;
	FD_ZERO(&fds);
	while(1) {
		fprintf(stderr, "ME: ");
		FD_SET(0, &fds);
		FD_SET(sock, &fds);
		if (select(maxfds, &fds, NULL, NULL, NULL) < 0){
			printf("select fail");
			exit(0);
		}
		if (FD_ISSET(sock, &fds)) {
			if ((readlen = recv(sock, rbuf, BUFSIZE, 0)) > 0) {
				rbuf[readlen] = '\0';
				fprintf(stderr, "\r%s", rbuf);	
			}
		}
		if (FD_ISSET(0, &fds)) {
			if (fgets(buf, BUFSIZE, stdin)) {
				if(strlen(buf)<2) {
					continue;
				}
				else if(strstr(buf, "/quit") != NULL) {
					client.tcp_close(sock);

					printf("Good bye.\n");
					exit(0);
				}
				if(strstr(buf, "/upload") != NULL) {
					DIR *d;
					struct dirent *dir;
					bool flag=true;
					d = opendir("./");
					if (d) {
						printf("file list of current directory\n");
						while ((dir = readdir(d)) != NULL) {	
							if(dir->d_name[0]=='.') continue;
							printf("\t-%s\n", dir->d_name);
							flag=false;
						}
						if(flag) printf("there is no file in directory\n");
						closedir(d);
					}
					printf("\rto upload, input filename or type q to cancel : ");

					char filename[BUFSIZE];
					fgets(filename, BUFSIZE, stdin);

					if(strlen(filename) < 2 | strstr(filename, "q") != NULL) {
						continue;
					}
					sprintf(wbuf, "/upload %s\n", filename);
					if (send(sock, wbuf, strlen(wbuf), 0) < 0) 
						printf("Error : Write error on socket.\n");

					fsock = client.tcp_connect(ip_addr, fport);
					char buffer[BUFSIZE];
					readlen = recv(fsock, buffer, 22, 0);
					buffer[readlen]='\0';
					printf("%s\n", buffer);
					memset(buffer, 0, BUFSIZE);
					filename[strlen(filename)-1]='\0';
					FILE *fd = fopen(filename, "r");
					while (feof(fd) == 0) {
						readlen = fread(&buffer, sizeof(char), BUFSIZE-1, fd);
						send(fsock, buffer, readlen, 0);
						memset(buffer, 0, BUFSIZE);
					}
					client.tcp_close(fsock);
					fclose(fd);
					printf("file send @ client\n");
				}
				else if(!strncmp(buf, "/download", 9)) {
					char filename[BUFSIZE];
					int namelen = strlen(buf)-11;
					if(namelen<1) {
						printf("you should enter filename\n");
						continue;
					}
					strncpy(filename, buf+10, namelen);
					filename[namelen]='\0';
					sprintf(wbuf, "/download %s\n", filename);
					if (send(sock, wbuf, strlen(wbuf), 0) < 0) 
						printf("Error : Write error on socket.\n");

					fsock = client.tcp_connect(ip_addr, fport);
					char buffer[BUFSIZE];
					readlen = recv(fsock, buffer, 22, 0);
					buffer[readlen]='\0';
					printf("%s\n", buffer);
					memset(buffer, 0, BUFSIZE);
					filename[strlen(filename)]='\0';
					FILE *fd = fopen(filename, "w");
					size_t datasize;
					int ind=0;
					memset(buffer, 0, BUFSIZE);
					while ((datasize = recv(fsock, buffer, BUFSIZE-1, 0)) > 0) {
						ind = fwrite(&buffer, 1, datasize, fd);
						if(ind < datasize) {
					 		printf("File write failed.\n");;
						}
						memset(buffer, 0, BUFSIZE);
					}
					client.tcp_close(fsock);
					fclose(fd);
					printf("file receive @ client\n");			
				}
				else {
					if(buf[0]== '/') {
						sprintf(wbuf, "%s\n", buf);
					}
					else sprintf(wbuf, "Client(%s): %s\n", str, buf);
					if (send(sock, wbuf, strlen(wbuf), 0) < 0) 
						printf("Error : Write error on socket.");
				}
			}
		}
	}
	return 0;
}
