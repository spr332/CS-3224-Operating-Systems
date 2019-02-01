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
	neu->current=-1;
	return neu;
}

int lsappend(struct long_string *ls, char l){
	while (ls->current > 510){
		if(ls->next == NULL){ls->next = lsnew();}
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
		if(L->current<1){break;}
		m[0]=L->data[i];
		if(m[0] != '\0'){
			printf(1, m);
		}else{;return 1;}
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

int main(){
	char l = 'l';
	struct long_string * ls1 = lsnew();
	struct long_string * ls2 = lsnew();
	int i;
	for(i=0;i<2030;i++){
		lsappend(ls1,l);
	}
	for(i=0;i<2030;i++){
		lsappend(ls2,l);
	}
	printf(1,lscompare(ls1,ls2)?"True":"False");
	lsprint(ls2);
	exit();
}