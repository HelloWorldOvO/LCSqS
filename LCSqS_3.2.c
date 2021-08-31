#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <stdbool.h>
#define POINTNUM 5000
#define RECTANGLENUM 300000
#define CLASSNUM 20
#define LNUM 10000000
#define LCSQSNUM 1000


typedef struct M{
  int numA,numB;
}M;


typedef struct Rectangle{
  int i, j, k, l;
  char c;
}R, Rectangle;

M stop_m = {.numA=-1, .numB=-1};  //M to stop
Rectangle stop_r = {.i =-1,.j =-1,.k =-1,.l = -1, .c = '\0'}; //Rectangle to stop

M M_array[CLASSNUM][POINTNUM] = {}; //store all point
char class_name[CLASSNUM] = {};
int class_M_length[CLASSNUM] = {};

R *R_array[CLASSNUM][RECTANGLENUM] = {}; //store all rectangle
int class_R_length[CLASSNUM] = {};

R *L_list[LNUM] = {};
int DP[LNUM] = {};
int L_length = 0;

int cmp_char(const void* _a , const void* _b){
  /*
  quick need
  */
    char* a = (char*)_a;
    char* b = (char*)_b;
    return *a - *b;
}

bool cmp_rectangle_small(R *r1, R *r2){
  if(r1->i<r2->i&&r1->j<r2->j&&r1->k<r2->k&&r1->l<r2->l){
    //r1 < r2
    return true;
  }
  return false;
}

bool cmp_rectangle_small_equal(R *r1, R *r2){
  if(r1->i<=r2->i&&r1->j<=r2->j&&r1->k<=r2->k&&r1->l<=r2->l){
    //r1 < r2
    return true;
  }
  return false;
}

char *Intersection(char A[POINTNUM], char B[POINTNUM]){
  /*
  return a intersection of A and B
  */
  char AB[POINTNUM * 2]; //make sure AB is big enough to put A and B
  char A_cpy[POINTNUM];
  char B_cpy[POINTNUM];
  char A_new[POINTNUM]={};
  char B_new[POINTNUM]={};
  strcpy(A_cpy, A);
  strcpy(B_cpy, B);
  qsort(A_cpy, strlen(A_cpy), sizeof(A_cpy)[0], cmp_char); //quick sort A_cpy
  qsort(B_cpy, strlen(B_cpy), sizeof(B_cpy)[0], cmp_char); //quick sort B_cpy
  // printf("A_cpy : %s\n",A_cpy);
  int A_len = 1;
  A_new[0] = A_cpy[0];
  for (int i = 0, j=1; i < strlen(A_cpy)-1;++j){
    if(A_cpy[i]==A_cpy[j]){
      // A_cpy[j] = 0;
      continue;
    }
    A_new[A_len] = A_cpy[j];
    ++A_len;
    i = j;
    j = i;
  }
  // printf("A_new : %s\n", A_new);
  // qsort(A_cpy, strlen(A_cpy), sizeof(A_cpy)[0], cmp_char); //quick sort A_cpy

  int B_len = 1;
  B_new[0] = B_cpy[0];
  for (int i = 0, j=1; i < strlen(B_cpy)-1;++j){
    if(B_cpy[i]==B_cpy[j]){
      // B_cpy[j] = 0;
      continue;
    }
    B_new[B_len] = B_cpy[j];
    ++B_len;
    i = j;
    j = i;
  }
  // qsort(B_cpy, strlen(B_cpy), sizeof(B_cpy)[0], cmp_char); //quick sort B_cpy

  strcpy(AB, strcat(A_new, B_new));             //put A + B into AB
  qsort(AB, strlen(AB), sizeof(AB)[0], cmp_char); //quick sort AB
  // printf("in function : %s", AB);
  char *same = (char*)calloc(strlen(AB),sizeof(char)); //allocate zero space

  for(int i = 0,j = 0;i<strlen(AB);++i){  //i iterate AB
    if(i!=strlen(AB)&&AB[i]==AB[i+1]){  //if i == i+1 in AB and i+1 not over the array
      if(j!=0&&(*(same+j-sizeof(char)) == AB[i])){ //if AB[i] have be record in same array or same array is empty
        continue;
      }
      *(same + j) = AB[i]; //put AB[i] into same array
      j += sizeof(char);
    }
  }
    // printf("after delete unsame : %s\n", p);
    return same;
}

