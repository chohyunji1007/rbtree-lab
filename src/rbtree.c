#include "rbtree.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
// #define SENTINEL 1

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  p->nil = (node_t *)calloc(1, sizeof(node_t));
  p->nil->color =1; //black
  p->root = p->nil;

  return p;
}
void del_node(rbtree *t, node_t *p){
  if(p != t->nil){
    del_node(t, p->left);
    del_node(t, p->right);
    free(p);
  }
}
void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  del_node(t, t->root);
  free(t->nil);
  free(t);
}


void left_rotation(rbtree *t, node_t *p){
  
  //p 노드 오른쪽 자식 cp
  node_t *cp = p->right;
  // cp의 왼쪽 자식을 p 오른쪽 자식으로
  p->right = cp->left;
  // cp의 왼쪽 자식이 nil노드가 아니라면
  if(cp->left != t->nil)
    //p를 cp의 왼쪽 자식의 부모를 p로 변경
    cp->left->parent = p;
  // cp의 부모 노드를 p의 부모노드로 변경
  cp->parent = p->parent;
  //p의 부모노드가 nil노드라면 p가 root였단 소리니
  if(p->parent == t->nil)
    //t의 루트노드를 cp로 변경
    t->root = cp;
  //p가 p 부모노드의 왼쪽 자식이라면
  else if(p==p->parent->left)
    //p의 부모의 왼쪽 자식을 cp로 변경
    p->parent->left = cp;
  else //p가 p 부모노드의 오른쪽 자식이라면 오른쪽 자식을 cp로 변경
    p->parent->right = cp;

  //cp의 왼쪽 자식을 p로 변경
  cp->left = p;
  //p의 부모노드를 cp로 변경
  p->parent = cp;

}
void right_rotation(rbtree *t, node_t *p){
  //p의 왼쪽 자식 노드 cp
  node_t *cp = p->left;
  //cp의 오른쪽 자식 노드를 p의 왼쪽 자식 노드로
  p->left = cp->right;
  //cp의 오른쪽 자식 노드가 nil노드가 아니라면 cp오른쪽 노드의 부모 노드에 p삽입
  if(cp->right !=t->nil)
    cp->right->parent = p;

  //cp부모 노드를 p의 부모 노드로 변경
  cp->parent = p->parent;
  //p의 부모노드가 nil노드라면 p가 root였다는 거니 t의 root노드에 cp 삽입
  if(p->parent == t->nil)
    t->root = cp;
  //p가 p의 부모 노드에 왼쪽 자식이였다면 왼쪽에 cp 삽입
  else if(p==p->parent->left)
    p->parent->left = cp;
  //p가 p의 부모 노드에 오른쪽 자식이였다면 오른쪽에 cp 삽입
  else
    p->parent->right = cp;
  
  //cp의 오른쪽 노드에 p 삽입
  cp->right = p;
  //p의 부모 노드에 cp 삽입
  p->parent = cp;
}

