/*******************************************************************************
국민대학교 화일처리 BST Code
2018. 11. 4
작성자 : 김선필(20143038)

코드 요약 : Node class와 Tree class를 이용한 BST 알고리즘 구현
********************************************************************************/
#include <iostream>
#include <cstdio>
#include <fstream>
#include <cstdlib>

using namespace std;

class  Node
{
private:
  int Key;        // Node 의 KEY 값
  Node *left;     // Node의 Left 포인터
  Node *right;    // Node의 Right 포인터
public:
  Node();         // Node Class 생성자
  Node(int);      // Node Class 생성자

  void setKey(int key)   // Node의 KEY 값을 할당
  {
    this->Key = key;
  }

  void setLeft()  // Node의 Left 포인터 값을 NULL로 할당
  {
    left = NULL;
  }

  void setLeft(Node* newNode) // Node의 Left 포인터 값을 parameter 로 할당
  {
    left = newNode;
  }

  void setRight() // Node의 Right 포인터 값을 NULL로 할당
  {
    right = NULL;
  }

  void setRight(Node* newNode)  // Node의 Left 포인터 값을 parameter 로 할당
  {
    right = newNode;
  }

  int getKey()  // Node의 KEY값 반환
  {
    return Key;
  }

  Node* getLeft() // Node의 Left 포인터 반환
  {
    return left;
  }

  Node* getRight()  // Node의 Right 포인터 반환
  {
    return right;
  }

};

// Node Class 생성자(기본 생성자)
Node::Node()
{
  this->Key = 0;
  this->left = NULL;
  this->right = NULL;
}

// Node Class 생성자(특정 Key를 할당하는 생성자)
Node::Node(int key)
{
  this->Key = key;
  this->left = NULL;
  this->right = NULL;
}

// Binary Search Tree Class
class BST
{
private:
  Node *root;     // Root 포인터
public:
  BST();  // BST 생성자
  void setRoot(); // root 할당 함수
  Node* getRoot() // root 반환 함수
  {
    return this->root;
  }
  // void setNode()
  // {
  //   this->numberOfNode++;
  // }
  // void unsetNode()
  // {
  //   this->numberOfNode--;
  // }
  // int getNumberOfNode()
  // {
  //   return numberOfNode;
  // }
  bool insertKey(int);
  Node* searchKey(int);
  Node* searchKey(int,Node**);
  void inorder(Node*);
  Node* maxNode(Node*);
  Node* minNode(Node*);
  int getNoNode(Node* temp);
  int getHeight(Node*);
  bool deleteKey(int);
  void drawBSTree(Node *p, int level);
};
/*
함수 이름 : BST()
함수 기능 : BST Class 생성자
*/
BST::BST()  // BST 생성자
{
  root = NULL;   // 초기 root 값을 NULL로 할당
}

/*
함수 이름 : BST::insertKey(int)
함수 기능 : BST 삽입 알고리즘
반 환 형 : true(삽입 성공) / false(삽입 실패)
*/
bool BST::insertKey(int key)  // BST 삽입 함수 - 삽입 성공 : true 리턴  삽입 실패 : false 리턴
{
  Node *newNode = new Node(key);  // 새로운 Node 동적 할당
  Node *p,*q;   // q = 삽입할 공간의 parent pointer
  if(!root)     // root node 가 비어 있을때(트리에 노드가 없을 때)
  {
    root = newNode; // root에 삽입

    return true;
  }
  else
  {
    p = root;

    while(p!=NULL)    // 삽입할 공간 탐색
    {
      if(key < p->getKey())
      {
        q=p;
        p=p->getLeft();
      }
      else if(key > p->getKey())
      {
        q=p;
        p=p->getRight();
      }
      else  // key값 중복시 예외 처리
      {
        cout << "P의 Key : " << p->getKey() << endl;
        cout << "Find same Key Error" << endl;
        delete newNode;
        return false;
      }
    }

    if(key < q->getKey())   // 삽입 될 KEY 값이 현재 q포인터의 KEY값보다 작을때
    {
      q->setLeft(newNode);  // q포인터의 Left에 할당
    }
    else                    // 삽입 될 KEY 값이 현재 q포인터의 KEY값보다 클때
    {
      q->setRight(newNode);  // q포인터의 Right에 할당
    }

    return true;
  }
}


