//
//  main.cpp
//  Discrete
//
//  Created by 이민재 on 09/11/2019.
//  Copyright © 2019 yi. All rights reserved.
//


//input example : (or a1 (or (not (or a2 a3)) a4)))

#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define BUFFERSIZE 1

int AnswerArray[128][128] = {0};
int maxval = 0;

char input[1024] = "";

int arrayX = 0 , arrayY = 0;
int charnum = 0;
char temp;
char tempStr[10];
int tempStrCount = 0;  // tempStr += temp를 구현하기 위해서 도입
int countArray = 0;
int countTest = 0;
int nextOper = 0;

char str[128][128];

typedef enum _boolean {
    FALSE,
    TRUE
} bool;

typedef struct node {
    int data;
    struct node *left_child;
    struct node *right_child;
    bool operatorCheck;
} tree_node;

void initial(char * temp)
{
  for(int i = 0 ; i< 10 ; i++)
  {
    temp[i] = '\0';
  }
    tempStrCount = 0;
}

int checkValid(int array[],int size)
{
  for(int i = 0; i<size; i++)
  {
    for(int j = 0; j<i; j++)
    {
      if(array[i]==0) break;
      if(array[i]+array[j]==0) return 0;
    }
          if(array[i]==0) break;
  }
  return 1;
}

tree_node * makeTree()
{
    tree_node * ptr = (tree_node *)malloc(sizeof(tree_node));
    tree_node * child;
  //  child->operatorCheck =TRUE;
    tree_node * child2;
 //   child2->operatorCheck =TRUE;
    ptr->left_child = ptr->right_child = NULL;
    int operandNum = 0;
    while(1){
      //nextOper
      if(!strcmp(str[nextOper],"or")){
        ptr->data = -1;
      }
      else if(!strcmp(str[nextOper],"and"))
      {
        ptr->data = -2;
      }
      else if(!strcmp(str[nextOper],"not"))
      {
        ptr->data = 0;
      }

      else if(str[nextOper][0]=='a'){
        //operand가 들어 왔을 때
        operandNum++;
        str[nextOper][0] = '0';

        int data = atoi(str[nextOper]);        // 문자열을 정수로 변환하여 num1에 할당
        if(operandNum==1)
        {
            ptr->left_child = (tree_node *)malloc(sizeof(tree_node));
            ptr->left_child->data = data;
        }
        else if(operandNum==2)
        {
          if(ptr->data!=0){
            ptr->right_child = (tree_node *)malloc(sizeof(tree_node));
            ptr->right_child->data = data;
          }
        }
        else
        {
          child = (tree_node *)malloc(sizeof(tree_node));
          child->data = ptr->data;
          child->right_child = (tree_node *)malloc(sizeof(tree_node));
          child->left_child = ptr->right_child;
          child->right_child->data = data;
          ptr->right_child = child;
        }
      }

      else if(!strcmp(str[nextOper],"("))
      {
        operandNum++;
        nextOper++;

        child = makeTree();
        if(operandNum==1)
        {
          ptr->left_child = child;
        }
        else if(operandNum==2)
        {
          ptr->right_child = child;
        }
        else{
            child2 = (tree_node *)malloc(sizeof(tree_node));
            child2->data = ptr->data;
            child2->left_child = ptr-> right_child;
            child2->right_child = child;
            ptr->right_child = child2;
            }
      }
      else if(!strcmp(str[nextOper],")"))
      {
        break;
      }
      nextOper++;

    }
    return ptr;
}


