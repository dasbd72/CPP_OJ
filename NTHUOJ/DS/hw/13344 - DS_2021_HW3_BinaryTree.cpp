#include <cmath>
#include <iostream>
#include <string>
#include <tuple>
#include <utility>

#define PREORDER 1
#define INORDER 2
#define POSTORDER 3

typedef long long LL;

using namespace std;

template <class T>
class myQueue {
   private:
    /* data */
    T* myArray;
    int rearidx, frontidx;
    int capacity;

    void extendCapacity() {
        int oldcapacity = capacity;
        capacity *= 2;
        T* anotherArray = myArray;
        myArray = new T[capacity];
        for (int i = 0, j = frontidx; j != rearidx; i++, j = (j + 1) % oldcapacity)
            myArray[i] = anotherArray[j];
        frontidx = 0;
        rearidx = oldcapacity - 1;
        delete[] anotherArray;
        return;
    }

   public:
    myQueue(int size = 1) {
        capacity = ((size + 1) / 2) * 2;
        myArray = new T[capacity];
        rearidx = frontidx = 0;
    }
    ~myQueue() {
        delete[] myArray;
    }
    bool empty() const {
        return rearidx == frontidx;
    }
    T& front() const {
        if (this->empty()) throw "Queue is empty. No front.";
        return myArray[frontidx];
    }
    void push(const T& item) {
        if ((rearidx + 1) % capacity == frontidx) this->extendCapacity();
        myArray[rearidx] = item;
        rearidx = (rearidx + 1) % capacity;
        return;
    }
    void pop() {
        if (this->empty()) throw "Queue is empty. Cannot pop.";
        frontidx = (frontidx + 1) % capacity;
    }
};

template <class T>
class myStack {
   private:
    /* data */
    T* myArray;
    int topidx;
    int capacity;

    void extendCapacity() {
        capacity *= 2;
        T* anotherArray = myArray;
        myArray = new T[capacity];
        for (int i = 0; i <= topidx; i++)
            myArray[i] = anotherArray[i];
        delete[] anotherArray;
        return;
    }

   public:
    myStack(int size = 1) {
        capacity = ((size + 1) / 2) * 2;
        myArray = new T[capacity];
        topidx = -1;
    }
    ~myStack() {
        delete[] myArray;
    }
    bool empty() const {
        return topidx == -1;
    }
    T& top() const {
        if (topidx == -1) throw "Stack is empty. No top.";
        return myArray[topidx];
    }
    void push(const T& item) {
        if (topidx == capacity - 1) this->extendCapacity();
        myArray[++topidx] = item;
        return;
    }
    void pop() {
        if (topidx == -1) throw "Stack is empty. Cannot pop.";
        topidx--;
    }
};

class Node {
   public:
    LL data;
    bool isProtected;
    bool isTower;
    LL kindomval;
    Node* left;
    Node* right;
    Node(LL data = 0) : data(data) {
        left = right = nullptr;
    }
    bool isleaf() const {
        return (left == nullptr && right == nullptr);
    }
};
class Binary_Tree {
   private:
    Node* root;
    bool inverted;

    Node* lchild(Node* node) const {
        return (!inverted ? node->left : node->right);
    }
    Node* rchild(Node* node) const {
        return (!inverted ? node->right : node->left);
    }
    void deltree(Node* node) const {
        if (!node) return;
        deltree(node->left);
        deltree(node->right);
        delete node;
    }
    void rstBinaryTower(Node* node) const {
        if (!node) return;
        node->isProtected = false;
        node->isTower = false;
        rstBinaryTower(node->left);
        rstBinaryTower(node->right);
    }
    LL calcBinaryTower(Node* node) const {
        if (!node) return 0;
        LL sum = 0;
        sum += this->calcBinaryTower(node->left);
        sum += this->calcBinaryTower(node->right);
        if ((node->left && node->left->isTower) || (node->right && node->right->isTower)) node->isProtected = true;
        if ((node->left && !node->left->isProtected) || (node->right && !node->right->isProtected) || (node == root && !node->isProtected)) {
            sum += 1;
            node->isTower = true;
            node->isProtected = true;
        }
        return sum;
    }
    LL calcKindom(Node* node) const {
        if (!node) return 0;
        LL maxKindom = 0, lval = 0, rval = 0;
        maxKindom = max(calcKindom(node->left), calcKindom(node->right));
        node->kindomval = 0;
        if (node->left && node->left->data == node->data) lval = node->left->kindomval + 1;
        if (node->right && node->right->data == node->data) rval = node->right->kindomval + 1;
        maxKindom = max(maxKindom, lval + rval);
        node->kindomval = max(lval, rval);
        return maxKindom;
    }

