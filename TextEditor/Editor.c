#include<stdio.h>
#include<string.h>
#include <fcntl.h> 
#include<errno.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
//char file_adr[500]="/home/shounakghatak/Desktop/out";
char file_adr[500];
char buff[10000];
FILE *fptr;
struct flock fl;
struct flock fl1;
struct termios orig_termios;
void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enableRawMode() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disableRawMode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}


void init(){
	write(STDOUT_FILENO, "\x1b[2J", 4);
  	write(STDOUT_FILENO, "\x1b[H", 3);
  	int y;
  	for (y = 0; y < 15; y++) {
    write(STDOUT_FILENO, "-\r\n", 3);
  }
  write(STDOUT_FILENO, "\x1b[H", 3);
  //enableRawMode();
}
void edit(){
	char append[1000];
	FILE *fptr1=fopen(file_adr,"a+");
	//scanf("%[^\n]%*c", append);
	char c;
  	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
  		//printf("%c",c);
  		//strncat(append,&c,1);
  		fprintf(fptr1, "%c",c );
  	}
	//fprintf(fptr1,"%s\n",append);
	//fclose(fptr1);
}
void load(){
	char data[800];
while(fscanf(fptr,"%[^\n]\n",data)!=EOF){
	 //write(STDOUT_FILENO, buff, sizeof(data));
	strcat(buff,data);
	strcat(buff,"\n");
}
write(STDOUT_FILENO, buff, sizeof(buff));
//fclose(fptr);
edit();
}



int main(){
	printf("TextEditor running\n");
	int i=1;
	int n=1;
	while(i<=n){
		i++;
		printf("Enter file address: \n");
		scanf("%[^\n]%*c", file_adr);
		//printf("%s\n",file_adr);
		fptr=fopen(file_adr,"r+");
		if(fptr==NULL){
			//printf("Address Not Possible/Unable to open file\n");
			fptr=fopen(file_adr,"a+");
			if(fptr==NULL){
				printf("Address Not Possible/Unable to open file\n");
				continue;
			}
			
		}
		//perror(errno);
		int fd=fileno(fptr);
		printf("%d\n",fd);
		
		init();
		
		memset(&fl, 0, sizeof(fl));
		memset(&fl1, 0, sizeof(fl1));
		fl.l_type = F_RDLCK;
		fl1.l_type = F_WRLCK;

		
		fl.l_whence = SEEK_SET; 
		fl.l_start = 0;         
		fl.l_len = 0;          
	                       

		fl.l_pid = 0;

		fl1.l_whence = SEEK_SET; 
		fl1.l_start = 0;         
		fl1.l_len = 0;           
                        

		fl1.l_pid = 0;
		if (fcntl(fd, F_GETLK, &fl1) == -1) {
			printf("%s\n",strerror(errno));
			continue;
		} 
		if(fl1.l_type!=F_UNLCK){
	 	printf("Warning!File in Use\n");
	 	}
	 	if (fcntl(fd, F_SETLK, &fl) == -1) {
			printf("%s\n",strerror(errno));
			continue;
		} 

		load();
		//fclose(fptr);
		if (fcntl(fd, F_SETLK, &fl) == -1) {
			printf("%s\n",strerror(errno));
    		continue;
		}
		 system("clear");


	}
	/*while(1){

	}*/
	//clrscr();
	//write(STDOUT_FILENO, "\x1b[H", 3);
	system("clear");
	printf("TextEditor terminated\n");
	
	//write(STDOUT_FILENO, "\x1b[H", 3);
	return 0;
}