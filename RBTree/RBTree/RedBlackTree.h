#include <windows.h>
#include <queue>
#include <string>

enum Color { RED, BLACK };

struct RBTreeNode {
    int value;
    Color color;
    RBTreeNode* left;
    RBTreeNode* right;
    RBTreeNode* parent;

    RBTreeNode(int val) : value(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBTree {
public:
    RBTree() : NIL(new RBTreeNode(0)), root(NIL)
    {
        NIL->color = BLACK;
        NIL->left = NIL;
        NIL->right = NIL;
        NIL->parent = NIL;
    }

    ~RBTree()
    {
        deleteSubtree(root);
        delete NIL;
    }

    void Insert(int value) {
        RBTreeNode* node = new RBTreeNode(value);
        node->parent = nullptr;
        node->left = NIL;
        node->right = NIL;
        node->color = RED;

        RBTreeNode* y = nullptr;
        RBTreeNode* x = this->root;

        while (x != NIL) {
            y = x;
            if (node->value < x->value)
            {
                x = x->left;
            }
            else {
                x = x->right;
            }
        }

        node->parent = y;
        if (y == nullptr) {
            root = node;
        }
        else if (node->value < y->value)
        {
            y->left = node;
        }
        else {
            y->right = node;
        }

        if (node->parent == nullptr)
        {
            node->color = BLACK;
            return;
        }

        if (node->parent->parent == nullptr)
        {
            return;
        }

        fixInsert(node);
    }

    void remove(int value)
    {
        removeNode(value);
    }

    void draw(HDC hdc) const
    {
        drawNode(hdc, root, 400, 50, 200);
    }

private:
    RBTreeNode* NIL;
    RBTreeNode* root;

    void deleteSubtree(RBTreeNode* node)
    {
        if (node != NIL)
        {
            deleteSubtree(node->left);
            deleteSubtree(node->right);
            delete node;
        }
    }

    void fixInsert(RBTreeNode* node)
    {
        while (node != root && node->parent->color == RED)
        {
            if (node->parent == node->parent->parent->left)
            {
                RBTreeNode* uncle = node->parent->parent->right;
                if (uncle->color == RED)
                {
                    // Case 1
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->right)
                    {
                        // Case 2
                        node = node->parent;
                        leftRotate(node);
                    }
                    // Case 3
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    rightRotate(node->parent->parent);
                }
            }
            else
            {
                RBTreeNode* uncle = node->parent->parent->left;
                if (uncle->color == RED)
                {
                    // Case 4
                    node->parent->color = BLACK;
                    uncle->color = BLACK;
                    node->parent->parent->color = RED;
                    node = node->parent->parent;
                }
                else
                {
                    if (node == node->parent->left)
                    {
                        // Case 5
                        node = node->parent;
                        rightRotate(node);
                    }
                    // Case 6
                    node->parent->color = BLACK;
                    node->parent->parent->color = RED;
                    leftRotate(node->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    void fixDelete(RBTreeNode* x)
    {
        while (x != root && x->color == BLACK)
        {
            if (x->parent == nullptr)
            {
                break;
            }
            if (x == x->parent->left)
            {
                RBTreeNode* w = x->parent->right;
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if (w->left->color == BLACK && w->right->color == BLACK)
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (w->right->color == BLACK)
                    {
                        w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            }
            else
            {
                RBTreeNode* w = x->parent->left;
                if (w->color == RED)
                {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if (w->right->color == BLACK && w->left->color == BLACK)
                {
                    w->color = RED;
                    x = x->parent;
                }
                else
                {
                    if (w->left->color == BLACK)
                    {
                        w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    w->color = x->parent->color;
                    x->parent->color = BLACK;
                    w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        x->color = BLACK;
    }

    void transplant(RBTreeNode* u, RBTreeNode* v)
    {
        if (u->parent == nullptr)
        {
            root = v;
        }
        else if (u == u->parent->left)
        {
            u->parent->left = v;
        }
        else
        {
            u->parent->right = v;
        }
        v->parent = u->parent;
    }

    void removeNode(int key)
    {
        RBTreeNode* z = root;
        while (z != NIL)
        {
            if (z->value == key)
            {
                break;
            }
            else if (key < z->value)
            {
                z = z->left;
            }
            else
            {
                z = z->right;
            }
        }

        if (z == NIL)
        {
            return;
        }

        RBTreeNode* y = z;
        Color yOriginalColor = y->color;
        RBTreeNode* x;

        if (z->left == NIL)
        {
            x = z->right;
            transplant(z, z->right);
        }
        else if (z->right == NIL)
        {
            x = z->left;
            transplant(z, z->left);
        }
        else
        {
            y = maximum(z->left);
            yOriginalColor = y->color;
            x = y->left;

            if (y->parent == z)
            {
                x->parent = y;
            }
            else
            {
                transplant(y, y->left);
                y->left = z->left;
                y->left->parent = y;
            }

            transplant(z, y);
            y->right = z->right;
            y->right->parent = y;
            y->color = z->color;
        }

        delete z;

        if (yOriginalColor == BLACK)
        {
            fixDelete(x);
        }
    }
    RBTreeNode* maximum(RBTreeNode* node)
    {
        while (node->right != NIL)
        {
            node = node->right;
        }
        return node;
    }

    void leftRotate(RBTreeNode* x)
    {
        RBTreeNode* y = x->right;
        x->right = y->left;
        if (y->left != NIL)
        {
            y->left->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->left)
        {
            x->parent->left = y;
        }
        else
        {
            x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
    }

    void rightRotate(RBTreeNode* x)
    {
        RBTreeNode* y = x->left;
        x->left = y->right;
        if (y->right != NIL)
        {
            y->right->parent = x;
        }
        y->parent = x->parent;
        if (x->parent == nullptr)
        {
            this->root = y;
        }
        else if (x == x->parent->right)
        {
            x->parent->right = y;
        }
        else
        {
            x->parent->left = y;
        }
        y->right = x;
        x->parent = y;
    }

    void drawNode(HDC hdc, RBTreeNode* node, int x, int y, int offset) const
    {
        if (node == NIL) return;

        // 노드 원 그리기
        HBRUSH hBrush;
        if (node->color == RED)
        {
            hBrush = CreateSolidBrush(RGB(255, 0, 0));
        }
        else
        {
            hBrush = CreateSolidBrush(RGB(0, 0, 0));
        }
        SelectObject(hdc, hBrush);
        Ellipse(hdc, x - 20, y - 20, x + 20, y + 20);
        DeleteObject(hBrush);

        // 노드 값 출력
        wchar_t temp[32];
        swprintf_s(temp, sizeof(temp) / sizeof(wchar_t), L"%d", node->value);
        TextOut(hdc, x - 10, y - 10, temp, wcslen(temp));

        // 자식 노드와 연결선 그리기
        if (node->left != NIL)
        {
            MoveToEx(hdc, x, y, NULL);
            LineTo(hdc, x - offset, y + 100);
            drawNode(hdc, node->left, x - offset, y + 100, offset / 2);
        }

        if (node->right != NIL)
        {
            MoveToEx(hdc, x, y, NULL);
            LineTo(hdc, x + offset, y + 100);
            drawNode(hdc, node->right, x + offset, y + 100, offset / 2);
        }
    }
};
