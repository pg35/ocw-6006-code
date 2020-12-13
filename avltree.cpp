//g++  7.4.0
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include<cstdlib>

using namespace std;
#define PRINT_MAP(mp) for(auto a: mp) cout<<a.first<<" -> ("<<a.second.first<<", "<<a.second.second<<")\n";

typedef vector<int> vi;
typedef pair<int,int> PairInt;
typedef pair<int, PairInt> PairNode;
typedef map<int, PairInt> MapN2C;
typedef vector<PairNode> VecNode;

//same node as BST with addtional parent pointer and template
template <typename T>
class AVLNode{
    public:
        T value;
        AVLNode<T> *parent, *left, *right;
        int height;
        AVLNode(T v, AVLNode<T> *p=0):value(v),parent(p),left(0),right(0),height(0){}
};
template<typename T>
class AVLTree{
    public:
        AVLNode<T> *root;
        AVLTree():root(0){}
        void insert(T v){
            if (!root) root=new AVLNode<T>(v);
            else {
                auto n = root;
                while(n){
                    if (v<=n->value){
                        if (n->left) n=n->left;
                        else break;
                    }
                    else{
                        if (n->right) n=n->right;
                        else break;
                    }
                }
                auto t=new AVLNode<T>(v,n);
                if (v<=n->value) n->left = t;
                else n->right = t;
                balance(t);
            }
        }
        void print(){
            if (!root) cout<<"AVLTree is empty\n";
            else print_preorder(root);
        }
        void print(MapN2C &mp){
            if (!root) return;
            else print_preorder(root, mp);
        }
        void print_preorder(auto n){
            if (!n) return;
            cout<<n->value<<" -> "<<(n->left? n->left->value: 0)<<", "<<(n->right? n->right->value:0)<<endl;
            print_preorder(n->left);
            print_preorder(n->right);
        }
        void print_preorder(auto n, MapN2C &mp){
            mp[n->value] = PairInt(n->left ? n->left->value : 0, n->right ? n->right->value : 0);
            if (n->left) print_preorder(n->left, mp);
            if (n->right) print_preorder(n->right, mp);
        }
        void balance(auto n){
            char buf[2]={};
            auto c = n;
            n = n->parent;
            while(n){
                buf[1] = buf[0];
                buf[0] = n->left == c ? 'L': 'R';
                int lh = ( n->left ? n->left->height : -1 ) + 1;
                int rh = ( n->right? n->right->height: -1 ) + 1;
                n->height = max(lh, rh);
                if (abs(lh-rh) > 1){
                    //cout<<"rototation type = "<<buf<<endl;
                    if (string(buf) == "LL") rotateLL(n);
                    else if (string(buf) == "RR") rotateRR(n);
                    else if (string(buf) == "LR") rotateLR(n);
                    else if (string(buf) == "RL") rotateRL(n);
                    else throw "Invalid rototation type";
                    
                }
                c = n;
                n = n->parent;
            }
        }
        void rotateLL(auto n){
            auto lc = n->left;
            adjustParent(n,lc);
            n->left = lc->right;
            lc->right = n;
            recalc_height(vector<AVLNode<T> *>{n,lc});
        }
        void rotateRR(auto n){
            auto rc = n->right;
            adjustParent(n,rc);
            n->right = rc->left;
            rc->left = n;
            recalc_height(vector<AVLNode<T> *>{n,rc});
        }
        void rotateLR(auto n){
            auto lc = n->left;
            auto lcr = lc->right;
            adjustParent(n,lcr);
            lc->right = lcr->left;
            n->left = lcr->right;
            lcr->left = lc;
            lcr->right = n;
            recalc_height(vector<AVLNode<T> *>{lcr->left, lcr->right, lcr});
        }
        void rotateRL(auto n){
            auto rc = n->right;
            auto rcl = rc->left;
            adjustParent(n, rcl);
            rc->left = rcl->right;
            n->right = rcl->left;
            rcl->left = n;
            rcl->right = rc;
            //same as calling it as (n,rc,rcl);
            recalc_height(vector<AVLNode<T> *>{rcl->left, rcl->right, rcl});
        }
        void adjustParent(auto n, auto c){
            if (!n->parent) root=c;
            else if (isLeftChild(n,n->parent)){
                n->parent->left = c; 
            }
            else{
                n->parent->right=c;
            }
        }
        bool isLeftChild(auto n, auto p){
            return n==n->parent->left;
        }
        //order of nodes is important
        void recalc_height(auto nodes){
            for(auto n:nodes){
                n->height = max(n->left?n->left->height:0,n->right?n->right->height:0)+1;
            }
        }

};

