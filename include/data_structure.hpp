#pragma once

#include <utility>
#include <unordered_set>
namespace itis {

#define NULL 0
#define SEARCH_KEY_NOT_FOUND 's'
#define REMOVE_KEY_NOT_FOUND 'r'

  // Структура для узлов в нашем дереве
  template<typename T>
  struct BNode {
    BNode<T> **child;  // Массив указателей на детей
    T *key;            // Массив ключей
    unsigned size;     // Количество ключей
    bool leaf;         // Является ли узел листом
  };

  typedef char BTREE_EXCEPTION;

  template<typename T>
  class BTree {
   public:
    // Конструктор
    // Первый параметр - минимальная степень дерева
    // Второй - функция сравнения ключей
    // Третий - функция, которая принтит ключи
    // Сложность O(1)
    BTree(unsigned, bool (*)(T, T), void (*)(T) = NULL);

    // Деструктор
    // Сложность O(n)
    ~BTree<T>();

    // Вставка ключа в дерево
    // Сложность O(log n)
    void insert(T);

    // Убирает ключ из дерева
    // Бросает ошибку BTREE_EXCEPTION если не найден объект, который надо удалить
    // Сложность O(log n)
    T remove(T);

    // Функция поиска ключа в дереве
    // Сложность O(log n)
    std::pair<BNode<T> *, unsigned> search(T);

    // Использует поиск, но выводит только ключ
    // Бросает ошибку BTREE_EXCEPTION если не найден нужный объект
    // Сложность O(log n)
    T searchKey(T);

    // Принт
    // Сложность O(n)
    void print();

   private:
    // Нужна для инициализации узлов
    void initializeNode(BNode<T> *);

    // Рекурсивная функция, используемая деструктором
    void freeNode(BNode<T> *);

    // Находит индекс ключа в узле
    unsigned findIndex(BNode<T> *, T);

    // Добавляет ключ в узел
    unsigned nodeInsert(BNode<T> *, T);

    // Удаляет ключ по указанному индексу
    T nodeDelete(BNode<T> *, unsigned);

    // Функция для разделения заполненных узлов
    void splitChild(BNode<T> *, int);

    // Объединяет двух детей из узла по указанному индексу в одного ребёнка
    char mergeChildren(BNode<T> *, unsigned);

    // Проверка того, что количество детей узла >= minDegree
    char fixChildSize(BNode<T> *, unsigned);

    // Рекурсия, принт поддерева
    void printNode(BNode<T> *, unsigned);

    // Корневой узел
    BNode<T> *root;

    // Функция сравнения, позволяющая организовавать расположение элементов
    bool (*lessThan)(T, T);

    // Функция принта элементов в дереве
    void (*printKey)(T);

    // Минимальная степень дерева
    unsigned minDegree;
  };

#include "data_structure.hpp"
}  // namespace itis