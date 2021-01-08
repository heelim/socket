#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> 
#include <dirent.h>
#include <pthread.h>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFSIZE 1024

#ifdef THREADING
void* upload(void*);
#endif

int main(int argc, char* argv[]) {
	char ip_addr[] = "127.0.0.1"; //server ip addr
	int port = 8888; //server port num

	int sock;
	struct sockaddr_in addr;
	int addrlen, readlen, recvbyte, maxbuf;

	char rbuf[BUFSIZE];
	char wbuf[BUFSIZE];
	char buf[BUFSIZE];

	int maxfds;
	fd_set fds;
#ifdef THREADING
	pthread_t thread;
#endif

	int fsock;
	struct sockaddr_in file_addr;
	fsock = socket(AF_INET, SOCK_STREAM, 0);
	if(fsock < 0) {
		printf("socket init error\n");
		return -1;
	}
	memset(&file_addr, 0, sizeof(file_addr));
	file_addr.sin_family = AF_INET;
	file_addr.sin_addr.s_addr = inet_addr(ip_addr);
	file_addr.sin_port = htons(9999);

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0) {
		printf("socket init error\n");
		return -1;
	}
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(ip_addr);
	addr.sin_port = htons(port);

	if(connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
		printf("connection error");
		close(sock);
		return -1;
	}
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
			if ((readlen = read(sock, rbuf, BUFSIZE)) > 0) {
				rbuf[readlen] = '\0';
				fprintf(stderr, "\r%s", rbuf);	
			}
		}
		if (FD_ISSET(0, &fds)) {
			if (fgets(buf, BUFSIZE, stdin)) {
				if(strlen(buf)<2) {
					//fprintf(stderr,"\r");
					continue;
				}
				if(strstr(buf, "/quit") != NULL) {
					printf("Good bye.\n");
					close(sock);
					exit(0);
				}
				if(strstr(buf, "/upload") != NULL) {
// #ifdef THREADING
// 					char arg[2];
// 					sprintf(arg,"%d",sock);
// 					pthread_create(&thread, NULL, upload, (void *)arg);
// 					pthread_detach(thread);
// #else
// 					DIR *d;
// 					struct dirent *dir;
// 					char filename[BUFSIZE];
// 					bool flag=true;
// 					d = opendir("./");
// 					if (d) {
// 						printf("file list of current directory\n");
// 						while ((dir = readdir(d)) != NULL) {	
// 							if(dir->d_name[0]=='.') continue;
// 							printf("\t-%s\n", dir->d_name);
// 							flag=false;
// 						}
// 						if(flag) printf("there is no file in directory\n");
// 						closedir(d);
// 					}
// 					fprintf(stderr, "\rto upload, input filename or type q to cancel : ");
// 					fgets(filename, BUFSIZE, stdin);
// 					// if(!strncmp(filename, "q", 2)) {
// 					if(strlen(filename) < 2 | strstr(filename, "q") != NULL) {
// 						continue;
// 					}
// 					sprintf(wbuf, "/upload %s\n", filename);
// 					if (write(sock, wbuf, strlen(wbuf)) < 0) 
// 						printf("Error : Write error on socket.\n");
// 					if(connect(fsock, (struct sockaddr *)&file_addr, sizeof(file_addr)) == -1) {
// 						printf("file connection error\n");
// 						close(fsock);
// 						continue;
// 					}		
// 					int bytes_read;
// 					char buffer[256];
// 					readlen = read(sock, buffer, 256);
// 					buffer[readlen]='\0';
// 					printf("%s\n", buffer);

// 					printf("file to open : %s\n", filename);
// 					sprintf(filename, "./%s", filename);
// 					FILE *fd = fopen(filename, "r");

					
// 					while (!feof(fd)) {
// 						if ((bytes_read = fread(&buffer, 1, sizeof(buffer), fd)) > 0)
// 							write(fsock, buffer, bytes_read);
// 						else
// 							break;
// 					}
// 					close(fsock);
// 					fclose(fd);
// 					printf("done sending file\n");
// #endif
				}
				else {
					if(buf[0]== '/') {
					sprintf(wbuf, "%s\n", buf);
					}
					else sprintf(wbuf, "Client(%s): %s\n", str, buf);
					if (write(sock, wbuf, strlen(wbuf)) < 0) 
						printf("Error : Write error on socket.");
				}
			}
		}
	}
	return 0;
}
#ifdef THREADING
void* upload(void* arg) {
	char socket[10];
	sprintf(socket, "%s", (char*)arg);
	printf("%s\n", socket);
	DIR *d;
	struct dirent *dir;
	char filename[BUFSIZE];
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
	fprintf(stderr, "\rto upload, input filename or type q to cancel : ");
	fgets(filename, BUFSIZE, stdin);
	// if(!strncmp(filename, "q", 2)) {
	if(strlen(filename) < 2 | strstr(filename, "q") != NULL) {
		return NULL;
	}
	printf("file to open : %s\n", filename);
	// FILE *fd = fopen(filename, "rb");
	// size_t rret, wret;
	// int bytes_read;
	// while (!feof(fd)) {
	// 	if ((bytes_read = fread(&buffer, 1, BUFFER_SIZE, fd)) > 0)
	// 		send(sock, buffer, bytes_read, 0);
	// 	else
	// 		break;
	// }
	// fclose(fd);
}
#endif