void CreatePoint(char A[POINTNUM], char B[POINTNUM], char *same){
  /*
  store all points into M_array with sorted when find
  */
  for (int iter = 0; iter < strlen(same);++iter){ //iterate all intersection
    int a = -1, b = -1;
    int len_A = strlen(A), len_B = strlen(B);
    class_name[iter] = *(same+iter*sizeof(char)); //store the character find now
    class_M_length[iter] = 0; //count
    // printf("%d",class_M_length[iter]);
    // printf("%llu", strlen(A));
    while(a<len_A || b<len_B){  //when a(iterate A) and b(iterate B) out of range both
      for (int i = a+1; i < len_A;++i){ //i iterate A
      // printf("in A");
        if(class_name[iter] == A[i]){ //find A[i] same with the character we are finding
          for (int j = b + 1; j < len_B;++j){ //find all B[j] same with A[i]
            if(A[i] == B[j]){
              M m = {.numA = i, .numB = j};
              M_array[iter][class_M_length[iter]] = m;
              ++class_M_length[iter]; //point in class execute now plus 1
              // printf("(%d,%d)", i, j);
            }
          }
          a = i;
        }
        if(i==len_A-1){
          a = len_A;
        }
      }
      for (int j = b + 1; b < len_B;++j){
      // printf("in B");
        if (class_name[iter] == B[j]){
          for (int i = a + 1; i < len_A;++i){
            if(A[i] == B[j]){
              M m = {.numA = i, .numB = j};
              M_array[iter][class_M_length[iter]] = m;
              ++class_M_length[iter];
            }
          }
          b = j;
        }
        if(j == len_B-1){
          b = len_B;
        }
      }
    }
  }
}

void Show_M_array(void){
  for(int class_num = 0;class_num < strlen(class_name);++class_num){
    printf("Class %c : ",class_name[class_num]);
    for (int i = 0;i<class_M_length[class_num];++i){
      // scanf("%d", &i);
      printf("(%d,%d) ", M_array[class_num][i].numA, M_array[class_num][i].numB);
    }
    printf("\n");
  }
}

void CreateRectangle(void){
  // printf("in function");
  for(int class_num = 0;class_num < strlen(class_name) ; ++class_num) { //for all class
    char character = class_name[class_num];
    class_R_length[class_num] = 0;
    for (int first = 0;first<class_M_length[class_num];++first){
      for (int second = first + 1;second<class_M_length[class_num];++second){
        // printf("test");

        if(M_array[class_num][first].numA<M_array[class_num][second].numA&&M_array[class_num][first].numB<M_array[class_num][second].numB){
          /* find two point conform the format of rectangle */
          struct Rectangle *ptr = (struct Rectangle *) malloc(sizeof(struct Rectangle));
          R r = { .i = M_array[class_num][first].numA,
                  .j = M_array[class_num][second].numA,
                  .k = M_array[class_num][first].numB,
                  .l = M_array[class_num][second].numB,
                  .c = character};
          *ptr = r;
          R_array[class_num][class_R_length[class_num]] = ptr;
          ++class_R_length[class_num];
          // printf("test");
        }
      }
    }
  }
}

void Show_R_array(void){
  for(int class_num = 0;class_num < strlen(class_name);++class_num){
    printf("Class %c : ",class_name[class_num]);
    for (int i = 0;i<class_R_length[class_num];++i){
      printf("(%d,%d,%d,%d) ",R_array[class_num][i]->i,R_array[class_num][i]->j,R_array[class_num][i]->k,R_array[class_num][i]->l);
    }
    printf("\n");
  }
}

void SortRectangle(void){
  int now_length = 0;
  for (int class_num = 0; class_num < strlen(class_name);++class_num){  //for all classes
    for (int Rnow = 0; Rnow < class_R_length[class_num];++Rnow){  //for all rectangles
      L_list[now_length] = R_array[class_num][Rnow];  //push Rnow in L_list
      ++now_length;
      for (int t_length = now_length-1;t_length-1>=0;--t_length){
        if (cmp_rectangle_small(L_list[t_length-1],L_list[t_length])){ //front samller than back
          break;
        }
        R *rt = L_list[t_length - 1];
        L_list[t_length - 1] = L_list[t_length];
        L_list[t_length] = rt;
      }
    }
  }
  L_length = now_length;
}

