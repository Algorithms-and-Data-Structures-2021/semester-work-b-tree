#include "data_structure.hpp"

#pragma once

#include <stdlib.h>
#include <utility>
#include <stdio.h>

using namespace std;

namespace itis {

#define NEW_ROOT 2
#define MODIFIED_NOT_ROOT 1
#define NOT_MODIFIED 0

  template<typename T>
  BTree<T>::BTree(unsigned t, bool (*compare)(T, T), void (*printK)(T)) {
    minDegree = t;
    lessThan = compare;
    root = (BNode<T> *) malloc(sizeof(BNode<T>));
    initializeNode(root);
    root->leaf = true;
    printKey = printK;
  }

  template<typename T>
  BTree<T>::~BTree<T>() {
    freeNode(root);
  }

  template<typename T>
  void BTree<T>::insert(T k) {

    if (root->size == 2 * minDegree - 1) {
      BNode<T> *newRoot = (BNode<T> *) malloc(sizeof(BNode<T>));
      initializeNode(newRoot);
      newRoot->leaf = false;
      newRoot->child[0] = root;
      root = newRoot;
      splitChild(newRoot, 0);
    }

    BNode<T> *curr = root;
    while (!curr->leaf) {

      int index = curr->size - 1;
      while (index >= 0 && lessThan(k, curr->key[index])) {
        index--;
      }
      index++;

      if (curr->child[index]->size == 2 * minDegree - 1) {
        splitChild(curr, index);
        if (lessThan(curr->key[index], k)) {
          index++;
        }
      }
      curr = curr->child[index];
    }

    nodeInsert(curr, k);
  }

