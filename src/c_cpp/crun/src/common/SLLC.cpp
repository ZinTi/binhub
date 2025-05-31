/*****************************************************************
 * NodeSLLC : Node SinglyLinkedList of Char, 以字符为节点的单链表的节点
 * SLLC     : SinglyLinkedList of Char, 以字符为节点的单链表
 * 
 *************************************************************/
#include "SLLC.h"
#include <stdio.h>
#include <stdlib.h>

// 初始化链表
SLLC InitSLLC() {
    SLLC head = (SLLC)malloc(sizeof(NodeSLLC));
    head->next = NULL;
    return head;
}

// 头插法新增节点
void InsertHeadSLLC(SLLC head, char data) {
    SLLC newNode = (SLLC)malloc(sizeof(NodeSLLC));
    newNode->data = data;
    newNode->next = head->next;
    head->next = newNode;
}

// 尾插法新增节点
void InsertTailSLLC(SLLC head, char data) {
    SLLC newNode = (SLLC)malloc(sizeof(NodeSLLC));
    newNode->data = data;
    newNode->next = NULL;

    SLLC temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// 查询第一次出现某字符的节点位置
int FindFirstSLLC(SLLC head, char data) {
    int pos = 0;
    SLLC temp = head->next;
    while (temp != NULL) {
        if (temp->data == data) {
            return pos;
        }
        pos++;
        temp = temp->next;
    }
    return -1;
}

// 查询最后一次出现某字符的节点位置
int FindLastSLLC(SLLC head, char data) {
    int pos = 0, lastPos = -1;
    SLLC temp = head->next;
    while (temp != NULL) {
        if (temp->data == data) {
            lastPos = pos;
        }
        pos++;
        temp = temp->next;
    }
    return lastPos;
}

// 查询指定位置的字符内容
char GetDataAtSLLC(SLLC head, int index) {
    int pos = 0;
    SLLC temp = head->next;
    while (temp != NULL) {
        if (pos == index) {
            return temp->data;
        }
        pos++;
        temp = temp->next;
    }
    return '\0'; // 返回空字符表示未找到
}

// 删除指定位置的节点
void DeleteAtSLLC(SLLC head, int index) {
    int pos = 0;
    SLLC temp = head, prev = NULL;

    while (temp->next != NULL && pos < index) {
        prev = temp;
        temp = temp->next;
        pos++;
    }

    if (temp->next == NULL || pos != index) return;

    prev->next = temp->next;
    free(temp);
}

// 修改指定位置的节点内容
void ModifyAtSLLC(SLLC head, int index, char newData) {
    int pos = 0;
    SLLC temp = head->next;

    while (temp != NULL) {
        if (pos == index) {
            temp->data = newData;
            return;
        }
        pos++;
        temp = temp->next;
    }
}

// 计算链表长度
int ListLengthSLLC(SLLC head) {
    int len = 0;
    SLLC temp = head->next;
    while (temp != NULL) {
        len++;
        temp = temp->next;
    }
    return len;
}

// 打印链表内容
void PrintSLLC(SLLC head) {
    SLLC temp = head->next;
    while (temp != NULL) {
        printf("%c -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}

// 顺序排序
void SortSLLC(SLLC head) {
    if (head->next == NULL || head->next->next == NULL) return;

    SLLC i, j;
    char temp;
    for (i = head->next; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->data > j->data) {
                temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
}

// 倒序排序
void ReverseSortSLLC(SLLC head) {
    if (head->next == NULL || head->next->next == NULL) return;

    SLLC i, j;
    char temp;
    for (i = head->next; i != NULL; i = i->next) {
        for (j = i->next; j != NULL; j = j->next) {
            if (i->data < j->data) {
                temp = i->data;
                i->data = j->data;
                j->data = temp;
            }
        }
    }
}

// 在链表尾部追加字符串作为一串节点
void AppendStringSLLC(SLLC head, const char* str) {
    SLLC temp = head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    while (*str) {
        SLLC newNode = (SLLC)malloc(sizeof(NodeSLLC));
        newNode->data = *str;
        newNode->next = NULL;
        temp->next = newNode;
        temp = newNode;
        str++;
    }
}

// 复制链表（深复制）
SLLC CopySLLC(SLLC head) {
    if (head == NULL) return NULL;

    SLLC newHead = (SLLC)malloc(sizeof(NodeSLLC));
    newHead->next = NULL;
    SLLC newTemp = newHead;

    SLLC temp = head->next;
    while (temp != NULL) {
        SLLC newNode = (SLLC)malloc(sizeof(NodeSLLC));
        newNode->data = temp->data;
        newNode->next = NULL;
        newTemp->next = newNode;
        newTemp = newNode;
        temp = temp->next;
    }
    return newHead;
}

// 将链表内容拼接成字符串
char* ToStringSLLC(SLLC head) {
    int len = 0;
    SLLC temp = head->next;
    while (temp != NULL) {
        len++;
        temp = temp->next;
    }

    char* str = (char*)malloc(len + 1); // 为字符串分配内存
    str[len] = '\0'; // 确保字符串结尾为 '\0'

    temp = head->next;
    for (int i = 0; i < len; i++) {
        str[i] = temp->data;
        temp = temp->next;
    }

    return str;
}

// 释放链表内存
void FreeSLLC(SLLC head) {
    SLLC temp = head;
    while (temp != NULL) {
        SLLC next = temp->next;
        free(temp);
        temp = next;
    }
}