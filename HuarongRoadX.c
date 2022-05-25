/*3-PUZZLE SOLVER X
**************************************
*三阶数字华容道求解器，可以
*1 输入一个开局，然后得到一个最优解
*2 统计所有开局的最优解分布情况
*3 保存所有开局的最优解到文件
*4 从文件读取任意开局的最优解
*@Catamint
**************************************
*/

/*
**************
已知的问题:
1.square转为整数后储存,开头的0被抹去了
*/

#include<stdio.h>
#include<conio.h>
#include<stdlib.h> 
#include<time.h>
#include<string.h>
#include<math.h>

#define ORDER 3  //(阶数)
#define TQS 9    // total quantity of squares. (ORDER^2)
#define FLOOR 33 // maximum of steps.

typedef struct ListNode{
    int *square; // [FLOOR] (宫格)
    int zero;    // Location of blank square.
    char *path;  // [TQS]
    struct ListNode *next;
    struct ListNode *last;
} node;

struct answer{
    int square;
    int step;
    int amount_of_paths;
    char *paths[10];
}answers[362881];

int step = 0;                //层数/步数.
int total_squares = 0;       // amount of squares been visited.
int visitList[362881] = {0}; // if visited turn to 1.
int stepsList[FLOOR] = {0};  //存放步数为[]的数量.
int openingSquare[TQS];      //输入的square,即开局.
int rangeSquare[TQS] = {1, 2, 3, 4, 5, 6, 7, 8, 0};

//流程：
void inatialization();
int getMode();
int scanTheSquare3();
void do_mode_1();
node *printNode(node *node);
void do_mode_2();
void do_mode_3();
int append_answer(int square, int id, char *path);
int print_array(int *array, int lenth, FILE *file);
int print_answer_list(FILE *file);
int do_mode_4();
int split(char *line, char ans[][FLOOR * 5]);
int array_to_int(int *array, int lenth);
//链表结构：
node *newList();
node *newNode(int *square, char *path, int zero);
void LinkNodeIfront(node *parentNode, node *thisNode);
void LinkNodeIback(node *parentNode, node *thisNode);
void deleteNode(node *thisNode);
//节点结构：
int *blankSquare();
int *newSquare(int *parentSquare);
int *swap(int *square, int zero, int neighbour);
char *blankPath();
char *newPath(char *parentPath);
char *add(char *path, int zero);
//数据操作：
node *bfs_to_file(node *headNode, char mode);
int haveAnswer(int square[]);
int zero_of(int *square);
int opening_is(int *square);
int indexOf(int square[]);
int visited(int square[]);
int *neighbourOf(int zero);

//mode1: 查找最优解
void do_mode_1(){
    node* answer = NULL;
    if(opening_is(rangeSquare)){
    printf("Have completed.....\n"); //首次完成检查.
    }else if(!haveAnswer(openingSquare)){
        printf("Absolutely have no answer!"); //有解检查.
    }else{
        answer = bfs_to_file(newList(),'1');
        if(answer!=NULL){
            printf("Completed!\n");
            printNode(answer);
        }else{
            printf("Error.\n");
        }
        printf("\n");
    }
}
//[mode 1] print the answer.
node* printNode(node* node) {
    printf("square =");
    for(int i=0;i<TQS;i++){
        printf(" %d",node->square[i]);
    }
    printf(";\npath of '0' = ");
    for(int i=step;i>=0;i--){
        printf("%c->",node->path[i]+1);
    }
    printf("9;\n");
    printf("%d steps in total.\n", step+1);
    printf("\n");
    return node;
}

//mode2: 统计最优解分布
void do_mode_2(){
    bfs_to_file(newList(),'2');
    printf("\n _______________ \n");
    printf("|steps\t|amount\t|\n");
    for(int i=0;i<FLOOR;i++){
        printf("|%d\t|%d\t|\n",i+1,stepsList[i]);
    }
    printf(" --------------- \n");
    printf("(9! / 2 = 181440;)\n");
    printf("Have visited %d squares in total.\n\n",total_squares);
}