void Show_L(void){
  for (int i = 0;i<L_length;++i){
    printf("(%d,%d,%d,%d) ", L_list[i]->i, L_list[i]->j, L_list[i]->k, L_list[i]->l);
  }
}

int HELLO_WORLD(void){
  int true_rb;
  memset(DP, 0, L_length);
  int max = 0;

  for (int rb = 0; rb < L_length;++rb){
    memset(DP, 0, LNUM);
    // for (int i = 0;i<L_length;++i){
    //   printf("%d", DP[i]);
    // }
    DP[rb] = 2;
    for (int re = rb + 1; re < L_length;++re){
      for (int rp = rb; rp < re;++rp){
        if(cmp_rectangle_small(L_list[rp], L_list[re])&&L_list[re]->i<L_list[rb]->j&&L_list[re]->k<L_list[rb]->l&&DP[rp]!=0){
          if(DP[rp]+2>DP[re]){
            DP[re] = DP[rp] + 2;
            if(DP[re]>max){
              max = DP[re];
              true_rb = rb;
            }
          }

        }
      }
    }
  }
  if(L_length!=0&&max==0){
    max = 2;
  }
  printf("LCSqS : %d\n", max);
  return true_rb;
}

void Show_LCSqS(int rb){
  int max = 0;
  memset(DP, 0, LNUM);
  char LCSqS[LCSQSNUM] = {};
  // for (int i = 0;i<L_length;++i){
  //   printf("%d", DP[i]);
  // }
  DP[rb] = 2;
  for (int re = rb + 1; re < L_length; ++re)
  {
    for (int rp = rb; rp < re; ++rp)
    {
      if (cmp_rectangle_small(L_list[rp], L_list[re]) && L_list[re]->i < L_list[rb]->j && L_list[re]->k < L_list[rb]->l&&DP[rp]!=0)
      {
        if (DP[rp] + 2 > DP[re])
        {
          DP[re] = DP[rp] + 2;
          if (DP[re] > max)
          {
            max = DP[re];
          }
        }
      }
    }
  }
  // printf("max : %d, rb :%c\n", max, class_name[rb]);
  printf("Sequence : ");

  int re=-1;

  int index = max/2 -1;
  int maxcpy = index;
  // int ret;/
  for (int rp = L_length-1; rp >= rb-1;--rp){
    if(DP[rp]==max){

      if(re==-1){
        re = rp;
        // ret = re;
        // printf("%c",L_list[rp]->c);
        LCSqS[index] = L_list[rp]->c;
        max -= 2;
        --index;
      }
      else{
        if(cmp_rectangle_small(L_list[rb],L_list[rp])&&cmp_rectangle_small(L_list[rp],L_list[re])&&L_list[rp]->i<L_list[rb]->j&&L_list[rp]->k<L_list[rb]->l&& L_list[re]->i<L_list[rp]->j&&L_list[re]->k<L_list[rp]->l){
          // ret = rp;
          re = rp;
          // printf("%c",L_list[rp]->c);
          LCSqS[index] = L_list[rp]->c;
          max -= 2;
          --index;
        }
      }


    }
  }
  // printf("%c",L_list[rb]->c);
  LCSqS[index] = L_list[rb]->c;
  for (int square = 2; square != 0;--square) {
    for (int i = 0; i <= maxcpy; ++i){
      printf("%c", LCSqS[i]);
    }
  }

}

int main(int argc, char **argv){
  char stringA[POINTNUM];
  char stringB[POINTNUM];
  int class_number;
  printf("input string A : ");
  scanf("%s",stringA);
  printf("input string B : ");
  scanf("%s",stringB);
  // printf("A:%s\nB:%s\n",stringA,stringB);

  char *intersection = Intersection(stringA, stringB);
  // printf("in main : %s\nlength : %llu", intersection, strlen(intersection));
  class_number = strlen(intersection);
  // printf("%d", class_number);

  CreatePoint(stringA, stringB, intersection); //Classifiction
  // Show_M_array();
  // printf("class_name : %llu",strlen(class_name));
  // printf("%d", class_number);
  CreateRectangle();
  // Show_R_array();

  SortRectangle();
  // Show_L();
  /*int r_base = */HELLO_WORLD();
  // Show_LCSqS(r_base);
  return 0;
}












