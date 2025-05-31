/*****************************************************************
 * NodeSLLS : Node SinglyLinkedList of Strings, 以字符串为节点的单链表的节点
 * SLLS     : SinglyLinkedList of Strings, 以字符串为节点的单链表
 * 
 * 
 *************************************************************/
#ifndef SLLS_H
#define SLLS_H

// 定义链表节点结构体
typedef struct NodeSLLS {
    char *data;
    struct NodeSLLS* next;
} NodeSLLS, *SLLS;

// 初始化单链表
SLLS InitSLLS(void);
// 创建新节点
SLLS CreateNodeSLLS(const char* data);
// 头插法新增节点
void InsertNodeSLLSAtHead(SLLS head, const char* data);
// 尾插法新增节点
void InsertNodeSLLSAtTail(SLLS head, const char* data);
// 查询节点位置（根据内容，返回位置从1开始）
int FindNodeSLLSByContent(SLLS head, const char* data);
// 查询节点内容（根据位置）
char* FindNodeSLLSByPosition(SLLS head, int position);
// 删除指定位置节点
void DeleteNodeSLLSByPosition(SLLS head, int position);
// 删除匹配内容的节点
void DeleteNodeSLLSByContent(SLLS head, const char* data);
// 修改节点内容（根据匹配原内容）
void ModifyNodeSLLSByContent(SLLS head, const char* oldData, const char* newData);
// 修改节点内容（根据指定位置）
void ModifyNodeSLLSByPosition(SLLS head, int position, const char* newData);
// 冒泡排序实现顺序排序
void SortSLLSAscending(SLLS head);
// 冒泡排序实现倒序排序
void SortSLLSDescending(SLLS head);
// 计算链表的元素个数（不包含头节点）
int GetSLLSLength(SLLS head);
// 打印链表
void PrintSLLS(SLLS head);
// 释放链表内存
void FreeSLLS(SLLS head);

#endif // SLLS_H