//mode3: 统计结果并保存到文件
void do_mode_3(){
    FILE* answers_text=fopen("answers.csv","w");
    bfs_to_file(newList(),'3');
    print_answer_list(answers_text);
    fclose(answers_text);
}
//[mode3] save answers in memory.
int append_answer(int square, int id, char* path){
    answers[id].square=square;
    answers[id].step=step;
    for(int i=0;i<10;i++){
        if(answers[id].paths[i]==NULL){
            answers[id].paths[i]=path;
            return 0;
        }
    }
    printf("%d--more than 10 answers",id);
    return -1;
}
//[mode3] print answers to a file.
int print_answer_list(FILE* file){
    int index;
    for(int line=0;line<362881;line++){
        index=0;
        fprintf(file,"%d,%d,%d",line,answers[line].square,answers[line].step);
        while (answers[line].paths[index]!=NULL){
            fprintf(file,",");
            fprintf(file,"%s",answers[line].paths[index]);
            index++;
        }
        fprintf(file,"\n");
    }
}
//[mode 3]
int array_to_int(int* array,int lenth){
    int quare=lenth-1;
    int fig=0;
    for(int i=0;i<lenth;i++,quare--){
        fig+=(int)(pow(10,quare)+0.5)*array[i];
    }
    return fig;
}

int do_mode_4(){
    FILE* answers_text=NULL;
    char line[500];
    char ans[13][FLOOR*5];
    int amount_of_ans;
    int index=indexOf(openingSquare);
    if((answers_text=fopen("answers.csv","r"))==NULL) {
        printf("Faild to open the file.\n");
        return -1;
    }
    if(opening_is(rangeSquare)){
    printf("Have completed.....\n"); //首次完成检查.
    }else if(!haveAnswer(openingSquare)){
        printf("Absolutely have no answer!"); //有解检查.
    }else{
        for(int current_line=0;current_line<index+1;current_line++){
            fgets(line,500,answers_text);
        }
        if((amount_of_ans=split(line,ans))==-1) return -1;
        printf("ID=%s, square =%s\n",ans[0],ans[1]);
        printf("path of '0':\n");
        for (int i = 3; i < amount_of_ans; i++){
            char* p=ans[i];
            while (*(++p)!='\0');
            while (p-- != ans[i]) printf("%c->",(*p)+1);
            printf("9");
            printf("\n");
        }
        printf("%s steps in total.\n", ans[2]);
    }
}
//[mode 4] 
int split(char* line, char ans[][FLOOR*5]){
    char *p = line;
    for(int i=0; i<13; i++){
        for(int j=0;j<FLOOR;j++,p++){
            if(*p==','){
                ans[i][j]='\0';
                p++;
                break;
            }else if(*p=='\n'){
                ans[i][j]='\0';
                return i+1;
            }else{
                ans[i][j]=*p;
            }
        }
    }
    return -1;
}

void help(){

}

int main() {
    printf("3-PUZZLE SOLVER v4\n");
    do{
        printf("\n");
        printf("[1]=solve a puzzle\t[2]=statistics all\n[3]=create file\t\t[4]=solve by file\n");
        printf("Press [?] for help; [q] to quit.\n");
        while(getMode()==-1){
            printf("\b\b\b   \b\b\b");
        };
        inatialization();
        //printf("time=%.2fs\n",(double)clock()/CLOCKS_PER_SEC);
    }while(1);
    return 0;
}

//数据初始化
void inatialization(){
    step=0; //全局层数.
    total_squares=0; //全局组合数.
    for (int i = 0; i < 362881; i++){
        visitList[i] = 0;
    }
    for (int i = 0; i < FLOOR; i++){
        stepsList[i] = 0;
    }
    for (int i = 0; i < 362881; i++){
        for(int j=0;j<10;j++){
            answers[i].paths[j]=NULL; 
        }
    }
}

