//==========================
//AVL TREE
//==========================
//same node as BST with addtional parent pointer and template
template <typename T>
class AVLNode{
    public:
        AVLNode<T> *parent, *left, *right;
        T value;
        int height;
        AVLNode(T v, AVLNode<T> *p=0):value(v),parent(p),left(0),right(0),height(0){}
};
template<typename T>
class AVLTree{
    public:
        AVLNoode<T> *root;
        AVLTree():root(0){}
        void insert(T v){
            if (!root) root=new AVLNode(v);
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
                auto t=new AVLNode(v,n);
                if (v<=n->value) n->left = t;
                else n->right = t;
                balance(t);
            }
        }
        void print(){
            if (!root) cout<<"AVLTree is empty\n";
            else print_preorder(root);
        }
        void print_preorder(auto n){
            if (!n) return;
            cout<<n->value<<" -> "<<(n->left? n->left->value)<<", "<<(n->right? n->right->value)<<endl;
            print_preorder(n->left);
            print_preorder(n->right);
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
                    switch(string(buf)){
                        case 'LL':
                            rotateLL(n);
                            break;
                        case 'RR':
                            rotateRR(n);
                            break;
                        case 'LR':
                            rotateLR(n);
                            break;
                        case 'RL':
                            rotateRL(n);
                            break;
                        default:
                            throw new Excpetion('Invalid rototation type');
                    }
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
            recalc_height(vector<AVLNode *>{n,lc});
        }
        void rotateRR(auto n){
            auto rc = n->right;
            adjustParent(n,rc);
            n->right = rc->left;
            rc->left = n;
            recalc_height(vector<AVLNode *>{n,rc});
        }
        void rotateLR(auto n){
            auto lc = n->left;
            auto lcr = lc->right;
            adjustParent(n,lcr);
            lc->right = lcr->left;
            n->left = lcr->right;
            lcr-left = lc;
            lcr->right = n;
            recalc_height(vector<AVLNode *>{lcr->left, lcr->right, lcr});
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
            recalc_height(vector<AVLNode *>{rcl->left, rcl->right, rcl});
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
        void isLeftChild(auto n, auto p){
            return n==n->parent->left;
        }
        //order of nodes is important
        void recalc_height(auto nodes){
            for(auto n:nodes){
                n->height = max(n->left?n->left->height:0,n->right?n->right->height:0)+1;
            }
        }

};