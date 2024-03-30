#include <iostream>
#include <cmath>
#include <vector>

template<typename T>
struct BinomialTreeNode {
    T value;
    int degree;
    BinomialTreeNode<T>* parent;
    BinomialTreeNode<T>* child;
    BinomialTreeNode<T>* sibling;

    BinomialTreeNode(T val) : value(val), degree(0), parent(nullptr), child(nullptr), sibling(nullptr) {}
};

template<typename T>
class BinomialHeap {
private:
    std::vector<BinomialTreeNode<T>*> trees;

    BinomialTreeNode<T>* mergeTrees(BinomialTreeNode<T>* tree1, BinomialTreeNode<T>* tree2) {
        if (tree1->value > tree2->value)
            std::swap(tree1, tree2);
        
        tree2->parent = tree1;
        tree2->sibling = tree1->child;
        tree1->child = tree2;
        tree1->degree++;
        return tree1;
    }

    void mergeHeap(std::vector<BinomialTreeNode<T>*>& heap1, std::vector<BinomialTreeNode<T>*>& heap2) {
        std::vector<BinomialTreeNode<T>*> merged;
        int i = 0, j = 0;
        BinomialTreeNode<T>* carry = nullptr;

        while (i < heap1.size() || j < heap2.size() || carry != nullptr) {
            BinomialTreeNode<T>* tree1 = (i < heap1.size()) ? heap1[i++] : nullptr;
            BinomialTreeNode<T>* tree2 = (j < heap2.size()) ? heap2[j++] : nullptr;

            if (carry) {
                if (tree1 && tree2) {
                    if (tree1->degree < tree2->degree)
                        carry = mergeTrees(carry, tree1);
                    else
                        carry = mergeTrees(carry, tree2);
                } else if (tree1)
                    carry = mergeTrees(carry, tree1);
                else
                    carry = mergeTrees(carry, tree2);

                if (!carry->sibling)
                    merged.push_back(carry);
                carry = nullptr;
            } else {
                if (tree1 && tree2) {
                    if (tree1->degree < tree2->degree)
                        merged.push_back(tree1);
                    else
                        merged.push_back(tree2);
                } else if (tree1)
                    merged.push_back(tree1);
                else
                    merged.push_back(tree2);

                if (tree1 && tree2 && tree1->degree == tree2->degree)
                    carry = mergeTrees(tree1, tree2);
            }
        }

        heap1 = merged;
    }

public:
    BinomialHeap() {}

    void insert(T value) {
        std::vector<BinomialTreeNode<T>*> singleton(1, new BinomialTreeNode<T>(value));
        mergeHeap(trees, singleton);
    }

    T extractMin() {
        if (trees.empty()) {
            std::cerr << "Heap is empty!" << std::endl;
            exit(EXIT_FAILURE);
        }

        auto minTree = trees[0];
        for (auto tree : trees) {
            if (tree->value < minTree->value)
                minTree = tree;
        }

        T minValue = minTree->value;

        std::vector<BinomialTreeNode<T>*> children;
        for (auto child = minTree->child; child; child = child->sibling)
            children.push_back(child);

        std::vector<BinomialTreeNode<T>*> newHeap;
        mergeHeap(newHeap, children);

        trees.erase(std::remove(trees.begin(), trees.end(), minTree), trees.end());
        delete minTree;

        mergeHeap(trees, newHeap);

        return minValue;
    }
};

int main() {
    BinomialHeap<int> heap;
    heap.insert(5);
    heap.insert(7);
    heap.insert(3);
    heap.insert(2);
    heap.insert(9);

    std::cout << "Min Value Extracted: " << heap.extractMin() << std::endl;
    
    return 0;
}

