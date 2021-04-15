//
// Created by AndrewOch on 13.04.2021.
//

#ifndef SEMESTER_WORK_B_TREE_BTREE_HPP
#define SEMESTER_WORK_B_TREE_BTREE_HPP

#include "b-node.hpp"

struct BTree {
 private:
  BNode *root;

  void insert_to_node(int key, BNode *node);

  void sort(BNode *node);

  void restruct(BNode *node);

  void deleteNode(BNode *node);

  bool searchKey(int key, BNode *node);

  void remove(int key, BNode *node);

  void removeFromNode(int key, BNode *node);

  void removeLeaf(int key, BNode *node);

  void lconnect(BNode *first_node, BNode *second_node);

  void rconnect(BNode *first_node, BNode *second_node);

  void repair(BNode *node);

 public:
  BTree();

  ~BTree();

  void insert(int key);

  bool search(int key);

  void remove(int key);
};

#endif  //SEMESTER_WORK_B_TREE_BTREE_HPP