  template<typename T>
  T BTree<T>::remove(T k) {
    BNode<T> *curr = root;
    while (true) {
      unsigned i = findIndex(curr, k);

      if (i < curr->size && !(lessThan(curr->key[i], k) || lessThan(k, curr->key[i]))) {
        T toReturn = curr->key[i];
        if (curr->leaf) {
          nodeDelete(curr, i);
        }

        else {
          BNode<T> *leftKid = curr->child[i];
          BNode<T> *rightKid = curr->child[i + 1];

          if (leftKid->size >= minDegree) {
            while (!(leftKid->leaf)) {
              fixChildSize(leftKid, leftKid->size);
              leftKid = leftKid->child[leftKid->size];
            }
            curr->key[i] = nodeDelete(leftKid, leftKid->size - 1);
          }

          else if (rightKid->size >= minDegree) {
            while (!(rightKid->leaf)) {
              fixChildSize(rightKid, 0);
              rightKid = rightKid->child[0];
            }
            curr->key[i] = nodeDelete(rightKid, 0);
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
          throw(BTREE_EXCEPTION) REMOVE_KEY_NOT_FOUND;
        }

        char result = fixChildSize(curr, i);
        if (result == NEW_ROOT) {
          curr = root;
        } else {
          curr = curr->child[findIndex(curr, k)];
        }
      }
    }
  }

  template<typename T>
  pair<BNode<T> *, unsigned> BTree<T>::search(T k) {

    BNode<T> *x = root;

    while (true) {

      unsigned i = findIndex(x, k);

      if (i < x->size && !(lessThan(k, x->key[i]) || lessThan(x->key[i], k))) {
        return pair<BNode<T> *, unsigned>(x, i);
      }

      else if (x->leaf) {
        return pair<BNode<T> *, unsigned>(NULL, 0);
      }

      else {
        x = x->child[i];
      }
    }
  }

  template<typename T>
  T BTree<T>::searchKey(T k) {
    pair<BNode<T> *, unsigned> node = search(k);
    if (node.first == NULL) {
      throw(BTREE_EXCEPTION) SEARCH_KEY_NOT_FOUND;
    }
    return node.first->key[node.second];
  }

  template<typename T>
  void BTree<T>::print() {
    if (printKey != NULL && root != NULL) {
      printf("\n");
      printNode(root, 0);
      printf("\n");
    }
  }

  template<typename T>
  void BTree<T>::initializeNode(BNode<T> *x) {
    x->size = 0;
    x->key = (T *) malloc((2 * minDegree - 1) * sizeof(T));
    x->child = (BNode<T> **) malloc(2 * minDegree * sizeof(BNode<T> *));
  }

  template<typename T>
  void BTree<T>::freeNode(BNode<T> *x) {
    if (!x->leaf) {
      for (unsigned i = 0; i <= x->size; i++) {
        freeNode(x->child[i]);
      }
    }
    free(x->child);
    free(x->key);
    free(x);
  }

  template<typename T>
  unsigned BTree<T>::findIndex(BNode<T> *x, T k) {
    unsigned i = 0;
    while (i < x->size && lessThan(x->key[i], k)) {
      i++;
    }
    return i;
  }

  template<typename T>
  unsigned BTree<T>::nodeInsert(BNode<T> *x, T k) {
    int index;

    for (index = x->size; index > 0 && lessThan(k, x->key[index - 1]); index--) {
      x->key[index] = x->key[index - 1];
      x->child[index + 1] = x->child[index];
    }

    x->child[index + 1] = x->child[index];
    x->key[index] = k;
    x->size++;

    return static_cast<unsigned int>(index);
  }

  template<typename T>
  T BTree<T>::nodeDelete(BNode<T> *x, unsigned index) {

    T toReturn = x->key[index];

    x->size--;
    while (index < x->size) {
      x->key[index] = x->key[index + 1];
      x->child[index + 1] = x->child[index + 2];
      index++;
    }
    return toReturn;
  }

  template<typename T>
  void BTree<T>::splitChild(BNode<T> *x, int i) {

    BNode<T> *toSplit = x->child[i];
    BNode<T> *newNode = (BNode<T> *) malloc(sizeof(BNode<T>));
    initializeNode(newNode);
    newNode->leaf = toSplit->leaf;
    newNode->size = minDegree - 1;

    for (unsigned j = 0; j < minDegree - 1; j++) {
      newNode->key[j] = toSplit->key[j + minDegree];
    }
    if (!toSplit->leaf) {
      for (unsigned j = 0; j < minDegree; j++) {
        newNode->child[j] = toSplit->child[j + minDegree];
      }
    }
    toSplit->size = minDegree - 1;

    nodeInsert(x, toSplit->key[minDegree - 1]);
    x->child[i + 1] = newNode;
  }

  template<typename T>
  char BTree<T>::mergeChildren(BNode<T> *parent, unsigned i) {

    BNode<T> *leftKid = parent->child[i];
    BNode<T> *rightKid = parent->child[i + 1];

    leftKid->key[leftKid->size] = nodeDelete(parent, i);
    unsigned j = ++(leftKid->size);

    for (unsigned k = 0; k < rightKid->size; k++) {
      leftKid->key[j + k] = rightKid->key[k];
      leftKid->child[j + k] = rightKid->child[k];
    }
    leftKid->size += rightKid->size;
    leftKid->child[leftKid->size] = rightKid->child[rightKid->size];

    free(rightKid->child);
    free(rightKid->key);
    free(rightKid);

    if (parent->size == 0) {
      root = leftKid;
      free(parent->child);
      free(parent->key);
      free(parent);
      return NEW_ROOT;
    }

    return MODIFIED_NOT_ROOT;
  }

  template<typename T>
  char BTree<T>::fixChildSize(BNode<T> *parent, unsigned index) {
    BNode<T> *kid = parent->child[index];

    if (kid->size < minDegree) {

      if (index != 0 && parent->child[index - 1]->size >= minDegree) {
        BNode<T> *leftKid = parent->child[index - 1];

        for (unsigned i = nodeInsert(kid, parent->key[index - 1]); i != 0; i--) {
          kid->child[i] = kid->child[i - 1];
        }
        kid->child[0] = leftKid->child[leftKid->size];
        parent->key[index - 1] = nodeDelete(leftKid, leftKid->size - 1);
      }

      else if (index != parent->size && parent->child[index + 1]->size >= minDegree) {
        BNode<T> *rightKid = parent->child[index + 1];
        nodeInsert(kid, parent->key[index]);
        kid->child[kid->size] = rightKid->child[0];
        rightKid->child[0] = rightKid->child[1];
        parent->key[index] = nodeDelete(rightKid, 0);
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

  template<typename T>
  void BTree<T>::printNode(BNode<T> *node, unsigned tab) {

    for (unsigned i = 0; i < tab; i++) {
      printf("\t");
    }

    for (unsigned i = 0; i < node->size; i++) {
      printKey(node->key[i]);
      printf(" ");
    }
    printf("\n");

    if (!node->leaf) {
      tab++;
      for (unsigned i = 0; i <= node->size; i++) {
        printNode(node->child[i], tab);
      }
    }
  }

}  // namespace itis