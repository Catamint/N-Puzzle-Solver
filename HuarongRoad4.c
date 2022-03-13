/*3-PUZZLE SOLVER v4.0
*
*三阶数字华容道求解器，可以
*1 输入一个开局，然后得到一个最优解
*2 统计所有开局的最优解分布情况
* @Catamint
*/

#include<stdio.h>
#include<stdlib.h> 
#include<time.h>
#include<string.h>
#define ORDER 3  //(阶数)
#define TQS 9  //total quantity of squares. (ORDER^2)
#define FLOOR 33  //maximum of steps.

typedef struct ListNode {
    int* square; // [FLOOR] (宫格)
    int zero; //0的位置.
    int* path; // [TQS]
    struct ListNode* next;
    struct ListNode* last;
}node;

int step=0; //全局层数.
int total_squares=0; //全局组合数.
int visitList[362881] = { 0 }; //if visited turn 0 to 1.
int stepsList[FLOOR]={0}; //存放步数为[]的数量.
int completeSquare[TQS]; //输入的square,即终点.
//
node* bfs(node* headNode, char mode);
void inatialization();
void do_mode_1();
void do_mode_2();
//
node* newList();
node* newNode(int* square, int* path, int zero);
void LinkNodeIfront(node* parentNode, node* thisNode);
void LinkNodeIback(node* parentNode, node* thisNode);
void deleteNode(node* thisNode);
//
int* blankSquare();
int* newSquare(int* parentSquare);
int* firstSquare();
int* swap(int* square, int zero, int neighbour);
int* blankPath();
int* newPath(int* parentPath);
int* add(int* path, int zero);
//
int haveAnswer(int square[]);
int zero_of(int* square);
int completed(int* square);
int indexOf(int square[]);
int visited(int square[]);
int* neighbourOf(int zero);
//
int scanTheSquare3();
int getMode();
node* printNode(node* node);
void printList(node* head);
//以下为测试
void testneigh(){
    for(int j=0;j<TQS;j++){
        int* nei=neighbourOf(j);
        printf("\n%d: ", j);
        for(int i=0;i<4;i++){
            printf("%d ",(*nei));
            nei++;
        }
    }   
}
//
int main() {
    printf("3-PUZZLE SOLVER v4\n");
    do{
        printf("\n");
        printf("[1]=answer a puzzle  [2]=statistics all  [?]=help\n");
        while(getMode()==-1);
        inatialization();
        //printf("time=%.2fs\n",(double)clock()/CLOCKS_PER_SEC);
    }while(1);
    return 0;
}
//
void do_mode_1(){
    node* answer = NULL;
    if(completed(firstSquare())){
    printf("Have completed.....\n"); //首次完成检查.
    }else if(!haveAnswer(completeSquare)){
        printf("Absolutely have no answer!"); //有解检查.
    }else{
        answer = bfs(newList(),'1');
        if(answer!=NULL){
            printf("Completed!\n");
            printNode(answer);
        }else{
            printf("Error.\n");
        }
        printf("\n");
        //printNode(BFSList->next);
        //testneigh(3);
    }
}
void do_mode_2(){
    bfs(newList(),'2');
    printf(" _______________ \n");
    printf("|steps\t|amount\t|\n");
    for(int i=0;i<FLOOR;i++){
        printf("|%d\t|%d\t|\n",i+1,stepsList[i]);
    }
    printf(" --------------- \n");
    printf("(9! / 2 = 181440;)\n");
    printf("Have visited %d squares in total.\n\n",total_squares);
}
//
void inatialization(){
    step=0; //全局层数.
    total_squares=0; //全局组合数.
    for (int i = 0; i < 362881; i++){
        visitList[i] = 0;
    }
    for (int i = 0; i < FLOOR; i++){
        stepsList[i] = 0;
    }
}
//bfs
node* bfs(node* headNode, char mode){
    node*thisNode=headNode->next;
    int* neighbour=NULL;
    node* tempNode=NULL;
    int* tempSquare=NULL;
    int* tempPath=NULL;
    do{
        do{
            //printf("\n\nnextfloor\n");////
            neighbour=neighbourOf(thisNode->zero);
            do{ 
                tempSquare=swap(newSquare(thisNode->square),thisNode->zero,*neighbour);
                if(visited(tempSquare)){
                    free(tempSquare);
                    tempSquare=NULL;
                    continue;
                }
                tempPath=newPath(add(thisNode->path,*neighbour));
                tempNode=newNode(tempSquare,tempPath,*neighbour);
                stepsList[step]++;
                total_squares++;
                if(mode=='1'){
                    if(completed(tempSquare)){
                        return tempNode;
                    } //return the answer if chose mode 1.
                } 
                LinkNodeIback(thisNode,tempNode); //
                //printList(headNode);////
            }while(*(++neighbour) != -1);//一个node.
            //printNode(thisNode);////
            if(thisNode->next!=NULL){
                thisNode=thisNode->next;
                deleteNode(thisNode->last);
            }else 
                if(headNode->next!=thisNode){
                free(headNode->square);
                free(headNode->path);
                headNode->square=newSquare(thisNode->last->square);
                headNode->path=newPath(thisNode->last->path);
                headNode->zero=thisNode->last->zero;
                tempNode=thisNode;
                thisNode=headNode->next;
                deleteNode(tempNode);
                break;
            }else{
                return NULL;
            }
        }while (1);
    }while(step++ <= FLOOR); //超层.
    return NULL;
}