tree_node * cvt2NNF(tree_node *ptrNNF,int change){
  if(ptrNNF->left_child==NULL && ptrNNF->right_child==NULL){
   if(change%2 == 1){
      ptrNNF->data = (ptrNNF->data)*-1;
   }
      return ptrNNF;
  }
    if(change%2==1)
    {
        if(ptrNNF->data == -1 && ptrNNF->operatorCheck==TRUE){
         // ptrNNF->operatorCheck = TRUE;
          ptrNNF->data  = -2;
        }
        else if(ptrNNF->data  ==-2 && ptrNNF->operatorCheck==TRUE){
         // ptrNNF->operatorCheck = TRUE;
          ptrNNF->data =-1;
        }
        else{
          ptrNNF->data = (ptrNNF->data)*-1;
        }
    }
    tree_node * child;
    tree_node * child2;

   if(!(ptrNNF->left_child== NULL))child = ptrNNF->left_child;
   if(!(ptrNNF->right_child== NULL))child2 = ptrNNF->right_child;
    if(ptrNNF->data == 0){
       change++;
       if(change==0){
         if(child->data == -1 && child->operatorCheck==TRUE){
          // child->operatorCheck = TRUE;
          child->data  = -2;
        }
        else if(child->data  ==-2 && child->operatorCheck==TRUE){
   //       child->operatorCheck = TRUE;
          child->data =-1;

        }
        else{
          child->data = (child->data)*-1;
        }
       }
      return cvt2NNF(child,change);
      }



     //printf("%d",ptrNNF->right_child->data);

   if(ptrNNF->left_child!=NULL)ptrNNF->left_child =cvt2NNF(ptrNNF->left_child,change);

  if(ptrNNF->right_child!=NULL) ptrNNF->right_child = cvt2NNF(ptrNNF->right_child,change);

   return ptrNNF;

}

void OperatorCheck(tree_node *root){
  if (root == NULL){
        return;
  }
  if(root->data<0){
    root->operatorCheck=1;
  }
  OperatorCheck(root->left_child);
  OperatorCheck(root->right_child);
}

int checkIfItisOnlyBuiltByAndOperation(tree_node * ptr)
{
  //return 1 if there is a and operation
  //and return 0 if there is no and operation and only consist with or operation
  if(ptr->operatorCheck==1&&ptr->data==-1) return 1;

  int temp;
  if(ptr->left_child!=NULL)
  {
    temp = checkIfItisOnlyBuiltByAndOperation(ptr->left_child);
    if(temp>0) return 1;
  }
  if(ptr->right_child!=NULL)
  {
    temp = checkIfItisOnlyBuiltByAndOperation(ptr->right_child);
    if(temp>0) return 1;
  }
  return 0;
}

tree_node * makeTreeWithTwoNodeandGiveAndOperator(tree_node * ptr1, tree_node * ptr2)
{
  tree_node * ptr = (tree_node *)malloc(sizeof(tree_node));
  ptr->operatorCheck = 1;
  ptr->data = -2;
  ptr->left_child = ptr1;
  ptr->right_child = ptr2;
  return ptr;
}

void findNodesFromAndandReturnArray(tree_node * ptr,tree_node * array[]){

    //and 일때
    if(ptr->operatorCheck==1&&ptr->data==-1)
    {
      findNodesFromAndandReturnArray(ptr->left_child,array);
      findNodesFromAndandReturnArray(ptr->right_child,array);
    }
    //or 이나 연산자일때
    else {
        array[countArray] = ptr;
        countArray++;
    }

}

tree_node * NNF2CNF(tree_node *ptr){
  tree_node * temp_left;
  tree_node * temp_right;
  int left_bold_capacity, right_bold_capacity;
  int check = 0;

  //this function needs to be when ptr is or
  check = checkIfItisOnlyBuiltByAndOperation(ptr);
  if(check==0){
    return ptr;
  }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~or
  temp_left = NNF2CNF(ptr->left_child);

  temp_right = NNF2CNF(ptr->right_child);

  ptr->right_child = temp_right;

  ptr->left_child = temp_left;

  if(ptr->operatorCheck==1&&ptr->data==-2)
  {
    //if it is or
    //NNF2CNF must be executed when we meet 'or'
    //and also makes the format buttom is or
    //and a right format
    tree_node * left_bold[100] = {NULL};
    tree_node * right_bold[100] = {NULL};

    countArray = 0;
    findNodesFromAndandReturnArray(ptr->left_child,left_bold);
    left_bold_capacity = countArray;
    countArray = 0;
    findNodesFromAndandReturnArray(ptr->right_child,right_bold);
    right_bold_capacity = countArray;
  //ptr이 or이 었을때
    ptr->data = -1;

    int temp0 = 0;

    for(int i = 0; i<left_bold_capacity; i++)
      for(int j = 0; j<right_bold_capacity; j++)
      {
        if(temp0==0)
        {
          ptr->left_child = makeTreeWithTwoNodeandGiveAndOperator(left_bold[i],right_bold[j]);
        }
        else if(temp0==1)
        {
          ptr->right_child = makeTreeWithTwoNodeandGiveAndOperator(left_bold[i],right_bold[j]);
        }
        else{
          tree_node * child = (tree_node *)malloc(sizeof(tree_node));
          child->data = -1;
          child->operatorCheck = 1;

          child->right_child = (tree_node *)malloc(sizeof(tree_node));
          child->left_child = ptr->right_child;

          child->right_child = makeTreeWithTwoNodeandGiveAndOperator(left_bold[i],right_bold[j]);

          ptr->right_child = child;
        }
        temp0++;
      }
    }
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  return ptr;
}

