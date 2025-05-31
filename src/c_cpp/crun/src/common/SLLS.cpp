/*****************************************************************
 * NodeSLLS : Node SinglyLinkedList of Strings, 以字符串为节点的单链表的节点
 * SLLS     : SinglyLinkedList of Strings, 以字符串为节点的单链表
 * 
 *************************************************************/
#include "SLLS.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>

// 初始化单链表
SLLS InitSLLS(void) {
    SLLS head = (SLLS)malloc(sizeof(NodeSLLS));
    head->data = NULL;  // 头节点无数据
    head->next = NULL;
    return head;
}

// 创建新节点
SLLS CreateNodeSLLS(const char* data) {
    SLLS newNodeSLLS = (SLLS)malloc(sizeof(NodeSLLS));
    if (newNodeSLLS == NULL) {
        printf("Memory allocate failed!\n");
        exit(EXIT_FAILURE); // 或者返回 NULL，根据你的设计
    }
    newNodeSLLS->data = (char*)malloc(strlen(data) + 1);
    if (newNodeSLLS->data == NULL) {
        printf("Memory allocate failed!\n");
        free(newNodeSLLS);
        exit(EXIT_FAILURE); // 或者返回 NULL
    }
    strcpy(newNodeSLLS->data, data);
    newNodeSLLS->next = NULL;
    return newNodeSLLS;
}

// 头插法新增节点
void InsertNodeSLLSAtHead(SLLS head, const char* data) {
    SLLS newNodeSLLS = CreateNodeSLLS(data);
    newNodeSLLS->next = head->next;
    head->next = newNodeSLLS;
}

// 尾插法新增节点
void InsertNodeSLLSAtTail(SLLS head, const char* data) {
    SLLS newNodeSLLS = CreateNodeSLLS(data);
    SLLS p = head;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = newNodeSLLS;
}

// 查询节点位置（根据内容，返回位置从1开始）
int FindNodeSLLSByContent(SLLS head, const char* data) {
    SLLS p = head->next;
    int position = 1;
    while (p != NULL) {
        if (strcmp(p->data, data) == 0) {
            return position;
        }
        p = p->next;
        position++;
    }
    return -1; // 未找到返回-1
}

// 查询节点内容（根据位置）
char* FindNodeSLLSByPosition(SLLS head, int position) {
    SLLS p = head->next;
    int count = 1;
    while (p != NULL && count < position) {
        p = p->next;
        count++;
    }
    if (p == NULL) {
        printf("Position is invalid!\n");
        return NULL; // 如果位置无效，返回NULL
    }
    return p->data; // 返回指定位置的节点内容
}

// 删除指定位置节点
void DeleteNodeSLLSByPosition(SLLS head, int position) {
    if (position <= 0) {
        printf("Invalid position!\n");
        return;
    }

    SLLS p = head;
    int count = 0;
    while (p->next != NULL && count < position - 1) {
        p = p->next;
        count++;
    }
    if (p->next == NULL) {
        printf("Position is invalid!\n");
        return;
    }
    SLLS temp = p->next;
    p->next = temp->next;
    free(temp->data);
    free(temp);
}

// 删除匹配内容的节点
void DeleteNodeSLLSByContent(SLLS head, const char* data) {
    SLLS p = head;
    while (p->next != NULL && strcmp(p->next->data, data) != 0) {
        p = p->next;
    }
    if (p->next == NULL) {
        printf("No matching node was found!\n");
        return;
    }
    SLLS temp = p->next;
    p->next = temp->next;
    free(temp->data);
    free(temp);
}

// 修改节点内容（根据匹配原内容）
void ModifyNodeSLLSByContent(SLLS head, const char* oldData, const char* newData) {
    SLLS p = head->next;
    while (p != NULL) {
        if (strcmp(p->data, oldData) == 0) {
            free(p->data);
            p->data = (char*)malloc(strlen(newData) + 1);
            if (p->data == NULL) {
                printf("Memory allocate failed!\n");
                exit(EXIT_FAILURE); // 或者根据情况返回
            }
            strcpy(p->data, newData);
            return;
        }
        p = p->next;
    }
    printf("No matching node was found!\n");
}

// 修改节点内容（根据指定位置）
void ModifyNodeSLLSByPosition(SLLS head, int position, const char* newData) {
    SLLS p = head->next;
    int count = 1;
    while (p != NULL && count < position) {
        p = p->next;
        count++;
    }
    if (p == NULL) {
        printf("Position is invalid!\n");
        return;
    }
    free(p->data);
    p->data = (char*)malloc(strlen(newData) + 1);
    strcpy(p->data, newData);
}

// 冒泡排序实现顺序排序
void SortSLLSAscending(SLLS head) {
    if (head->next == NULL) return; // 空链表或单节点链表无需排序

    SLLS p, q;
    char *tempData;
    int swapped;
    do {
        swapped = 0;
        for (q = head->next; q->next != NULL; q = q->next) {
            if (strcmp(q->data, q->next->data) > 0) {
                tempData = q->data;
                q->data = q->next->data;
                q->next->data = tempData;
                swapped = 1;
            }
        }
    } while (swapped);
}

// 冒泡排序实现倒序排序
void SortSLLSDescending(SLLS head) {
    if (head->next == NULL) return; // 空链表或单节点链表无需排序

    SLLS p, q;
    char *tempData;
    for (p = head->next; p != NULL; p = p->next) {
        for (q = head->next; q->next != NULL; q = q->next) {
            if (strcmp(q->data, q->next->data) < 0) {
                tempData = q->data;
                q->data = q->next->data;
                q->next->data = tempData;
            }
        }
    }
}

// 计算链表的元素个数（不包含头节点）
int GetSLLSLength(SLLS head) {
    int length = 0;
    SLLS p = head->next; // 跳过头节点
    while (p != NULL) {
        length++;
        p = p->next;
    }
    return length;
}

// 打印链表
void PrintSLLS(SLLS head) {
    SLLS p = head->next;
    while (p != NULL) {
        printf("%s -> ", p->data);
        p = p->next;
    }
    printf("NULL\n");
}

// 释放链表内存
void FreeSLLS(SLLS head) {
    SLLS p = head;
    while (p != NULL) {
        SLLS temp = p;
        p = p->next;
        if (temp->data) free(temp->data);
        free(temp);
    }
}