   public:
    Binary_Tree() {
        root = nullptr;
        inverted = false;
    }
    ~Binary_Tree() {
        deltree(root);
    }
    void Construct(string exp) {
        myStack<tuple<int, Node*, int>> expSt;  // start, parent, direction
        myStack<int> parSt;                     // position
        int* pos = new int[exp.length()];       // next position
        int numl, numr;
        int start, direction;
        Node *parent, *newnode;

        for (int i = exp.length() - 1; i >= 0; i--) {  // Locate parenthesis pair
            pos[i] = 0;
            if (exp[i] == '(') {
                pos[i] = parSt.top();
                parSt.pop();
            } else if (exp[i] == ')')
                parSt.push(i);
        }
        expSt.push(make_tuple(0, nullptr, 0));
        while (!expSt.empty()) {
            start = get<0>(expSt.top());
            parent = get<1>(expSt.top());
            direction = get<2>(expSt.top());
            expSt.pop();
            if (pos[start] == start + 1) continue;

            numl = numr = start + 1;
            while (isdigit(exp[numr]) || exp[numr] == '-') numr++;
            newnode = new Node(stoll(exp.substr(numl, numr - numl)));

            if (parent == nullptr)
                root = parent = newnode;
            else if (direction == 0)
                parent = parent->left = newnode;
            else if (direction == 1)
                parent = parent->right = newnode;

            expSt.push(make_tuple(numr, parent, 0));           // left subtree
            expSt.push(make_tuple(pos[numr] + 1, parent, 1));  // right subtree
        }
        delete[] pos;
    }
    void Traverse(int order) const {
        bool output;
        Node* curnode;
        myStack<tuple<bool, Node*>> st;  // operation, node
        st.push(make_tuple(false, root));
        while (!st.empty()) {
            output = get<0>(st.top());
            curnode = get<1>(st.top());
            st.pop();
            if (curnode == nullptr) continue;
            if (output)
                cout << curnode->data << " ";
            else {
                if (order == PREORDER) {
                    st.push(make_tuple(false, rchild(curnode)));
                    st.push(make_tuple(false, lchild(curnode)));
                    st.push(make_tuple(true, curnode));
                } else if (order == INORDER) {
                    st.push(make_tuple(false, rchild(curnode)));
                    st.push(make_tuple(true, curnode));
                    st.push(make_tuple(false, lchild(curnode)));
                } else if (order == POSTORDER) {
                    st.push(make_tuple(true, curnode));
                    st.push(make_tuple(false, rchild(curnode)));
                    st.push(make_tuple(false, lchild(curnode)));
                }
            }
        }
        cout << '\n';
    }
    LL WeightSum() const {
        LL sum = 0;
        Node* curnode;
        myStack<Node*> st;  // operation, node
        st.push(root);
        while (!st.empty()) {
            curnode = st.top();
            st.pop();
            if (!curnode) continue;
            st.push(curnode->right);
            st.push(curnode->left);
            sum += curnode->data;
        }
        return sum;
    }
    LL MaximumPathSum() const {
        if (root == nullptr) return 0;
        myQueue<tuple<LL, Node*>> que;  // Sum, Node
        LL maxsum = -0x7fffffffffffffff, sum;
        Node* node;
        que.push(make_tuple(0, root));
        while (!que.empty()) {
            sum = get<0>(que.front());
            node = get<1>(que.front());
            que.pop();
            if (!node) continue;
            sum += node->data;
            if (node->isleaf()) maxsum = max(sum, maxsum);
            que.push(make_tuple(sum, node->left));
            que.push(make_tuple(sum, node->right));
        }
        return maxsum;
    }
    LL BinaryTower() const {
        this->rstBinaryTower(root);
        return this->calcBinaryTower(this->root);
    }
    bool Foldable() const {
        if (root == nullptr) return true;
        Node *lnode, *rnode;
        myQueue<tuple<Node*, Node*>> que;  // Left Right
        que.push(make_tuple(root->left, root->right));
        while (!que.empty()) {
            lnode = get<0>(que.front());
            rnode = get<1>(que.front());
            que.pop();
            if ((lnode == nullptr) ^ (rnode == nullptr)) return false;
            if (lnode && rnode) {
                que.push(make_tuple(lnode->left, rnode->right));
                que.push(make_tuple(lnode->right, rnode->left));
            }
        }
        return true;
    }
    void DeleteLeaf() {
        if (root == nullptr) return;
        if (root->isleaf()) {
            delete root;
            root = nullptr;
            return;
        }
        Node* curnode;
        myStack<Node*> st;  // node
        st.push(root);
        while (!st.empty()) {
            curnode = st.top();
            st.pop();
            if (curnode->left != nullptr) {
                if (curnode->left->isleaf()) {
                    delete curnode->left;
                    curnode->left = nullptr;
                } else
                    st.push(curnode->left);
            }
            if (curnode->right != nullptr) {
                if (curnode->right->isleaf()) {
                    delete curnode->right;
                    curnode->right = nullptr;
                } else
                    st.push(curnode->right);
            }
        }
    }
    void Invert() {
        inverted = !inverted;
    }
    LL KingdonUnitePath() const {
        return (this->calcKindom(root));
    }
};

int main() {
    string exp, op;
    while (cin >> exp) {
        Binary_Tree bt;
        bt.Construct(exp);
        while (cin >> op) {
            if (op == "End") {
                cout << "\n";
                break;
            }
            if (op == "Traverse") {
                bt.Traverse(PREORDER);
                bt.Traverse(INORDER);
                bt.Traverse(POSTORDER);
            } else if (op == "WeightSum") {
                cout << bt.WeightSum() << '\n';
            } else if (op == "MaximumPathSum") {
                cout << bt.MaximumPathSum() << '\n';
            } else if (op == "BinaryTower") {
                cout << bt.BinaryTower() << '\n';
            } else if (op == "DeleteLeaf") {
                bt.DeleteLeaf();
            } else if (op == "Foldable") {
                cout << (bt.Foldable() ? "Yes" : "No") << '\n';
            } else if (op == "Invert") {
                bt.Invert();
            } else if (op == "KingdomUnitedPath") {
                cout << bt.KingdonUnitePath() << '\n';
            }
        }
    }
    return 0;
}