void print(tree_node *root){
  //tree_node *child = root->left_child;
  //tree_node *child2 = root->right_child;
  if (root == NULL){
    return;
  }
  print(root->left_child);
  if(root->operatorCheck==0)
  {
    AnswerArray[arrayY][arrayX] = root->data;
    arrayX++;
  }
  else if(root->operatorCheck==1 && root->data == -1)
  {
    arrayY++;
    arrayX = 0;
  }
    print(root->right_child);


}

int main(int argc, char **argv) {

    int count = 0;
    char temp;
    char temp_str[128] = {0};
    int temp_int = 0 ;
    
    char buffer[BUFFERSIZE];
    FILE* instream;
    int bytes_read =0;
    int buffer_size =0;
    buffer_size = sizeof(unsigned char)*BUFFERSIZE;
    instream = fopen("/dev/stdin","r");
    //FILE *fp = fopen("", "w");
    if (instream==NULL)
    {
        printf("errer occured\n");
        exit(1);
    }
    int i=0;
    while((bytes_read=fread(&buffer, buffer_size, 1, instream))==buffer_size){
      input[i] = buffer[0];
      i++;
    }
    

    //printf("%s\n",buffer);
    tree_node * result;
    initial(tempStr);
    int check = 0;
    while(1)
    {
        temp = input[temp_int];
        temp_int++;
        check = 0;
        if(temp=='\0') break;

        else if((temp==')'||temp==' ')&&temp_str[0]!='\0')
        {
            strcpy(str[count],temp_str);
            count++;
            if(temp_str[0] == ')') check = 1;
            initial(temp_str);
        }

        else if(temp=='(')
        {
            strcpy(str[count],"(");
            count++;
        }

        else if(temp!=' ')
        {
            temp_str[tempStrCount] = temp;
            tempStrCount++;
        }

        if(temp==')')
        {
           if(check==1)
           {
             continue;
           }
            strcpy(str[count],")");
            count++;
            initial(temp_str);

        }
    }

    if(!strcmp(str[nextOper],"("))
    {
      nextOper++;
      result = makeTree();
    }
    else
    {
      printf("wrong input");
    }

  OperatorCheck(result);
  tree_node * result2;
  result2 =cvt2NNF(result,0);
  tree_node * result3;

  result3 = NNF2CNF(result2);
  print(result3);
  int val;
  int temp_val;
  for(int i=0 ; i<arrayY+1 ; i++)
  {
    for(int j=0 ; j<128; j++)
    {
      val = AnswerArray[i][j];
      if(val==0) break;
      temp_val = val;
      if(temp_val<0) temp_val = -temp_val;
      if(maxval<temp_val) maxval= temp_val;
       printf("%d ",val);
    }
    printf("\n");
  }
    printf("0\n");
  //printf("%d",maxval);
  int find = 0;
  int findResult = 0;
  int valid = 0;
  for(int i=0 ; i<arrayY+1 ; i++)
  {
      valid = checkValid(AnswerArray[i],128);
      if(valid==1) {
      find = 0;
      printf("\nsolution\n");
      for(int j = 1; j<=maxval; j++)
      {
        findResult = j;
        for(int k = 0; k<maxval;k++)
        {
          if(AnswerArray[i][k]==(-j))
          {
            findResult = -j;
            break;
          }
        }
        printf("%d ",findResult);
      }
      break;
      }
  }
    printf("\n");
    if(valid==0)
        printf("UNSAT\n");
}
