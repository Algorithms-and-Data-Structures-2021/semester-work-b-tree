#include "data_structure.hpp"

#include <cstdlib>
#include <utility>
#include <cstdio>
#include <iostream>

using namespace std;

namespace itis {

#define NEW_ROOT 2
#define MODIFIED_NOT_ROOT 1
#define NOT_MODIFIED 0

  BTree::BTree(unsigned t) {
    minDegree = t;
    root = reinterpret_cast<BNode *>(malloc(sizeof(BNode)));
    initializeNode(root);
    root->isLeaf = true;
  }

  BTree::~BTree() {
    freeNode(root);
  }

  void BTree::insert(int k) {

    if (root->size == 2 * minDegree - 1) {
      BNode *newRoot = reinterpret_cast<BNode *>(malloc(sizeof(BNode)));
      initializeNode(newRoot);
      newRoot->isLeaf = false;
      newRoot->children[0] = root;
      root = newRoot;
      splitChild(newRoot, 0);
    }

    BNode *curr = root;
    while (!curr->isLeaf) {

      int index = static_cast<int>(curr->size - 1);
      while (index >= 0 && lessThan(k, curr->key[index])) {
        index--;
      }
      index++;

      if (curr->children[index]->size == 2 * minDegree - 1) {
        splitChild(curr, index);
        if (lessThan(curr->key[index], k)) {
          index++;
        }
      }
      curr = curr->children[index];
    }

    insertKeyToNode(curr, k);
  }

  int BTree::remove(int k) {
    BNode *curr = root;
    while (true) {
      unsigned i = findIndexOfKeyInNode(curr, k);

      if (i < curr->size && !(lessThan(curr->key[i], k) || lessThan(k, curr->key[i]))) {
        int toReturn = curr->key[i];
        if (curr->isLeaf) {
          deleteKeyInNode(curr, i);
        }

        else {
          BNode *leftKid = curr->children[i];
          BNode *rightKid = curr->children[i + 1];

          if (leftKid->size >= minDegree) {
            while (!(leftKid->isLeaf)) {
              fixChildSize(leftKid, leftKid->size);
              leftKid = leftKid->children[leftKid->size];
            }
            curr->key[i] = deleteKeyInNode(leftKid, leftKid->size - 1);
          }

          else if (rightKid->size >= minDegree) {
            while (!(rightKid->isLeaf)) {
              fixChildSize(rightKid, 0);
              rightKid = rightKid->children[0];
            }
            curr->key[i] = deleteKeyInNode(rightKid, 0);
          }

          else {
            mergeChildren(curr, i);
            curr = leftKid;
            continue;
          }
        }
        return toReturn;
      }

      else {

        if (curr->isLeaf) {
          throw(BTREE_EXCEPTION) REMOVE_KEY_NOT_FOUND;
        }

        char result = fixChildSize(curr, i);
        if (result == NEW_ROOT) {
          curr = root;
        } else {
          curr = curr->children[findIndexOfKeyInNode(curr, k)];
        }
      }
    }
  }

  pair<BNode *, unsigned> BTree::search(int k) {

    BNode *x = root;


    while (true) {

      unsigned i = findIndexOfKeyInNode(x, k);

      if (i < x->size && !(lessThan(k, x->key[i]) || lessThan(x->key[i], k))) {
        return pair<BNode *, unsigned>(x, i);
      }

      else if (x->isLeaf) {
        return pair<BNode *, unsigned>(NULL, 0);
      }

      else {
        x = x->children[i];
      }
    }
  }

  int BTree::searchKey(int k) {
    pair<BNode *, unsigned> node = search(k);
    if (node.first == NULL) {
      throw(BTREE_EXCEPTION) SEARCH_KEY_NOT_FOUND;
    }
    return node.first->key[node.second];
  }

  void BTree::print() {
    if (root != NULL) {
      printf("\n");
      printNode(root, 0);
      printf("\n");
    }
  }

  void BTree::printKey(int a) {
    printf(reinterpret_cast<const char *>(a));
  }

  void BTree::initializeNode(BNode *x) {
    x->size = 0;
    x->key = reinterpret_cast<int *>(malloc((2 * minDegree - 1) * sizeof(int)));
    x->children = reinterpret_cast<BNode **>(malloc(2 * minDegree * sizeof(BNode *)));
  }

  void BTree::freeNode(BNode *x) {
    if (!x->isLeaf) {
      for (unsigned i = 0; i <= x->size; i++) {
        freeNode(x->children[i]);
      }
    }
    free(x->children);
    free(x->key);
    free(x);
  }