/*
함수 이름 : BST::searchKey(int)
함수 기능 : BST 알고리즘
반 환 형 : 찾은 Key의 address 를 반환 / 못찾으면 NULL 값 반환
*/
Node* BST::searchKey(int key) // parameter로 보내진 key값을 탐색하고 탐색한 Node의 포인터를 반환
{
  if(!root)
  {
    return NULL;
  }

  Node *p;
  p = getRoot();    // p 포인터에 root 값 할당

  while(p!=NULL)    // BST 알고리즘으로 KEY값 탐색
  {
    if(key<p->getKey())
    {
      p = p->getLeft();
    }
    else if(p->getKey() < key)
    {
      p = p->getRight();
    }
    else if(p->getKey() == key)
    {
    //  cout << "Success to find the Key : " << key << endl;
      return  p;
    }
  }
  cout << "Couldn't find the Key" << endl;
  return NULL;
}


/*
함수 이름 : BST::searchKey(int,Node**)
함수 기능 : BST 알고리즘
반 환 형 : 찾은 Key의 address 를 반환 & 찾은 Key의 parent Node의 address 반환 / 못찾으면 NULL 값 반환
ppt 상의 searchParentNode를 함수 오버라이딩으로 searchKey함수 안에 같이 구현..
*/
Node* BST::searchKey(int key, Node** parent)   // parameter로 보내진 key값을 탐색하고 탐색한 Node 포인터와 parent 포인터 반환
{
  if(!root)
  {
    return NULL;
  }

  Node *p;
  p = getRoot();
  *parent = getRoot();

  while(p!=NULL)
  {
    if(key<p->getKey())
    {
      *parent = p;
      //cout << *parent << endl;
      p = p->getLeft();
    }
    else if(p->getKey() < key)
    {
      *parent = p;
      //cout << *parent<< endl;
      p = p->getRight();
    }
    else if(p->getKey() == key)
    {
      //cout << "Success to find the Key : " << key << endl;
      return  p;
    }
  }
  cout << "Couldn't find the Key" << endl;
  return NULL;
}

/*
함수 이름 : BST::inorder(Node *)
함수 기능 : BST 중위 순회 알고리즘
반 환 형 : X
*/
void BST::inorder(Node *temp)   // 중위 순회 알고리즘
{
  if(temp != NULL)
  {
    inorder(temp->getLeft());
    cout << temp->getKey() << ' ';
    inorder(temp->getRight());
  }
}

/*
함수 이름 : BST::maxNode(Node *)
함수 기능 : Max값 탐색
반 환 형 : Max값을 가지고 있는 노드의 address 반환
*/
Node* BST::maxNode(Node* max)  // 원하는 부분에서 max값을 찾아 그 포인터를 반환하는 함수
{
  if(max == NULL)
  {
    cout << "No Nodes!" << endl;
    return NULL;
  } // 예외 처리
  while(max->getRight() != NULL)
  {
    max = max->getRight();
  }
  return max;
}

/*
함수 이름 : BST::minNode(Node *)
함수 기능 : Min값 탐색
반 환 형 : Min값을 가지고 있는 노드의 address 반환
*/
Node* BST::minNode(Node* min) // 원하는 부분에서 min값을 찾아 그 포인터를 반환하는 함수
{
  if(min == NULL)
  {
    cout << "No Nodes!" << endl;
    return NULL;
  } // 예외 처리

  while(min->getLeft() != NULL)
  {
    min = min->getLeft();
  }
  return min;
}

