#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

typedef struct student {
    int ID;
    char Full_name[50];
    char Account[50];
    float GPA;
} Student;

typedef struct node {
    Student data;
    struct node* next;
} Node;


void printStudent(Student student);
void printList(Node* head);
Node* insertNode(Node* head, Student student, int position);
Node* deleteNode(Node* head, int position);
Node* findNode(Node* head, char* name);
Node* sortList(Node* head, int option);
void Infor_Stusent();
void Menu();
void Enter_sort();

#endif /*__LINKED_LIST_H*/