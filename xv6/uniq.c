#include "types.h"
#include "stat.h"
#include "user.h"
#define NULL 0

struct long_string{
	char * data;
	int current;
	struct long_string *next;
};

struct long_string * lsnew(){
	struct long_string * neu = malloc(sizeof(struct long_string)) ;
	neu->data = malloc(sizeof(char[512]));
	neu->data[511]='\0';
	neu->next=NULL;
	neu->current=-1;
	return neu;
}

int lsappend(struct long_string *ls, char l){
	if( l == '\r' ){return 0;}//Carriage returns ignored.
	while (ls->current > 510){
		if(ls->next == NULL){ls->next = lsnew();}
		ls = ls->next;
	}
	ls->current++;
	ls->data[ls->current] = l;
	ls->data[ls->current + 1] = '\0'; //511 will always be \0
	return 1;
}
char tolower(char c){
	if ( strchr("ABCDEFGHIJKLMNOPQRSTUVWXYZ",c)  ){
		return (c+32);
	}
	return c;
}
int lscompare(struct long_string* L, struct long_string* R, int IFLAG){
	if((L == NULL) && (R == NULL)){return 1;}
	if((L == NULL) || (R == NULL)){return 0;}
	int i;
	char iflag_l, iflag_r;
	
	comp:
	for( i=0; i<511; i++){
		if(IFLAG){ //if (L->data[i] != R->data[i]){return 0;}
			iflag_l = L->data[i];
			iflag_r = R->data[i];
			iflag_l = tolower(iflag_l);
			iflag_r = tolower(iflag_r);
			//printf(1,"\n **\n %d",(int)iflag_l);
			if(iflag_l!=iflag_r){return 0;}
			goto iskip;
		}	
		if (L->data[i] != R->data[i]){
			return 0;
		}
		
		iskip:
		if(L->data[i] == '\0'){
			return 1;
		}
	}
	if((L->next == NULL) && (R->next == NULL)){return 1;}
	if((L->next == NULL) || (R->next == NULL)){return 0;}
	L = L->next;
	R = R->next;
	goto comp;
}

int lsprint(struct long_string* L){
	int i;
	char m[2] = "\0\0";
	topprint:
	for(i=0; i<=L->current; i++){
		m[0]=L->data[i];
		if(m[0] != '\0'){
			printf(1, m);
		}else{return 1;}
	}
	if(L->next != NULL){
		L=L->next;
		goto topprint;
	}
	return 1;
}

int lsdelete(struct long_string* L){
	struct long_string * R = L->next;
	if(R!=NULL){lsdelete(R);}
	free(L->data);
	free(L);
	return 1;
}



//#####
void uniq(int fd, int CFLAG, int DFLAG, int IFLAG){
	int n = 0;
	char buf[2];
	buf[1]='\0';
	int flagfirst = 0;
	int counter=1;
	struct long_string * CurrentLine = NULL;
	struct long_string * Line;
	getLine:
	Line = lsnew();
	while((n = read(fd, buf, sizeof(char) )) > 0 ){
		lsappend(Line, buf[0]);
		if (buf[0] == '\n' || buf[0] == '\0'){break; }
	}
	if(! flagfirst){
		flagfirst=1;
		CurrentLine = Line;
		Line = NULL;
		goto getLine;
	}
	
	if (lscompare(CurrentLine, Line, IFLAG)){
		counter++;
	}
	else{
		if( DFLAG && counter == 1){ goto dskip; }
		if( CFLAG ){printf(1,"%d ",counter);}
		lsprint(CurrentLine);
		dskip:
		counter = 1;
	}
	lsdelete(CurrentLine);
	CurrentLine = Line;
	if(n){goto getLine;}
	if( (counter-1 || !DFLAG) && !( counter&&DFLAG )) {
		if( CFLAG ){printf(1,"%d ",counter);}
		lsprint(CurrentLine);
	}
	printf(1,"\n");
}

int main(int argc, char *argv[]){
	int fd;
	fd = 0;
	int fl;
	int CFLAG, DFLAG, IFLAG;
	CFLAG = DFLAG = IFLAG = 0;
	
	if (argc == 1){goto start;}
	
	int i;
	for(i=1; i<argc; i++){
		if (argv[i][0] != '-'){fl=i;}
		else{
			if (argv[i][1]=='c' || argv[i][1]=='C'){CFLAG=1;}
			else if (argv[i][1]=='d' || argv[i][1]=='D'){DFLAG=1;}
			else if (argv[i][1]=='i' || argv[i][1]=='I'){IFLAG=1;}
		}
	}
	
	if((fd = open(argv[fl], 0)) < 0){
      printf(1, "uniq: cannot open %s\n", argv[fl]);
      exit();
    }
	
	start:
	uniq(fd, CFLAG, DFLAG, IFLAG);
	close(fd);
	exit();
}