/*
함수 이름 : BST::getHeight(Node *)
함수 기능 : Tree의 Height 구하기
반 환 형 : Height 반환
*/
int BST::getHeight(Node* temp)  // Tree의 Height를 구하고 그 값을 반환
{
  if (!temp)
    return 0;
  else {
    int heightOfLeft = getHeight(temp->getLeft());
    int hieghtOfRight = getHeight(temp->getRight());
    return 1 + (heightOfLeft > hieghtOfRight ? heightOfLeft : hieghtOfRight);
  }
}

/*
함수 이름 : BST::getNoNode(Node *)
함수 기능 : Tree의 노드 개수 구하기
반 환 형 : 노드 개수 반환
*/
int BST::getNoNode(Node* temp)  // Tree의 Node의 갯수를 구하고 반환
{
  if (!temp)
    return 0;
  else
    return getNoNode(temp->getLeft()) + getNoNode(temp->getRight()) + 1;
}

/*
함수 이름 : BST::deleteKey(Node *)
함수 기능 : BST 삭제 알고리즘
반 환 형 : true(성공) / false(실패)
*/
bool BST::deleteKey(int key)  // parameter 로 보내진 KEY값을 찾아 삭제하는 함수
{
  Node *parent, *search;    // search : delete할 부분의 포인터  | parent : delete 포인터의 parent 포인터

  search = searchKey(key,&parent);  // searchKey함수를 이용해 dp와 dp의 parent를 받아옴

  if(search == NULL)  // 지우고하는 KEY값이 없을 때의 예외 처리
  {
    cout << "Can't find the data to delete" << endl;
  }

  // Case 1 : 자식 노드가 없을 때(leaf node일 경우)
  if(search->getRight() == NULL && search->getLeft() == NULL)
  {
      if(root == search) // root node가 leaf node일때
      {
        root = NULL;     // root node가 가리키는 값을 NULL로 바꿈(잘못된 메모리에 접근을 방지하기 위한 조치)
        delete search;   // dp 삭제
      }
      // dp 을 지우고 dp의 parent node가 잘못된 메모리 영역에 접근하는 것을 방지하기 위해 left,right값 처리
      else{
        if(parent->getKey() > key)
        {
          delete search;
          parent->setLeft();  // dp 의 parent node의 left 값을 NULL로 변경
        }
        else
        {
          delete search;      // dp 의 parent node의 Right 값을 NULL로 변경
          parent->setRight();
        }
    }
  } // 자식 노드가 없을때 삭제 끝

  // Case 2 : 자식 노드가 한개만 있을 때
  else if(search->getRight() == NULL || search->getLeft() == NULL)
  {
    if(root == search)  // 지우고자 하는 node가 root 노드일 경우
    {
      if(search->getLeft() != NULL) // dp의 left에 자식노드가 있을 경우
      {
        root = search->getLeft();
        search->setLeft();
        delete search;
      }
      else if(search->getRight() != NULL) // dp의 right에 자식노드가 있을 경우
      {
        root = search->getRight();
        search->setRight();
        delete search;
      }
    }
    else  // 지우고자 하는 node가 root 노드가 아닐 경우
    {
      if(search->getLeft() != NULL) // dp의 left에 자식노드가 있을 경우
      {
        if(parent->getLeft() == search)
        {
          parent->setLeft(search->getLeft());
          search->setLeft();
          delete search;
        }
        else if(parent->getRight() == search)
        {
          parent->setRight(search->getLeft());
          search->setLeft();
          delete search;
        }
      }
      else if(search->getRight() != NULL) // dp의 right에 자식노드가 있을 경우
      {
        if(parent->getLeft() == search)
        {
          parent->setLeft(search->getRight());
          search->setRight();
          delete search;
        }
        else if(parent->getRight() == search)
        {
          parent->setRight(search->getRight());
          search->setRight();
          delete search;
        }
      }
    }
  } // 자식 노드가 1개일 경우 삭제 끝

  // 자식 노드가 2개일 경우
  else if(search->getRight() != NULL && search->getLeft() != NULL)
  {
      // dp의 왼쪽 서브트리와 오른쪽 서브트리 중 height를 비교하여 더 큰 height를 가진 서브트리에서 대체값 구하기

      //left 서브트리의 height가 right 서브트리의 height보다 작을 때(right 서브트리의 min값으로 대체)
      if(getHeight(search->getLeft()) < getHeight(search->getRight()))
      {
        Node *parentOfTemp; // right 서브트리의 min값의 부모노드
        Node *temp = searchKey(minNode(search->getRight())->getKey(),&parentOfTemp);  // temp : right 서브트리의 min값을 가진 노드의 포인터

        if(temp->getRight()!=NULL)  // right 서브트리 min값의 right node가 존재할 경우의 예외처리
        {
          parentOfTemp->setRight(temp->getRight());
        }
        else if(search == parentOfTemp)
        {
          parentOfTemp->setRight(temp->getRight());
        }
        else
        {
          parentOfTemp->setLeft();
        }

        search->setKey(temp->getKey()); // 오른쪽 서브트리의 min 값으로 key값 복사

        temp-> setLeft();   // delete 해주기 전에 사전 처리(잘못된 메모리 영역을 가리키는 것을 방지)
        temp-> setRight();  // delete 해주기 전에 사전 처리(잘못된 메모리 영역을 가리키는 것을 방지)

        delete temp; // min값을 가진 node 삭제
      }

      //left 서브트리의 height가 right 서브트리의 height보다 클 때(left 서브트리의 max값으로 대체)
      else if(getHeight(search->getLeft()) > getHeight(search->getRight()))
      {

        Node *parentOfTemp;  // left 서브트리의 max값의 부모노드
        Node *temp = searchKey(maxNode(search->getLeft())->getKey(),&parentOfTemp); // temp : left 서브트리의 max값을 가진 노드의 포인터

        if(temp->getLeft()!=NULL)  // 왼쪽 서브트리 max값의 left node가 존재할 경우의 예외처리
        {
          parentOfTemp->setLeft(temp->getLeft());
          //cout << parentOfTemp->getLeft()->getKey() << endl;
        }
        else if(search == parentOfTemp)
        {
          search->setLeft(temp->getLeft());
        }
        else
        {
          parentOfTemp->setRight();
        }

        search->setKey(temp->getKey()); // left 서브트리의 max 값으로 key값 복사

        temp-> setLeft();   // delete 해주기 전에 사전 처리(잘못된 메모리 영역을 가리키는 것을 방지)
        temp-> setRight();  // delete 해주기 전에 사전 처리(잘못된 메모리 영역을 가리키는 것을 방지)

        delete temp;        // max값을 가진 node 삭제

      }

      //left 서브트리의 height랑 right 서브트리의 height 같을때(left와 right 서브트리의 node의 개수로 판단)
      else if(getHeight(search->getLeft()) == getHeight(search->getRight()))
      {
        // left 서브트리의 노드의 개수가 right 서브트리의 노드의 개수보다 작을 때(right 서브트리의 min값으로 대체)
        if(getNoNode(search->getLeft()) < getNoNode(search->getRight()))
        {
          Node *parentOfTemp; // right 서브트리의 min값의 부모노드
          Node *temp = searchKey(minNode(search->getRight())->getKey(),&parentOfTemp);  // temp : right 서브트리의 min값을 가진 노드의 포인터

          if(temp->getRight()!=NULL)  // right 서브트리 min값의 right node가 존재할 경우의 예외처리
          {
            parentOfTemp->setLeft(temp->getRight());
          }
          else
          {
            parentOfTemp->setLeft();
          }

          search->setKey(temp->getKey()); // right 서브트리의 min 값으로 key값 복사

          temp-> setLeft();   // delete 해주기 전에 사전 처리(잘못된 메모리 영역을 가리키는 것을 방지)
          temp-> setRight();  // delete 해주기 전에 사전 처리(잘못된 메모리 영역을 가리키는 것을 방지)

          delete temp;        // min값을 가진 node 삭제

          //cout << "Root : " << root->getKey() << endl;
        }
        else
        {
            Node *parentOfTemp;   // left 서브트리의 max값의 부모노드
            Node *temp = searchKey(maxNode(search->getLeft())->getKey(),&parentOfTemp);   // temp : left 서브트리의 max값을 가진 노드의 포인터
            //cout << parentOfTemp->getKey() << endl;
            if(temp->getLeft()!=NULL) // 왼쪽 서브트리 max값의 left node가 존재할 경우의 예외처리
            {
              parentOfTemp->setRight(temp->getLeft());
              //cout << parentOfTemp->getLeft()->getKey() << endl;
            }
            else if(search == parentOfTemp)
            {
              search->setLeft(temp->getLeft());
            }
            else
            {
              parentOfTemp->setRight();
            }

            search->setKey(temp->getKey()); // left 서브트리의 max 값으로 key값 복사

            temp-> setLeft();   // delete 해주기 전에 사전 처리(잘못된 메모리 영역을 가리키는 것을 방지)
            temp-> setRight();  // delete 해주기 전에 사전 처리(잘못된 메모리 영역을 가리키는 것을 방지)

            delete temp;  // max값을 가진 node 삭제
        }

      }

  }
  return true;
}