  unsigned BTree::findIndexOfKeyInNode(BNode *x, int k) {
    unsigned i = 0;
    while (i < x->size && lessThan(x->key[i], k)) {
      i++;
    }
    return i;
  }

  unsigned BTree::insertKeyToNode(BNode *x, int k) {
    int index;

    for (index = static_cast<int>(x->size); index > 0 && lessThan(k, x->key[index - 1]); index--) {
      x->key[index] = x->key[index - 1];
      x->children[index + 1] = x->children[index];
    }

    x->children[index + 1] = x->children[index];
    x->key[index] = k;
    x->size++;

    return static_cast<unsigned int>(index);
  }

  int BTree::deleteKeyInNode(BNode *x, unsigned index) {

    int toReturn = x->key[index];

    x->size--;
    while (index < x->size) {
      x->key[index] = x->key[index + 1];
      x->children[index + 1] = x->children[index + 2];
      index++;
    }
    return toReturn;
  }

  void BTree::splitChild(BNode *x, int i) {

    BNode *toSplit = x->children[i];
    BNode *newNode = reinterpret_cast<BNode *>(malloc(sizeof(BNode)));
    initializeNode(newNode);
    newNode->isLeaf = toSplit->isLeaf;
    newNode->size = minDegree - 1;

    for (unsigned j = 0; j < minDegree - 1; j++) {
      newNode->key[j] = toSplit->key[j + minDegree];
    }
    if (!toSplit->isLeaf) {
      for (unsigned j = 0; j < minDegree; j++) {
        newNode->children[j] = toSplit->children[j + minDegree];
      }
    }
    toSplit->size = minDegree - 1;

    deleteKeyInNode(x, toSplit->key[minDegree - 1]);
    x->children[i + 1] = newNode;
  }

  char BTree::mergeChildren(BNode *parent, unsigned i) {

    BNode *leftKid = parent->children[i];
    BNode *rightKid = parent->children[i + 1];

    leftKid->key[leftKid->size] = deleteKeyInNode(parent, i);
    unsigned j = ++(leftKid->size);

    for (unsigned k = 0; k < rightKid->size; k++) {
      leftKid->key[j + k] = rightKid->key[k];
      leftKid->children[j + k] = rightKid->children[k];
    }
    leftKid->size += rightKid->size;
    leftKid->children[leftKid->size] = rightKid->children[rightKid->size];

    free(rightKid->children);
    free(rightKid->key);
    free(rightKid);

    if (parent->size == 0) {
      root = leftKid;
      free(parent->children);
      free(parent->key);
      free(parent);
      return NEW_ROOT;
    }

    return MODIFIED_NOT_ROOT;
  }

  char BTree::fixChildSize(BNode *parent, unsigned index) {
    BNode *kid = parent->children[index];

    if (kid->size < minDegree) {

      if (index != 0 && parent->children[index - 1]->size >= minDegree) {
        BNode *leftKid = parent->children[index - 1];

        for (unsigned i = deleteKeyInNode(kid, parent->key[index - 1]); i != 0; i--) {
          kid->children[i] = kid->children[i - 1];
        }
        kid->children[0] = leftKid->children[leftKid->size];
        parent->key[index - 1] = deleteKeyInNode(leftKid, leftKid->size - 1);
      }

      else if (index != parent->size && parent->children[index + 1]->size >= minDegree) {
        BNode *rightKid = parent->children[index + 1];
        deleteKeyInNode(kid, parent->key[index]);
        kid->children[kid->size] = rightKid->children[0];
        rightKid->children[0] = rightKid->children[1];
        parent->key[index] = deleteKeyInNode(rightKid, 0);
      }

      else if (index != 0) {
        return mergeChildren(parent, index - 1);
      } else {
        return mergeChildren(parent, index);
      }
      return MODIFIED_NOT_ROOT;
    }

    return NOT_MODIFIED;
  }

  void BTree::printNode(BNode *node, unsigned tab) {

    for (unsigned i = 0; i < tab; i++) {
      printf("\t");
    }

    for (unsigned i = 0; i < node->size; i++) {
      printKey(node->key[i]);
      printf(" ");
    }
    printf("\n");

    if (!node->isLeaf) {
      tab++;
      for (unsigned i = 0; i <= node->size; i++) {
        printNode(node->children[i], tab);
      }
    }
  }

  bool BTree::lessThan(int a, int b) {
    return a < b;
  }
}  // namespace itis
