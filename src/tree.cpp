//
// Created by AndrewOch on 13.04.2021.
//

#include "../include/tree.hpp"

void BTree::insert_to_node(int key, BNode *node) {
  node->keys[node->count] = key;
  node->count = node->count + 1;
  sort(node);
}

void BTree::sort(BNode *node) {
  int m = 0;
  for (int i = 0; i < (2 * t - 1); ++i) {
    for (int j = i + 1; j <= (2 * t - 1); ++j) {
      if ((node->keys[i] != 0) && (node->keys[j] != 0)) {
        if (node->keys[i] > node->keys[j]) {
          m = node->keys[i];
          node->keys[i] = node->keys[j];
          node->keys[j] = m;
        }
      } else {
        break;
      }
    }
  }
}

//TODO restruct
void BTree::restruct(BNode *node) {}

bool BTree::searchKey(int key, BNode *node) {
  return false;
}
//TODO remove
void BTree::remove(int key, BNode *node) {}

//TODO remove from node
void BTree::removeFromNode(int key, BNode *node) {}

//TODO remove leaf
void BTree::removeLeaf(int key, BNode *node) {}

//TODO left connect
void BTree::lconnect(BNode *first_node, BNode *second_node) {}

//TODO right connect
void BTree::rconnect(BNode *first_node, BNode *second_node) {}

//TODO repair
void BTree::repair(BNode *node) {}

BTree::BTree() : root(nullptr) {}

BTree::~BTree() {
  if (root != nullptr) {
    deleteNode(root);
  }
}

void BTree::deleteNode(BNode *node) {
  if (node != nullptr) {
    for (int i = 0; i <= (2 * t - 1); ++i) {
      if (node->children[i] != nullptr) {
        deleteNode(node->children[i]);
      } else {
        delete (node);
        break;
      }
    }
  }
}

void BTree::insert(int key) {
  if (root == nullptr) {
    auto *newRoot = new BNode;
    newRoot->keys[0] = key;
    for (int i = 1; i <= (2 * t - 1); ++i) {
      newRoot->keys[i] = 0;
    }
    for (int i = 0; i <= (2 * t - 1); ++i) {
      newRoot->children[i] = nullptr;
    }
    newRoot->count = 1;
    newRoot->countChildren = 0;
    newRoot->leaf = true;
    newRoot->parent = nullptr;
  } else {
    BNode *ptr = root;
    while (!ptr->leaf) {
      for (int i = 0; i <= (2 * t - 1); ++i) {
        if (ptr->keys[i] != 0) {
          if (key < ptr->keys[i]) {
            ptr = ptr->children[i];
            break;
          }
          if ((ptr->keys[i + 1] == 0) && (key > ptr->keys[i])) {
            ptr = ptr->children[i + 1];
            break;
          }
        } else
          break;
      }
    }
    insert_to_node(key, ptr);
    while (ptr->count == 2 * t) {
      if (ptr == root) {
        restruct(ptr);
        break;
      } else {
        restruct(ptr);
        ptr = ptr->parent;
      }
    }
  }
}

//TODO search
void BTree::search(int key) {}

//TODO remove
void BTree::remove(int key) {}