//宽度搜索3
node* bfs_to_file(node* headNode, char mode){
    node*thisNode=headNode->next;
    int* neighbour=NULL;
    node* tempNode=NULL;
    int* tempSquare=NULL;
    do{
        if (thisNode==headNode){
            if(thisNode->next==thisNode) return NULL;
            thisNode=thisNode->next;
            step++;
            continue;
        }
        neighbour=neighbourOf(thisNode->zero);
        do{
            tempSquare=swap(newSquare(thisNode->square),thisNode->zero,*neighbour);
            if(visited(tempSquare)){
                if(mode=='3' && step==answers[indexOf(tempSquare)].step){
                    append_answer(
                        array_to_int(tempSquare,TQS),
                        indexOf(tempSquare),
                        add(newPath(thisNode->path),*neighbour)
                        );
                }
                free(tempSquare);
                continue;
            }
            tempNode=newNode(tempSquare, add(newPath(thisNode->path),*neighbour), *neighbour);
            LinkNodeIback(thisNode,tempNode); //
            if(mode=='1' && opening_is(tempSquare)) return tempNode; //return the answer.
            else if(mode=='2'){
                total_squares++;
                stepsList[step]++;
            }
            else if(mode=='3'){
                append_answer(array_to_int(tempSquare,TQS),indexOf(tempSquare),tempNode->path);
            }
        }while(*(++neighbour) != -1); //
        thisNode=thisNode->next;
        deleteNode(thisNode->last);
    }while (step<FLOOR);
    return NULL;
}

//以下为数据结构
//Create a new list and get the initial data (by arg).
node* newList() {      
    node* head = newNode(NULL,NULL,-1);
    node* firstNode = newNode(rangeSquare, blankPath(), TQS-1);
    *(firstNode->path)=(char)(firstNode->zero+'0');
    *(firstNode->path+1)='\0';///////////////////////////////////////////////////////////////////
    head->last=firstNode;
    head->next=firstNode;
    firstNode->last=head;
    firstNode->next=head;//实现循环
    return head;
} 
//new a node
node* newNode(int* square, char* path, int zero){
    node* thisNode = (node*)malloc(sizeof(node));
    if (thisNode == NULL) {
        printf("malloc failed.\n");
        return NULL;
    } 
    thisNode->last=NULL;
    thisNode->next=NULL;
    thisNode->square=square;
    thisNode->path=path;
    thisNode->zero=zero;
    return thisNode;
}
//link a node in front of parent.
void LinkNodeIfront(node* parentNode, node* thisNode){
    if(parentNode->next!=NULL) parentNode->next->last=thisNode;
    thisNode->last = parentNode;
    thisNode->next = parentNode->next;
    parentNode->next = thisNode; 
}
//link a node in the back of parent.
void LinkNodeIback(node* parentNode, node* thisNode){
    if(parentNode->last!=NULL) parentNode->last->next=thisNode;
    thisNode->last = parentNode->last;
    thisNode->next = parentNode;
    parentNode->last = thisNode; 
}
//delete the node.
void deleteNode(node* thisNode) {
    if(thisNode!=NULL){
        node* nextNode = thisNode->next;
        node* lastNode = thisNode->last;
        lastNode->next = nextNode;
        if(nextNode!=NULL) nextNode->last = lastNode;
        free(thisNode->square);
        //free(thisNode->path); //在最后一个square意外暂停？(已解决，但不知何故这一行效率极低)(mode3需要paths,不改了)
        free(thisNode);
        thisNode=NULL;
    }
}

//以下为node内部结构及操作.
//create a blank square.
int* blankSquare(){
    int* thisSquare = (int*)malloc(sizeof(int)*TQS);
    if (thisSquare == NULL) {
        printf("malloc failed.\n");
        return NULL;
    } 
    return thisSquare;
} 
//create a new Square from parent.（未来会改用指针数组）
int* newSquare(int* parentSquare){
    int* thisSquare = blankSquare();
    memcpy(thisSquare,parentSquare,sizeof(int)*TQS);
    return thisSquare;
}
//return the square after swapping(对换).
int* swap(int* square, int zero, int neighbour){
    int temp=*(square+zero);
    *(square+zero)=*(square+neighbour);
    *(square+neighbour)=temp;
    return square;
}
//create a blank path. (have fred)
char *blankPath(){
    char *thisPath = (char*)malloc(sizeof(int)*(FLOOR+5));
    if (thisPath == NULL) {
        printf("malloc failed.\n");
        return NULL;
    } 
    return thisPath;
} 
//create a new path from parent. (have fred)（未来会改用指针数组）
char *newPath(char *parentPath){
    char *thisPath = blankPath();
    strcpy(thisPath,parentPath);
    return thisPath;
}
//return the path after add.
char *add(char *path, int zero){
    *(path+step)=(char)(zero+'0');
    *(path+step+1)='\0';
    return path;
}