void insertion_fixup(rbtree *t, node_t *p){
  while(p->parent->color==0){ //부모노드의 색이 red인 경우
    //p의 부모노드가 왼쪽 자식인 경우
    if(p->parent==p->parent->parent->left){
      //p의 삼촌 노드를 cp에 삽입
      node_t *cp = p->parent->parent->right;
      if(cp->color==0){ //삼촌 노드cp가 붉은 노드일 경우
        p->parent->color=1; //부모 노드를 black으로 변환
        cp->color =1; //삼촌 노드 black으로 변환
        p->parent->parent->color =0; //할아버지를 red로 변환
        //p를 할아버지로 변경
        p = p->parent->parent;
      }
      else{ 
        //p가 부모노드의 오른쪽 자식인 경우
        if(p == p->parent->right){
          p = p->parent;
          left_rotation(t, p);
        }
        //p의 부모노드를 black으로 변환
        p->parent->color = 1;
        //p의 할아버지를 red로 변환
        p->parent->parent->color =0;
        right_rotation(t, p->parent->parent);
      }
    }
    //p의 부모노드가 오른쪽 자식 노드인 경우
    else{ 
      //cp에 p의 삼촌노드 삽입
      node_t *cp = p->parent->parent->left;
      //p의 삼촌노드가 red일때
      if(cp->color==0){ 
        //p의 부모노드를 black으로 변환
        p->parent->color=1;
        //cp(p의 삼촌노드)를 black으로 변환
        cp->color=1;
        //p의 할아버지를 red로 변환
        p->parent->parent->color=0;
        //p를 p의 할아버지 노드로 변경
        p=p->parent->parent;
      }else{
        //p가 부모노드의 왼쪽 자식인 경우
        if(p==p->parent->left){
          p=p->parent;
          right_rotation(t, p);
        }
        //p의 부모노드를 black으로 변환
        p->parent->color=1;
        //p의 할아버지를 red로 변환
        p->parent->parent->color=0;
        left_rotation(t, p->parent->parent);
      }
    }
  }
  //root를 black으로 변환
  t->root->color=1;
}
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *t_nil = t->nil;
  node_t *t_root = t->root;

  node_t *insert_node = (node_t *)calloc(1, sizeof(node_t));
  insert_node->key=key;
  insert_node->color=0; //입력 노드는 red
  
  while(t_root!=t->nil){
    t_nil = t_root;
    if(insert_node->key < t_root->key)
      t_root = t_root->left;
    else
      t_root = t_root->right;
  }

  insert_node->parent = t_nil;

  if(t_nil == t->nil)
    t->root = insert_node;
  else if(insert_node->key < t_nil->key)
    t_nil->left = insert_node;
  else
    t_nil->right = insert_node;

  insert_node->left = t->nil;
  insert_node->right = t->nil;
  
  insertion_fixup(t, insert_node);
  return t->root;
}
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *cmp_node = t->root;

  while(cmp_node!=NULL){
    
    if(cmp_node->key < key )
      cmp_node=cmp_node->right;
    else if(cmp_node->key > key)
      cmp_node=cmp_node->left;
    
    else //같은 경우 빠져나가기
      break;
  }
    
  return cmp_node;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cmp_node = t->root;
  if(cmp_node == NULL)
    return cmp_node;
  while(cmp_node->left!=t->nil){
      cmp_node=cmp_node->left;
  }
  return cmp_node;
  
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cmp_node = t->root;
  if(cmp_node == NULL)
    return cmp_node;
  while(cmp_node->right!=t->nil){
      cmp_node=cmp_node->right;
  }
  return cmp_node;

}
void rbtree_delete_fixup(rbtree *t, node_t *p){
  while(p!=t->root && p->color==1){
    //case1-4
    if(p==p->parent->left){
      node_t *w = p->parent->right;
      //case1 : p의 형제가 red인 경우
      if(w->color==0){
        w->color = 1; //형제를 black으로
        p->parent->color = 0; //부모를 red로
        left_rotation(t, p->parent);
        w = p->parent->right;
      }
      //case2 : p의 형제가 black이고, 형제의 자식이 모두 black인 경우
      if(w->left->color ==1 && w->right->color==1){
        w->color = 0; //형제를 red로
        p=p->parent;
      }

      //case 3 : p의 형제가 black, 형제의 왼쪽자식은 red, 형제의 오른쪽 자식은 black인 경우
      else{
        if(w->right->color ==1){
          w->left->color=1;
          w->color=0;
          right_rotation(t, w);
          w = p->parent->right;
        }
        //case 4 : p의 형제가 black, 형제의 오른쪽 자식은 red
        w->color = p->parent->color;
        p->parent->color = 1;
        w->right->color = 1;
        left_rotation(t, p->parent);
        p=t->root;
      }
    }
    //case 5-8: right case
    else{
      node_t *w = p->parent->left;

      //case 5 : p의 형제가 red
      if(w->color == 0){
        w->color=1;
        p->parent->color=0;
        right_rotation(t, p->parent);
        w = p->parent->left;
      }
      //case 6: p의 형제는 black, 형제의 두 모두 black인 경우
      if(w->right->color==1 && w->left->color == 1){
        w->color=0; //p의 형제를 red
        p=p->parent;
      }
      //case 7: p의 형제는 black, 형제의 왼쪽 자식은 red, 오른쪽 자식은 black인 경우
      else{
        if(w->left->color==1){
          w->right->color=1;
          w->color=0;
          left_rotation(t, w);
          w=p->parent->left;
        }
      //case 8: p의 형제는 black, 오른쪽 자식은 red인 경우
      w->color = p->parent->color;
      p->parent->color = 1;
      w->left->color = 1;
      right_rotation(t, p->parent);
      p = t->root;
      }
    }
  }
  p->color=1;
}
void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
  if(u->parent == t->nil)
    t->root = v;
  else if(u==u->parent->left)
    u->parent->left = v;
  else  
    u->parent->right = v;
  v->parent = u->parent;
}
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *y;
  node_t *x;
  color_t y_ori_color;

  y=p;
  y_ori_color = y->color;
  //삭제하려는 값의 자녀가 둘이라면 삭제되는 색은 노드의 successor의 색
  
  if(p->left == t->nil){
    x = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if(p->right == t->nil){
    x = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else{
    y = p->right;
    while(y->left != t->nil){
      y=y->left;
    }
    y_ori_color=y->color;
    x = y->right;

    if(y->parent==p)
      x->parent=y;
    else{
      rbtree_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rbtree_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }
  if(y_ori_color==1)
    rbtree_delete_fixup(t, x);
  free(p);

  return 0;

}

int inorder_array(node_t *p, key_t *arr, const rbtree *t, int i){
  if (p == t->nil) {
    return i;
  }
  i = inorder_array(p->left, arr, t, i);
  arr[i++] = p->key;
  i = inorder_array(p->right, arr, t, i);
  return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n){
  // int i = 0;
  node_t *tmp = t->root;
  inorder_array(tmp, arr, t, 0);
  return 0;
}


// int main(){
  // rbtree *t= new_rbtree();
  // rbtree_insert(t, 10);
  // rbtree_insert(t, 20);
  // rbtree_insert(t, 30);
  // rbtree_insert(t, 40);
  // rbtree_insert(t, 50);
  // node_t *a=rbtree_max(t);
  // key_t *res = calloc(6, sizeof(key_t));
//   test_to_array_suite();
// }