//以下为数据结构
//Create a new list and get the initial data (by arg).
node* newList() {      
    node* head = newNode(NULL,NULL,-1); //flag.
    node* firstNode = newNode(firstSquare(), blankPath(), TQS-1);
    *(firstNode->path)=firstNode->zero;
    LinkNodeIfront(head,firstNode);
    return head;
} 
//new a node
node* newNode(int* square, int* path, int zero){
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
        node* nextNode = thisNode->next; //Q:可否以A->B->C访问结构体成员的成员？A:可.
        node* lastNode = thisNode->last;
        lastNode->next = nextNode;
        if(nextNode!=NULL) nextNode->last = lastNode;
        free(thisNode->square);
        thisNode->square=NULL;
        //free(thisNode->path); //在最后一个square意外暂停？(已解决，但不知何故这一行效率极低)
        //thisNode->path=NULL;
        free(thisNode);
        thisNode=NULL;
    }
}

//以下为node内部结构及操作.
//create a blank square. (have freed)
int* blankSquare(){
    int* thisSquare = (int*)malloc(sizeof(int)*TQS);
    if (thisSquare == NULL) {
        printf("malloc failed.\n");
        return NULL;
    } 
    return thisSquare;
} 
//create a new Square from parent. (have freed)
int* newSquare(int* parentSquare){
    int* thisSquare = blankSquare();
    for (int i = 0; i < TQS; i++){
        *(thisSquare+i)=*(parentSquare+i);
    }
    return thisSquare;
}
//create a zhengzhengshu-0 list.
int* firstSquare(){
    int* thisSquare = blankSquare();
    for(int i=1;i<TQS;i++){
        thisSquare[i-1]=i;
    }
    thisSquare[TQS-1]=0;
    return thisSquare;
}
//return the square after swapping(对换).
int* swap(int* square, int zero, int neighbour){
    int temp=*(square+zero);
    *(square+zero)=*(square+neighbour);
    *(square+neighbour)=temp;
    return square;
}
//create a blank path. (have freed)
int* blankPath(){
    int* thisPath = (int*)malloc(sizeof(int)*(FLOOR+5));
    if (thisPath == NULL) {
        printf("malloc failed.\n");
        return NULL;
    } 
    return thisPath;
} 
//create a new path from parent. (have freed)
int* newPath(int* parentPath){
    int* thisPath = blankPath();
    for (int i = 0; i < FLOOR; i++)
    {
        *(thisPath+i)=*(parentPath+i);
    }
    return thisPath;
}
//return the path after add.
int* add(int* path, int zero){
    *(path+step)=zero;
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
int completed(int square[]){
    for(int i=0;i<TQS-1;i++){
        if (square[i] != completeSquare[i]) return 0;
    }
    return 1;
}
//make an index for each square.
int indexOf(int square[]){
    int number/*顺序数*/, jiecheng/*阶乘*/;
	int result = 0;
	for (int i = 0; i < 8; i++){
        number = 0;
		jiecheng = 1;
		if(8 - i != 0){
		    for(int j = 1; j <= 8 - i; j++){
			    jiecheng *= j;
			}
		}
		for(int j = 0; j <= i; j++){
			if (square[j] < square[i]) number += 1;
		}
		result += jiecheng * (square[i] - number); //获取当前坐标.
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
int scanTheSquare3(){
    printf("\ninput the matrix with squares' locations.\n");
    printf("(tips: blank square = 0; backspace = clear)\n");
    printf("example:\n1 2 3\n4 5 0\n7 8 6\n>>\n");
    char tempChar;
    int tempFig;
    for (int i = 0; i < TQS; i++){
        tempChar=getche();
        while (tempChar < '0' || tempChar >= '0'+TQS){
            if(tempChar=='\b'){
                if(i%ORDER==0) goto err;
                else{
                    i--;
                    printf("\b \b");
                }
            }
            else if(tempChar=='\t') goto err;
            else if(tempChar=='\r'){
                for(int j=0;j<i%ORDER;j++){
                    printf("%d ",completeSquare[i-i%ORDER+j]);
                }
            }
            else printf("\b \b");
            tempChar=getche();
        }
        printf(" ");
        tempFig=tempChar-'0';
        completeSquare[i] = tempFig;
        if(i%ORDER==ORDER-1) printf("\n");
    }
    printf("\n");
    return 0;

err:printf("\n\nRE:");
    return -1;
}
//
int getMode(){
    char mode=0;
    printf(">>");
    mode=getche();
    printf("\n");
    if(mode=='1'){
        while(scanTheSquare3()==-1);
        do_mode_1();
    }
    else if(mode=='2')do_mode_2();  
    else if(mode=='?') printf("No help now.\n");
    else return -1;
    return 0;
}
//
node* printNode(node* node) {
    printf("square =");
    for(int i=0;i<TQS;i++){
        printf(" %d",node->square[i]);
    }
    printf(";\npath of '0' = begin");
    for(int i=step;i>=0;i--){
        printf("->%d",node->path[i]+1);
    }
    printf(";\n");
    printf("%d steps in total.\n", step+1);
    printf("\n");
    return node;
}
//
void printList(node* head){
    node* temp = head;
    printf("-------list=\n");
    while(temp->next!=NULL){
        temp=printNode(temp->next);
    }
    printf("-------end\n");
}
