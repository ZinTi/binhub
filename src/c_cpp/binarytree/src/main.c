#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef _WIN32
	#include <windows.h>
#endif

#include "binhub.h"



void ShowBinaryTree(int total, int depth);
int Involution(int x, int y);
int Menu(void);
void ShowPropertiesOfBinTree(void);

int main(int argc, char* argv[]){
	BINHUB_UNUSED(argc);
	BINHUB_UNUSED(argv);

	int o;
	while(1){
		o = Menu();
		if (o==1){
			int total = 0;
			printf("Total > ");
			if(scanf("%d", &total)){}
			while(  getchar()!='\n' );
			ShowBinaryTree(total, 0);
		}else if(o==2){
			int dep;
			printf("Depth > ");
			if(scanf("%d", &dep)){}
			if(dep>30){
				printf("Invalid: dep>30.\n");
				continue;
			}
			while(  getchar()!='\n' );
			ShowBinaryTree(0, dep);
		}else if(o==3){
			ShowPropertiesOfBinTree();
		}
	}


	return 0;
}

void ShowPropertiesOfBinTree(void){
#ifdef _WIN32
	binhub_setenc(ENCODING_UTF8);
#endif
	printf("1. 二叉树的第i层上至多有2^(i-1)个结点(i>=1)\n");
	printf("2. 深度为k的二叉树至多有(2^k)-1个结点(k>=1)\n");
	printf("3. 对任意一棵二叉树T，若终端结点数为n0，而其度数为2的结点数为n2，则n0 = n2 + 1\n");
	printf("4. 具有n个结点的完全二叉树的深度为 Floor(log2 n)+1 \n");
	printf("5. 对于具有n个结点的完全二叉树，如果按照从上到下、从左到右的顺序对二叉树中的所有节点从1开始顺序编号，则对于任意序号为i的结点有：\n");
	printf("\t① 如 i=1，则序号为i的结点是根结点，无双亲结点；如i>1，则序号为i的结点的双亲结点序号为 Floor(i/2)。\n");
	printf("\t② 如 2i>n，则序号为i的结点无左孩子；如2i<=n，则序号为i的结点的左孩子结点的序号为2i。\n");
	printf("\t③ 如 2i+1>n，则序号为i的结点无右孩子；如2i+1<=n，则序号为i的结点的右孩子结点的序号为 2i+1。\n\n");
	printf("           %02d\n", 1);
	printf("        /        \\\n");
	printf("      %02d         %02d\n", 2, 3);
	printf("    /   \\      /   \\\n");
	printf("  %02d    %02d    %02d    %02d\n", 4, 5, 6, 7);
	printf("%4s%6s%6s%6s\n", "/\\", "/\\", "/\\", "/\\");
	printf("%02d %02d %d %d %d %d %d %d\n\n", 8,9,10,11,12,13,14,15);
}

void ShowBinaryTree(int total, int depth){
	if(total==0 && depth!=0){
		printf("%s%10s%10s%10s\n--------+----------+----------+----------\n","Depth","Min","Max","Num");
		for(int i=0;i<depth;i++){
			printf("%3d%12d%15d%10d\n", i+1, Involution(2, i), Involution(2, i+1)-1, Involution(2, i));
		}
		printf("-----------------------------------------\ntotal=%d\n\n", Involution(2,depth)-1);
	}else if(total!=0 && depth==0){
		printf("%s%12s%12s%12s\n--------+----------+----------+----------\n","Depth","Min","Max","Num");
		for(int i=0;;i++){
			if(Involution(2, i+1)-1 <= total){
				printf("%3d%12d%15d%10d\n", i+1, Involution(2, i), Involution(2, i+1)-1, Involution(2, i));
			}else {
				printf("%3d%12d%15d%10d\n", i+1, Involution(2, i), total, total+1-Involution(2, i));
				break;
			}
		}
		int n0=0, n2=0, n1;
		if(total%2==0){
			n0 = total/2;
			n1 = 1;
		}else {
			n0 = total/2+1;
			n1 = 0;
		}
		n2 = n0-1;
		printf("-----------------------------------------\nn0=%d, n2=%d, n1=%d\n\n", n0, n2, n1);
	}else {
		printf("It cannot be 0.\n");
	}

}

int Menu(void){
	while(1){
		printf("a.Total b.Depth c.Properties e.Exit Input > ");
		char option;
		if(scanf("%c", &option)){}
		while( getchar()!='\n' );
		if(option=='a'){
			return 1;
		}else if(option=='b'){
			return 2;
		}else if(option=='c'){
			return 3;
		}else if(option=='e'){
			printf("Bye!\n");
			exit(0);
		}else {
			printf("Invalid option.\n");
		}
	}
}

int Involution(int x, int y){
	int power = 1;
	for(int i=0;i<y;i++){
		power *= x ;
	}
	return power;
}