void testAVL(vi &&data, MapN2C &&expected, string desc){
    AVLTree<int> tree;
    for(auto n: data) tree.insert(n);
    MapN2C mp;
    tree.print(mp);
    cout<<(mp==expected ? "PASS" : "FAIL")<<" = "<<desc<<endl;
    if ( mp!=expected) {
        PRINT_MAP(mp);
        cout<<"---\n";
        PRINT_MAP(expected);
        cout<<endl;
        cout<<"===\n";
    }
}
//vn is a vector where each item is a pair(v, pair(lcv, rch)). 
//output is a map where each item is a node (v -> pair(lcv,rcv)).
//going to do vec 2 map conversion b/c I want to use initializer list to write expected value for testDeleteNode().
MapN2C v2map(VecNode &&vn){
    MapN2C mp;
    copy(vn.begin(),vn.end(),inserter(mp, mp.begin()));
    return mp;
}
int main()
{
    testAVL(vector<int>{30,20,10}, v2map(VecNode{ {20,PairInt(10,30)}, {10,PairInt(0,0)}, {30,PairInt(0,0)} }),"LL, root");
    testAVL(vector<int>{10,20,30}, v2map(VecNode{ {20,PairInt(10,30)}, {10,PairInt(0,0)}, {30,PairInt(0,0)} }),"RR, root");
    testAVL(vector<int>{30,10,20}, v2map(VecNode{ {20,PairInt(10,30)}, {10,PairInt(0,0)}, {30,PairInt(0,0)} }),"LR, root");
    testAVL(vector<int>{10,30,20}, v2map(VecNode{ {20,PairInt(10,30)}, {10,PairInt(0,0)}, {30,PairInt(0,0)} }),"RL, root");

    //LL,LR,LR
    testAVL(vector<int>{30,20,40,10,25,5}, v2map(VecNode{ {20,PairInt(10,30)}, {10,PairInt(5,0)}, {5,PairInt(0,0)}, {30,PairInt(25,40)}, {25,PairInt(0,0)}, {40,PairInt(0,0)} }), "LL, root, new parent (20) had right child (25)");
    testAVL(vector<int>{30,20,40,10,25,22}, v2map(VecNode{ {25,PairInt(20,30)}, {20,PairInt(10,22)}, {10,PairInt(0,0)}, {22,PairInt(0,0)}, {30,PairInt(0,40)}, {40,PairInt(0,0)} }), "LR, root, new parent (25) had left child (22)");
    testAVL(vector<int>{30,20,40,10,25,28}, v2map(VecNode{ {25,PairInt(20,30)}, {20,PairInt(10,0)}, {10,PairInt(0,0)}, {30,PairInt(28,40)}, {28,PairInt(0,0)}, {40,PairInt(0,0)} }), "LR, root, new parent (25) had right child (28)");
    //RR
    testAVL(vector<int>{20,10,30,25,40,50}, v2map(VecNode{ {30,PairInt(20,40)}, {20,PairInt(10,25)}, {10,PairInt(0,0)}, {25,PairInt(0,0)}, {40,PairInt(0,50)}, {50,PairInt(0,0)} }), "RR, root, new parent (30) had left child (25)");
}