/*
함수 이름 : BST::drawBSTree(Node *,int)
함수 기능 : BST 그리는 알고리즘
반 환 형 : X
// 각 단계마다 노드의 관계가 잘 표현되는지 확인하기 위한 코드(개인적으로 추가 구현함)
*/

void BST::drawBSTree(Node *p, int level)
{
  if (p != 0 && level <= 100)
  {
    drawBSTree(p->getRight(), level+1);
    for (int i = 1; i <= (level-1); i++)
    cout << "   ";
    cout << " " << p->getKey();
    if (p->getLeft() != 0 && p->getRight() != 0) cout << "<" << endl;
    else if (p->getRight() != 0) cout << " /" << endl;
    else if (p->getLeft() != 0) cout << "  \\" << endl;
    else cout << endl;
    drawBSTree(p->getLeft(), level+1);
  }
}

int main()
{
  BST *bst = new BST();

  int arrayOfData[16] ={8,18,11,5,15,4,9,1,7,17,6,14,10,3,19,20};  // Data array


  // ppt에 나와있는 main함수의 수행을 for문을 이용해 코드 길이를 줄임.
  cout << "=========================== 최초 삽입 ===========================" << endl;
  for(int i = 0; i<16;i++)
  {
    cout << "< " <<arrayOfData[i]<< " 삽입 > " << endl;
      bst->insertKey(arrayOfData[i]);
      bst->inorder(bst->getRoot());
      cout << endl << endl;
  }
  bst->drawBSTree(bst->getRoot(),bst->getHeight(bst->getRoot()));
  
  cout << endl <<"=========================== 정순 삭제 ===========================" << endl;
  for(int i = 0; i<16;i++)
  {
    cout << "< " <<arrayOfData[i]<< " 삭제 > " << endl;
    bst->deleteKey(arrayOfData[i]);
    bst->inorder(bst->getRoot());
    cout << endl << endl;

  }
  bst->drawBSTree(bst->getRoot(),bst->getHeight(bst->getRoot()));

  cout << endl <<"=========================== 재 삽입 ===========================" << endl;
  for(int i = 0; i<16;i++)
  {
    cout << "< " <<arrayOfData[i]<< " 삽입 > " << endl;
      bst->insertKey(arrayOfData[i]);
      bst->inorder(bst->getRoot());
      cout << endl << endl;
  }
  bst->drawBSTree(bst->getRoot(),bst->getHeight(bst->getRoot()));

  cout << endl <<"=========================== 역순 삭제 ===========================" << endl;
  for(int i = 15; i>=0;i--)
  {
    cout << "< " <<arrayOfData[i]<< " 삭제 > " << endl;
    bst->deleteKey(arrayOfData[i]);
    bst->inorder(bst->getRoot());
    cout << endl << endl;
  }
  bst->drawBSTree(bst->getRoot(),bst->getHeight(bst->getRoot()));

  return 0;
}