//以下为流程方法.
//no answer 0 else 1, only when zero is in square[TQS].
int haveAnswer(int square[]){
    int inverseNumber=0; //逆序数.
    int square1[TQS];
    int index_0=0; //0的位置.
    for (int i = 0; i < TQS; i++)
    {
        if(square[i]==0) {
            square1[i]=TQS;
            index_0=i;
        }else{
            square1[i]=square[i];
        }
    }
    for(int i=0;i<TQS;i++){
        for(int j=0;j<i;j++){
            if(square1[j]>square1[i]) inverseNumber++;
        }
    }
    if((inverseNumber+index_0)%2==0) return 1;
    else return 0;
}
//only in start.
int zero_of(int* square){
    for(int i=0;i<TQS;i++){
        if (*(square+i) == 0) return i;
    }
    return -1;
}
//if completed return 1 else 0.
int opening_is(int square[]){
    for(int i=0;i<TQS-1;i++){
        if (square[i] != openingSquare[i]) return 0;
    }
    return 1;
}
//make an index for each square.
int indexOf(int square[]){
    int number/*顺序数*/, jiecheng/*阶乘*/;
	int result=0;
	for (int i=0;i<8;i++){
        number=0;
		jiecheng=1;
		if(8-i != 0){
		    for(int j=1;j<=8-i;j++){
			    jiecheng*=j;
			}
		}
		for(int j=0;j<=i;j++){
			if (square[j]<square[i]) number+=1;
		}
		result+=jiecheng*(square[i]-number); //获取当前坐标.
	}
    return result;
}
//if visited return 1 else 0 && mark.
int visited(int square[]){
	int indexOfSquare = indexOf(square); 
	if (visitList[indexOfSquare] == 1) return 1;
	else{
		visitList[indexOfSquare] = 1;
		return 0;
	}
}
//look for the neighbors and return a matrix.(warning:手动释放空间!!)
int* neighbourOf(int zero){
    static int neighbour[5] = {-1};
    for (int i = 0, j = 0; i < TQS; i++){
        if(abs(zero%ORDER-i%ORDER)+abs((zero/ORDER-i/ORDER))==1){
            neighbour[j]=i;
            j++;
        } neighbour[j]=-1;
    }
    return neighbour;
}

//以下为输入输出.

//input a mode.
int getMode(){
    char mode= 0;
    printf(">>");
    mode=getche();
    if(mode=='1'){
        while(scanTheSquare3()==-1){
            printf("\n\nREtry:");
        };
        do_mode_1();
    }
    else if(mode=='2') do_mode_2();  
    else if(mode=='3') do_mode_3();
    else if(mode=='4'){
        while(scanTheSquare3()==-1){
            printf("\n\nREtry:");
        };
        do_mode_4();
    }
    else if(mode=='?') printf("No help now.\n");
    else if(mode=='q') exit(0);
    else return -1;
    return 0;
}
//input a square to begin.
int scanTheSquare3(){
    printf("\ninput the matrix with squares' locations.\n");
    printf("(tips: 0 = blank square; backspace = clear)\n");
    printf("example:\n1 2 3\n4 5 0\n7 8 6\n>>\n");
    char tempChar;
    int tempFig;
    for (int i = 0; i < TQS; i++){
        tempChar=getche();
        while (tempChar < '0' || tempChar >= '0'+TQS){
            if(tempChar=='\b'){
                if(i%ORDER==0) return -1;
                else{
                    i--;
                    printf("\b \b");
                }
            }
            else if(tempChar=='\t') return -1;
            else if(tempChar=='\r'){
                for(int j=0;j<i%ORDER;j++){
                    printf("%d ",openingSquare[i-i%ORDER+j]);
                }
            }
            else printf("\b \b");
            tempChar=getche();
        }
        printf(" ");
        tempFig=tempChar-'0';
        openingSquare[i] = tempFig;
        if(i%ORDER==ORDER-1) printf("\n");
    }
    printf("\n");
    return 0;
}
