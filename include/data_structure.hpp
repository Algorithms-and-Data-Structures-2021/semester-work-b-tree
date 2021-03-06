#pragma once

#include <utility>
#include <unordered_set>
namespace itis {

  /// Структура для узлов в нашем дереве

  struct BNode {
    BNode **children;  // Массив указателей на детей
    int *keys;         // Массив ключей
    int size;          // Количество ключей
    bool leaf;         // Является ли узел листом
  };

  class BTree {
   public:
    // Конструктор
    // Параметр - минимальная степень дерева
    // Сложность O(1)
    explicit BTree(int);

    // Деструктор
    // Сложность O(n)
    ~BTree();

    // Вставка ключа в дерево
    // Сложность O(log n)
    void insert(int);

    // Убирает ключ из дерева
    // Бросает ошибку BTREE_EXCEPTION если не найден объект, который надо удалить
    // Сложность O(log n)
    int remove(int);

    // Функция поиска ключа в дереве
    // Сложность O(log n)
    std::pair<BNode *, int> search(int);

    // Использует поиск, но выводит только ключ
    // Бросает ошибку BTREE_EXCEPTION если не найден нужный объект
    // Сложность O(log n)
    int searchKey(int);

    // Принт
    // Сложность O(n)
    void print();

   private:
    // Нужна для инициализации узлов
    void initializeNode(BNode *);

    // Рекурсивная функция, используемая деструктором
    void freeNode(BNode *);

    // Находит индекс ключа в узле
    int findIndex(BNode *, int);

    // Добавляет ключ в узел
    int nodeInsert(BNode *, int);

    // Удаляет ключ по указанному индексу
    int nodeDelete(BNode *, int);

    // Функция для разделения заполненных узлов
    void splitChild(BNode *, int);

    // Объединяет двух детей из узла по указанному индексу в одного ребёнка
    int mergeChildren(BNode *, int);

    // Проверка того, что количество детей узла >= minDegree
    int fixChildSize(BNode *, int);

    // Рекурсия, принт поддерева
    void printNode(BNode *, int);

    // Корневой узел
    BNode *root;

    // Сравнение, позволяющее организовавать расположение элементов
    bool lessThan(int, int);

    // Вывод элементов в дереве
    void printKey(int);

    // Минимальная степень дерева
    int minDegree;
  };

}  // namespace itis