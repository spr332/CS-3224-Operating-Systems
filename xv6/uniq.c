#include "types.h"
#include "stat.h"
#include "user.h"
#define NULL 0

struct long_string{
	char data[512];
	int current;
	struct long_string *next;
};

struct long_string * lsnew(){
	struct long_string * neu = malloc(sizeof(struct long_string)) ;
	neu->data[511]='\0';
	neu->next=NULL;
	neu->current=0;
	return neu;
}

int lsappend(struct long_string *ls, char l){
	while (ls->current > 510){
		ls->next = lsnew();
		ls = ls->next;
	}
	ls->current++;
	ls->data[ls->current] = l;
	ls->data[ls->current + 1] = '\0'; //511 will always be \0
	return 1;
}

int lscompare(struct long_string* L, struct long_string* R){
	if((L == NULL) && (R == NULL)){return 1;}
	if((L == NULL) || (R == NULL)){return 0;}
	int i;
	comp:
	for( i=0; i<511; i++){
		if (L->data[i] != R->data[i]){
			return 0;
		}
		if(L->data[i] == '\0'){
			return 1;
		}
	}
	if((L->next == NULL) && (R->next == NULL)){
		return 1;
	}
	L = L->next;
	R = R->next;
	goto comp;
}
int lsprint(struct long_string* L){
	int i;
	topprint:
	for(i=0; i<=L->current; i++){
		if(L->data[i] != '\0'){
			printf(1, &L->data[i]);
		}
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
	free(L);
	return 1;
}



//#####

int main(int argc, char *argv[]){
	int n, fd;
	n = fd = 0;
	char buf[1];
	if((fd = open(argv[1], 0)) < 0){
      printf(1, "uniq: cannot open %s\n", argv[1]);
      exit();
    }
	
	int counter=0;
	struct long_string* CurrentLine = NULL;
	struct long_string * Line;
	getLine:
	Line = lsnew();
	while((n = read(fd, buf, sizeof(buf) )) > 0 ){
		lsappend(Line, buf[0]);
		if (buf[0] == '\n'){break;}
	}
	
	if (n<=0){goto exit;}
	
	if (lscompare(CurrentLine, Line)){
		counter++;
	}
	else{
		lsprint(CurrentLine);
		counter = 0;
		lsdelete(CurrentLine);
		CurrentLine = Line;
	}
	goto getLine;
	
	exit:
	exit();
}