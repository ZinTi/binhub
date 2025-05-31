/*****************************************************************
 * NodeSLLC : Node SinglyLinkedList of Char, 以字符为节点的单链表的节点
 * SLLC     : SinglyLinkedList of Char, 以字符为节点的单链表
 * 
 * 
 *************************************************************/
#ifndef SLLC_H
#define SLLC_H

// 定义链表节点结构体
typedef struct NodeSLLC {
    char data;
    struct NodeSLLC* next;
} NodeSLLC, *SLLC;

// 初始化链表
SLLC InitSLLC();
// 头插法新增节点
void InsertHeadSLLC(SLLC head, char data);
// 尾插法新增节点
void InsertTailSLLC(SLLC head, char data);
// 查询第一次出现某字符的节点位置
int FindFirstSLLC(SLLC head, char data);
// 查询最后一次出现某字符的节点位置
int FindLastSLLC(SLLC head, char data);
// 查询指定位置的字符内容
char GetDataAtSLLC(SLLC head, int index);
// 删除指定位置的节点
void DeleteAtSLLC(SLLC head, int index);
// 修改指定位置的节点内容
void ModifyAtSLLC(SLLC head, int index, char newData);
// 计算链表长度
int ListLengthSLLC(SLLC head);
// 打印链表内容
void PrintSLLC(SLLC head);
// 顺序排序
void SortSLLC(SLLC head);
// 倒序排序
void ReverseSortSLLC(SLLC head);
// 在链表尾部追加字符串作为一串节点
void AppendStringSLLC(SLLC head, const char* str);
// 复制链表（深复制）
SLLC CopySLLC(SLLC head);
// 将链表内容拼接成字符串
char* ToStringSLLC(SLLC head);
// 释放链表内存
void FreeSLLC(SLLC head);

#endif // SLLC_H
