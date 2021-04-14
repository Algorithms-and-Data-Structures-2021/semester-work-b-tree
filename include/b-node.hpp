//
// Created by AndrewOch on 13.04.2021.
//

#ifndef SEMESTER_WORK_B_TREE_BNODE_HPP
#define SEMESTER_WORK_B_TREE_BNODE_HPP

const int t = 2;
struct BNode {
  int keys[2 * t];
  BNode *children[2 * t + 1];
  BNode *parent;
  int count;
  int countChildren;
  bool leaf;
};

#endif  //SEMESTER_WORK_B_TREE_BNODE_HPP