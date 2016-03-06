//roboman2444
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define FALSE 0
#define TRUE 1

#define URLPLACE 0
#define USERPLACE 1
#define PASSPLACE 2

FILE *inread;

#define LBALIGN 32

char * lb = 0;
unsigned int lbs = 0;
unsigned int lbp = 0;

char * fields[] = {"url", "user", "pass", "name", "grouping", "fav", 0};
unsigned int ofs[] = {0, 0, 0, 0, 0, 0, 0};
			//pass, user, grouping, fav, url, name, terminator
unsigned int writes[] = {3, 2, 5, 6, 1, 4, 0};



int main(const int argc, const char ** argv){
	inread = stdin;
	if(argc > 1){
		FILE *n = fopen(argv[1], "r");
		if(n){ inread = n; printf("Readin from file %s\n", argv[1]); }
		else printf("File %s cant be opened, reading from stdin\n", argv[1]);
	}

	char c = ' ';
	while(c != EOF){
		int fldplace = 1;
		lbp = 0;
		while((c = fgetc(inread)) != EOF){
			if(!c || c == '\n') break;
			if(lbp >= lbs){
				lbs = (lbp + LBALIGN) & ~(LBALIGN-1);
				lb = realloc(lb, lbs);
			}
			if(c == ','){
				c = 0;
				if(fields[fldplace]) ofs[fldplace++] = lbp+1;
			}
			lb[lbp++] = c;

		}
		printf("Output for line: ");
		int i;
		for(i = 0; i < lbp; i++) putc(lb[i] ? lb[i] : ',',stdout);
		putc('\n',stdout);
		for(i = 0; i < fldplace; i++) if(*(lb+ofs[i]))printf("\t%s:%s\n", fields[i], lb + ofs[i]);
		//enter dat shit
		//create thing
		size_t bufl = 0;
		if(fldplace >4 && *(lb+ofs[4])) bufl+= strlen(lb+ofs[4]);
		if(fldplace >1 && *(lb+ofs[1])) bufl+= strlen(lb+ofs[1]);
		if(!bufl){
			printf("errorro\n");
			continue;
		}
		bufl += 10;
		char *outn = malloc(bufl);
		sprintf(outn, "%s/%s", lb+ofs[4], (fldplace >1 && *(lb+ofs[1])) ? lb+ofs[1] : "unnamed");
		printf("creating thing %s\n", outn);
		//set up pid
		int fd[2];
		pipe(fd);
		int ret;

		pid_t cpid;
		if((cpid = fork()) == -1){
			printf("We fucked up\n");
			return TRUE;
		}
		if(cpid){ // parent
			close(fd[0]);
			//password
			int i;
			for(i = 0;writes[i]; i++) dprintf(fd[1], "%s\n", (fldplace > writes[i]-1 && *(lb+ofs[writes[i]-1])) ? lb+ofs[writes[i]-1] : "none");
			close(fd[1]);
			waitpid(cpid, &ret, 0);
		} else { //child
			close(fd[1]);
//			close(0);
			dup2(fd[0],0);
			char * oots[] = {"pass", "insert", "-m", "-f", outn, 0};
			execvp("pass", oots);
			exit(0);
		}
	}
	if(lb) free(lb);

	return FALSE;
}
