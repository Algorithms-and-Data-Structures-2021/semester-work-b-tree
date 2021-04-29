#include "data_structure.hpp"

#include <utility>
#include <cstdio>
#include <iostream>

using namespace std;

namespace itis {

  BTree::BTree(int t) : minDegree(t) {
    root = new BNode();
    initializeNode(root);
    root->leaf = true;
  }

  BTree::~BTree() {
    freeNode(root);
  }

  void BTree::insert(int k) {

    if (root->size == 2 * minDegree - 1) {
      BNode *newRoot = new BNode();
      initializeNode(newRoot);
      newRoot->leaf = false;
      newRoot->children[0] = root;
      root = newRoot;
      splitChild(newRoot, 0);
    }

    BNode *curr = root;
    while (!curr->leaf) {

      int index = curr->size - 1;
      while (index >= 0 && lessThan(k, curr->keys[index])) {
        index--;
      }
      index++;

      if (curr->children[index]->size == 2 * minDegree - 1) {
        splitChild(curr, index);
        if (lessThan(curr->keys[index], k)) {
          index++;
        }
      }
      curr = curr->children[index];
    }

    nodeInsert(curr, k);
  }

  int BTree::remove(int k) {
    BNode *curr = root;
    while (true) {
      int i = findIndex(curr, k);

      if (i < curr->size && !(lessThan(curr->keys[i], k) || lessThan(k, curr->keys[i]))) {
        int toReturn = curr->keys[i];
        if (curr->leaf) {
          nodeDelete(curr, i);
        }

        else {
          BNode *leftKid = curr->children[i];
          BNode *rightKid = curr->children[i + 1];

          if (leftKid->size >= minDegree) {
            while (!(leftKid->leaf)) {
              fixChildSize(leftKid, leftKid->size);
              leftKid = leftKid->children[leftKid->size];
            }
            curr->keys[i] = nodeDelete(leftKid, leftKid->size - 1);
          }

          else if (rightKid->size >= minDegree) {
            while (!(rightKid->leaf)) {
              fixChildSize(rightKid, 0);
              rightKid = rightKid->children[0];
            }
            curr->keys[i] = nodeDelete(rightKid, 0);
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

        if (curr->leaf) {
          throw std::invalid_argument("Remove node not found");
        }

        char result = fixChildSize(curr, i);
        if (result == 2) {
          curr = root;
        } else {
          curr = curr->children[findIndex(curr, k)];
        }
      }
    }
  }

  pair<BNode *, int> BTree::search(int k) {

    BNode *x = root;

    while (true) {

      int i = findIndex(x, k);

      if (i < x->size && !(lessThan(k, x->keys[i]) || lessThan(x->keys[i], k))) {
        return pair<BNode *, int>(x, i);
      }

      else if (x->leaf) {
        return pair<BNode *, int>(nullptr, 0);
      }

      else {
        x = x->children[i];
      }
    }
  }

  int BTree::searchKey(int k) {
    pair<BNode *, int> node = search(k);
    if (node.first == nullptr) {
      throw std::invalid_argument("Search node not found");
    }
    return node.first->keys[node.second];
  }

  void BTree::print() {
    if (root != nullptr) {
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
    x->keys = new int[(static_cast<unsigned long long int>(2 * minDegree - 1)) * sizeof(int)];
    x->children =
        reinterpret_cast<BNode **>(new BNode[static_cast<unsigned long long int>(2 * minDegree) * sizeof(BNode *)]);
  }

  void BTree::freeNode(BNode *x) {
    if (!x->leaf) {
      for (int i = 0; i <= x->size; i++) {
        freeNode(x->children[i]);
      }
    }
    delete (x->children);
    delete (x->keys);
    delete (x);
  }

  int BTree::findIndex(BNode *x, int k) {
    int i = 0;
    while (i < x->size && lessThan(x->keys[i], k)) {
      i++;
    }
    return i;
  }

  int BTree::nodeInsert(BNode *x, int k) {
    int index;

    for (index = x->size; index > 0 && lessThan(k, x->keys[index - 1]); index--) {
      x->keys[index] = x->keys[index - 1];
      x->children[index + 1] = x->children[index];
    }

    x->children[index + 1] = x->children[index];
    x->keys[index] = k;
    x->size++;

    return index;
  }

  int BTree::nodeDelete(BNode *x, int index) {

    int toReturn = x->keys[index];

    x->size--;
    while (index < x->size) {
      x->keys[index] = x->keys[index + 1];
      x->children[index + 1] = x->children[index + 2];
      index++;
    }
    return toReturn;
  }

  void BTree::splitChild(BNode *x, int i) {

    BNode *toSplit = x->children[i];
    BNode *newNode = new BNode;
    initializeNode(newNode);
    newNode->leaf = toSplit->leaf;
    newNode->size = minDegree - 1;

    for (int j = 0; j < minDegree - 1; j++) {
      newNode->keys[j] = toSplit->keys[j + minDegree];
    }
    if (!toSplit->leaf) {
      for (int j = 0; j < minDegree; j++) {
        newNode->children[j] = toSplit->children[j + minDegree];
      }
    }
    toSplit->size = minDegree - 1;

    nodeInsert(x, toSplit->keys[minDegree - 1]);
    x->children[i + 1] = newNode;
  }

  int BTree::mergeChildren(BNode *parent, int i) {

    BNode *leftKid = parent->children[i];
    BNode *rightKid = parent->children[i + 1];

    leftKid->keys[leftKid->size] = nodeDelete(parent, i);
    int j = ++(leftKid->size);

    for (int k = 0; k < rightKid->size; k++) {
      leftKid->keys[j + k] = rightKid->keys[k];
      leftKid->children[j + k] = rightKid->children[k];
    }
    leftKid->size += rightKid->size;
    leftKid->children[leftKid->size] = rightKid->children[rightKid->size];

    delete (rightKid->children);
    delete (rightKid->keys);
    delete (rightKid);

    if (parent->size == 0) {
      root = leftKid;
      delete (parent->children);
      delete (parent->keys);
      delete (parent);
      return 2;
    }

    return 1;
  }

  int BTree::fixChildSize(BNode *parent, int index) {
    BNode *kid = parent->children[index];

    if (kid->size < minDegree) {

      if (index != 0 && parent->children[index - 1]->size >= minDegree) {
        BNode *leftKid = parent->children[index - 1];

        for (int i = nodeInsert(kid, parent->keys[index - 1]); i != 0; i--) {
          kid->children[i] = kid->children[i - 1];
        }
        kid->children[0] = leftKid->children[leftKid->size];
        parent->keys[index - 1] = nodeDelete(leftKid, leftKid->size - 1);
      }

      else if (index != parent->size && parent->children[index + 1]->size >= minDegree) {
        BNode *rightKid = parent->children[index + 1];
        nodeInsert(kid, parent->keys[index]);
        kid->children[kid->size] = rightKid->children[0];
        rightKid->children[0] = rightKid->children[1];
        parent->keys[index] = nodeDelete(rightKid, 0);
      }

      else if (index != 0) {
        return mergeChildren(parent, index - 1);
      } else {
        return mergeChildren(parent, index);
      }
      return 1;
    }

    return 0;
  }

  void BTree::printNode(BNode *node, int tab) {

    for (int i = 0; i < tab; i++) {
      printf("\t");
    }

    for (int i = 0; i < node->size; i++) {
      printKey(node->keys[i]);
      printf(" ");
    }
    printf("\n");

    if (!node->leaf) {
      tab++;
      for (int i = 0; i <= node->size; i++) {
        printNode(node->children[i], tab);
      }
    }
  }

  bool BTree::lessThan(int a, int b) {
    return a < b;
  }
}  // namespace itis