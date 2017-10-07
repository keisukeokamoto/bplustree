//
//CMPT454 Project
//Name: Keisuke Okamoto
//
//
//

#include "BpTree.h"

BpTree::BpTree(void)
{
    root = NULL;
    num = 0;
}

BpTree::BpTree(const BpTree & bt)
{
    root = bt.root;
    num = bt.num;
    flo = bt.flo;
    cei = bt.cei;
}

BpTree::BpTree(int n)
{
    if(n <= 1){
        cout << "The node size must be bigger than 1" << endl;
        exit(0);
    }
    root = NULL;
    num = n;
    flo = floor((double)num/2);
    cei = ceil((double)num/2);
}

BpTree::BpTree(Node* r, int n, int f, int c)
{
    root = r;
    num = n;
    flo = f;
    cei = c;
}

BpTree::~BpTree(void)
{
    freeNode();
}

BpTree::Node::Node(int x)
{
    isLeaf = false;
    nkey = 0;
    key = new int[x];
    data = new string[x];
    child = new Node*[x+1];
    next = NULL;
    parent = NULL;
    
    for(int i = 0; i < x; i++){
        key[i] = 0;
        data[i] = "";
    }
    
    for(int i = 0; i<=x; i++){
        child[i] = NULL;
    }
}

BpTree::Node::Node(bool _isLeaf,int* _key, string* _data, Node** _child, int _nkey, Node* _next, Node* _parent)
{
    isLeaf = _isLeaf;
    nkey = _nkey;
    key = new int[nkey];
    data = new string[nkey];
    child = new Node*[nkey+1];
    next = _next;
    parent = _parent;
    
    for(int i = 0; i < nkey; i++){
        key[i] = _key[i];
        data[i] = _data[i];
    }
    
    for(int i = 0; i<=nkey; i++){
        child[i] = _child[i];
    }

}

BpTree::Node::~Node(void)
{
    delete []key;
    delete []data;
    delete []child;
}

const BpTree &BpTree::operator=(const BpTree& bt)
{
    root = bt.root;
    num = bt.num;
    flo = bt.flo;
    cei = bt.cei;
    
    return bt;
}

string BpTree::find(int k) const
{
    Node* temp = root;
    if(temp != NULL){
        //from root, it finds down a node which contain k
        while(temp->isLeaf == false){
            for(int i = 0; i < temp->nkey; i++){
                int t = temp->key[i];
                int u = temp->key[i+1];
                if(t > k){
                    temp = temp->child[i];
                    break;
                }else if(t == k || u > k){
                    temp = temp->child[i+1];
                    break;
                }else{
                    if(i == temp->nkey-1){
                        temp = temp->child[i+1];
                        break;
                    }
                }
            }
        }
        
        //after it finds a node it will find a key
        for(int j = 0; j < temp->nkey; j++){
            if(temp->key[j] == k){
                return temp->data[j];
            }
        }
    }
    return "key not found \n" ;
}

void BpTree::printKeys() const
{
    if(root== NULL || root->nkey ==0){
        cout << "Tree does not have any key" <<endl;
        return;
    }
    
    //do breath first search using queue
    //lev is the level of the node (use for line break)
    queue <Node*> qu;
    queue <int> lev;
    qu.empty();
    lev.empty();
    qu.push(root);
    lev.push(findLevel(root));
    int prev = 1;//save the previous level of the node to compare
    
    while(qu.size() != 0){
        Node* node = qu.front();
        qu.pop();
        
        //line break if the level of tree changes
        if(lev.front() != prev){
            cout << endl;
        }
        prev = lev.front();
        lev.pop();
        
        cout << " [" ;
        for(int i = 0; i < node->nkey;i++){
            cout << node->key[i];
            if(i != node->nkey-1){
                cout << ", ";
            }
        }
        cout << "] " ;
        
        for(int i =0; i <= node->nkey;i++){
            if(node->child[i] != NULL){
                qu.push(node->child[i]);
                lev.push(findLevel(node->child[i]));
            }
        }
    }
    cout << endl << endl;

}

void BpTree::printValues() const
{
    if(root== NULL || root->nkey ==0){
        cout << "Tree does not have any value" <<endl;
        return;
    }
    
    Node* temp = root->child[0];
    //from root, go down to most left leaf
    if(temp != NULL){
        while(temp->isLeaf == false){
            temp = temp->child[0];
        }
    }else{
    //root is leaf
        for(int i = 0; i < root->nkey; i++){
            cout << root->data[i] << endl ;
        }
        return;
    }
    
    //prints key and goes next leaf
    while(temp != NULL){
        for(int i = 0; i < temp->nkey; i++){
            cout << temp->data[i] << " " << endl ;
        }
        temp = temp->next;
    }
}

void BpTree::insert(int k, string d)
{
    if(k <= 0){
        cout << "the key must be greater than 0" <<endl;
        return;
    }
    
    if(d == ""){
        cout << "the value cannot be empty" << endl;
        return;
    }
    
    if(search(k)){
        cout << "key " << k << " already exists" << endl;
        return;
    }else{
        if(root == NULL){
        //create first node (root)
            root = new Node(num);
            root->isLeaf = true;
            root->key[0] = k;
            root->data[0] = d;
            root->nkey++;
        }else if(root->isLeaf == true){
            if(insertWithinNode(k,d,root)==false){
            //key is inserted to root
                return;
            }else{
            //when tree contains only root
            //root is splitted
            //create new root above
                Node* tempNew = new Node(num);
                tempNew->child[0] = root;
                tempNew->child[0]->parent = tempNew;
                tempNew->child[1] = root->next;
                tempNew->child[1]->parent = tempNew;
                tempNew->key[0] = tempNew->child[1]->key[0];
                tempNew->nkey++;
                root = tempNew;
                return;
            }
        }else{
        //when there is more than 2 levels
            Node * temp = findNode(k);
            if(insertWithinNode(k,d,temp) == false){
            //key inserted
                return;
            }else{
            //leaf splitted
                if(fixPointer(temp) == false){
                //root did not split
                    return;
                }else{
                //root splitted
                //add one level
                    Node* tempNew = new Node(num);
                    tempNew->child[0] = root;
                    tempNew->child[0]->parent = tempNew;
                    tempNew->child[1] = root->next;
                    tempNew->child[1]->parent = tempNew;
                    tempNew->key[0] = findSuc(tempNew->child[1]);
                    tempNew->nkey++;
                    root = tempNew;
                    return;
                }
            }
        }
    
    }

}

void BpTree::remove(int k)
{
    if(!search(k)){
        cout << "key " << k << " does not exists" << endl;
        return;
    }else{
        Node* temp = findNode(k);
        int deletedIndex = -1;
        //delete key
        for(int i = 0; i < temp->nkey; i++){
            if(temp->key[i] == k){
                deletedIndex = i; //save which index of key deleted
                //delete the key and shift key to right by one
                if(i != temp->nkey-1){
                    for(int j = i; j < temp->nkey-1; j++){
                        temp->key[j] = temp->key[j+1];
                        temp->data[j] = temp->data[j+1];
                    }
                }
                //empty the last key
                temp->key[temp->nkey-1] = 0;
                temp->data[temp->nkey-1] = "";
                temp->nkey--;
                break;
            }
        }
        
        //when the tree contains only root
        if(temp == root){
            return;
        }
        
        //if the first key of the node is deleted fix the keys
        if(deletedIndex == 0){
            fixKey(temp);
        }
        
        //if node has enough key
        if(temp->nkey >= cei){
            return;
        }else{
            //Take key from left sibling
            Node* left = findLeftSibling(temp);
            if(left != NULL && left->nkey > cei){
                takeKeyFromLeftLeaf(temp, left);
                return;
            }
            
            //Take key from right sibling
            Node* right = findRightSibling(temp);
            if(right != NULL && right->nkey > cei){
                takeKeyFromRightLeaf(temp, right);
                return;
            }
            
            //Merge two node
            //Merge with left sibling
            if(left != NULL){
                mergeWithLeftLeaf(temp,left);
                temp = left;
            }else{
            //Merge with right sibling
                mergeWithRightLeaf(temp, right);
            }
            
            if(fixParent(temp) == false){
            //root has enough keys
                return;
            }else{
            //root has only one pointer
            //remove the root and make its child as root
                root = root->child[0];
                return;
            }
        }
    }
    
}

void BpTree::freeNode()
{
    //do breath first travarsal of node and delete them
    queue <Node*> qu;
    qu.empty();
    qu.push(root);
    
    while(qu.size() != 0){
        Node* node = qu.front();
        
        qu.pop();
        
        for(int i =0; i <= node->nkey;i++){
            if(node->child[i] != NULL){
                qu.push(node->child[i]);
            }
        }
        delete node;
    }
}

//almost same as find()
bool BpTree::search(int k) const
{
    Node* temp = root;
    if(root != NULL){
        while(temp->isLeaf == false){
            for(int i = 0; i < temp->nkey; i++){
                int t = temp->key[i];
                int u = temp->key[i+1];
                if(t > k){
                    temp = temp->child[i];
                    break;
                }else if(t == k || u > k){
                    temp = temp->child[i+1];
                    break;
                }else{
                    if(i == temp->nkey-1){
                        temp = temp->child[i+1];
                        break;
                    }
                }
            }
        }
    
        for(int j = 0; j < temp->nkey; j++){
            if(temp->key[j] == k){
                return true;
            }
        }
    }
    return false;
}

BpTree::Node* BpTree::findNode(int k) const
{
    Node* temp = root;
    while(temp->isLeaf == false){
        for(int i = 0; i < temp->nkey; i++){
            int t = temp->key[i];
            int u = temp->key[i+1];
            if(t > k){
                temp = temp->child[i];
                break;
            }else if(t == k || u > k){
                temp = temp->child[i+1];
                break;
            }else{
                if(i == temp->nkey-1){
                    temp = temp->child[i+1];
                    break;
                }
            }
        }
    }
    return temp;
}

int BpTree::findSuc(Node* n) const
{
    Node* temp = n;
    while(temp->isLeaf == false){
        temp = temp->child[0];
    }
    return temp->key[0];
}

int BpTree::findLevel(Node* n) const
{
    int level = 1;
    Node* temp = n;
    while(temp != root){
        temp = temp->parent;
        level++;
    }
    return level;
}

bool BpTree::fixPointer(Node* n)
{
    
    Node* temp = n->parent;
    Node* tempChild = n;
    
    while(true){
        int p = 0; //p is the position of new node in the parent node
        for(int i = 0; i < temp->nkey + 1 ; i++){
            if(tempChild == temp->child[i]){
                p = i + 1;
                break;
            }
        }
        
        if(temp->nkey <= num){
        //when the parent is not full
        //shift key by one to right until p from back
            int j = temp->nkey;
            while(j>p){
                temp->child[j] = temp->child[j-1];
                temp->key[j-1] = findSuc(temp->child[j]);
                j--;
            }
            temp->child[p] = temp->next;
            temp->child[p]->parent = temp;
            temp->key[p-1] = findSuc(temp->child[p]);
            temp->next = NULL;
            return false;
        }else{
        //when node cannot get a new key anymore, split node
            splitInternalNode(temp,p);
            if(temp == root){
            //return true if the root if the root is splitted
                return true;
            }else{
                //repeat until pointers fits in parent node
                tempChild = temp;
                temp = temp->parent;
            }
        }
    }
    return true;
}

bool BpTree::insertWithinNode(int k,string d, Node* n)
{
    Node *temp = n;
    bool isSplit = false;
    
    //if the node cannot contain any more keys
    if(n->nkey == num){
        int p = num; //p is the position of new node in the parent node
        for(int i = 0; i < temp->nkey; i++){
            if(k < temp->key[i]){
                p = i;
                break;
            }
        }
        //split leaf node
        splitLeafNode(n,p);
        isSplit = true;
        if(p <= flo){
        //if new node is left side
            temp = n;
        }else{
        //if new node is right side
            temp = n->next;
        }
    }
    //insert new key and data to the node
    int i = temp->nkey-1;
    while(i >= 0 && temp->key[i] > k){
        temp->key[i+1] = temp->key[i];
        temp->data[i+1] = temp->data[i];
        i--;
    }
    temp->key[i+1] = k;
    temp->data[i+1] = d;
    temp->nkey++;
    
    //return true if the node is splitted or not
    return isSplit;
}

void BpTree::splitLeafNode(Node* n, int p)
{
    Node* right = new Node(num);
    if(p <= flo){
    //the new key is left side
        for(int i = 0; i < cei; i++){
        //copy the key and data to left node
            right->key[i] = n->key[flo + i];
            n->key[flo + i] = 0;
            right->data[i] = n->data[flo + i];
            n->data[flo + i] = "";
            right->nkey++;
            n->nkey--;
        }
    }else{
    //the new key is right side
        for(int i = 1; i < cei; i++){
        //copy the new key to right node
            right->key[i-1] = n->key[flo + i];
            n->key[flo + i] = 0;
            right->data[i-1] = n->data[flo + i];
            n->data[flo + i] = "";
            right->nkey++;
            n->nkey--;
        }
    }
    right->next = n->next;
    right->isLeaf = true;
    n->next = right;
    
    //the new node parent's next point right temporary
    if(n->parent != NULL){
        n->parent->next = right;
        right->parent = n->parent;
        right->parent->nkey++;
    }
    
}

void BpTree::splitInternalNode(Node* n, int p)
{
    Node* right = new Node(num);
    Node* temp = n;
    if(p <= cei){
    //then new node goes left node
        right->child[0] = temp->child[cei];
        right->child[0]->parent = right;
        for(int i = 1; i < flo+1; i++){
        //copy the new key to right node
            right->child[i] = temp->child[i+cei];
            right->child[i]->parent = right;
            temp->child[i+cei] = NULL;
            right->key[i-1] = findSuc(right->child[i]);
            temp->key[i+cei-1] = 0;
            right->nkey++;
            temp->nkey--;
        }
        
        int j = temp->nkey-1;
        //shift key and child to right
        while(j>p){
            temp->child[j] = temp->child[j-1];
            temp->child[j]->parent = temp;
            temp->key[j-1] = findSuc(temp->child[j]);
            j--;
        }
        
        temp->nkey--;
        temp->child[p] = temp->next;
        temp->key[p-1] = findSuc(temp->child[p]);
        temp->next = NULL;
    }else{
    //when new node is right side
        int j = num;
        while(j>=p){
        //copy the key and child to right
            right->child[j-cei] = temp->child[j];
            right->child[j-cei]->parent = right;
            temp->child[j] = NULL;
            right->key[j-cei-1] = findSuc(right->child[j-cei]);
            temp->key[j-1] = 0;
            right->nkey++;
            temp->nkey--;
            j--;
        }
        
        //do left side of p as well
        temp->nkey--;
        right->child[p-cei-1] = temp->next;
        right->child[p-cei-1]->parent = right;
        temp->next = NULL;
        
        while(cei < j){
        //copy keys and child
            right->key[j-cei-1] = findSuc(right->child[j-cei]);
            temp->key[j-1] = 0;
            right->child[j-cei-1] = temp->child[j];
            right->child[j-cei-1]->parent = right;
            temp->child[j] = NULL;
            right->nkey++;
            temp->nkey--;
            j--;
        }
    }
    //fix next pointer
    temp->next = right;
   
    //fix the parent node's pointers
    if(temp != root){
        temp->parent->next = right;
        right->parent = temp->parent;
        right->parent->nkey++;
    }
    
}

int BpTree::whichParent(Node* n) const
{
    if(n != root){
        for(int i = 0; i <= n->parent->nkey; i++){
            if(n == n->parent->child[i]){
                return i;
            }
        }
    }
    return -1;
}

BpTree::Node* BpTree::findLeftSibling(Node* n) const
{
    if(n != NULL){
        int in = whichParent(n);
        if(in == 0){
            //return null if it does not have left sibling
            return NULL;
        }else{
            return n->parent->child[in-1];
        }
    }
    return NULL;
}

BpTree::Node* BpTree::findRightSibling(Node* n) const
{
    if(n != NULL){
        int in = whichParent(n);
        if(in == n->parent->nkey){
        //return null if it does not have right sibling
            return NULL;
        }else{
            return n->parent->child[in+1];
        }
    }
    return NULL;
}

void BpTree::takeKeyFromLeftLeaf(Node* n, Node* l)
{
    Node* temp = n;
    Node* left = l;
    
    //shift keys to right by one
    for(int i = temp->nkey; i > 0; i--){
        temp->key[i] = temp->key[i-1];
        temp->data[i] = temp->data[i-1];
    }
    //copy left sibling's most left key to right
    temp->key[0] = left->key[left->nkey-1];
    left->key[left->nkey-1] = 0;
    temp->data[0] = left->data[left->nkey-1];
    left->data[left->nkey-1] = "";
    temp->nkey++;
    left->nkey--;
    
    //fix parent key
    fixKey(temp);
}

void BpTree::takeKeyFromRightLeaf(Node* n, Node* r)
{
    Node* temp = n;
    Node* right = r;
    
    //copy right sibling's most right key to left
    temp->key[temp->nkey] = right->key[0];
    temp->data[temp->nkey] = right->data[0];
    //shift keys to left by one
    for(int i = 0; i < right->nkey-1; i++){
        right->key[i] = right->key[i+1];
        right->data[i] = right->data[i+1];
    }
    right->key[right->nkey-1] = 0;
    right->data[right->nkey-1] = "";
    temp->nkey++;
    right->nkey--;
    
    //fix parent key
    fixKey(right);
}

void BpTree::mergeWithRightLeaf(Node* n, Node* r)
{
    Node* temp = n;
    Node* right = r;
    int p = whichParent(right);
    int tnkey = temp->nkey;
    //move the keys and childs to left
    for(int i = 0; i < right->nkey; i++){
        temp->key[i+tnkey] = right->key[i];
        temp->data[i+tnkey] = right->data[i];
        temp->nkey++;
    }
    temp->next = right->next;
    
    //delete the right node
    delete right;
    
    //fix parent pointer
    int i;
    for(i = p; i < temp->parent->nkey; i++){
        temp->parent->child[i] = temp->parent->child[i+1];
        temp->parent->key[i-1] = temp->parent->key[i];
    }
    
    temp->parent->child[i] = NULL;
    temp->parent->key[i-1] = 0;
    temp->parent->nkey--;
}

void BpTree::mergeWithLeftLeaf(Node* n, Node* l)
{
    Node* temp = n;
    Node* left = l;
    int p = whichParent(temp);
    int lnkey = left->nkey;
    //move the keys and childs to left
    for(int i = 0; i < temp->nkey; i++){
        left->key[i+lnkey] = temp->key[i];
        left->data[i+lnkey] = temp->data[i];
        left->nkey++;
    }
    left->next = temp->next;
    
    //delete right node
    delete temp;
    
    //fix parent pointer
    int i;
    for(i = p; i < left->parent->nkey; i++){
        left->parent->child[i] = left->parent->child[i+1];
        left->parent->key[i-1] = left->parent->key[i];
    }
    
    left->parent->child[i] = NULL;
    left->parent->key[i-1] = 0;
    left->parent->nkey--;
}

bool BpTree::fixParent(Node* n)
{
    Node* temp = n->parent;
    //repeat until parent node has enough keys or reach to root
    while(true){
        if(temp == root){
            if(temp->nkey == 0){
                //return true if root has no key
                return true;
            }else{
                //return false if root has more than 1 key
                return false;
            }
        }
        
        if(temp->nkey >= flo){
        //node has enough keys
            return false;
        }
        
        //if the node can take a key from left sibling
        Node* left = findLeftSibling(temp);
        if(left != NULL && left->nkey > flo){
            takeKeyFromLeftInternal(temp, left);
            return false;
        }
        
        //if the right can take a key from right sibling
        Node* right = findRightSibling(temp);
        if(right != NULL && right->nkey > flo){
            takeKeyFromRightInternal(temp, right);
            return false;
        }
    
        //it will either merge with left or right node
        if(left != NULL){
            mergeWithLeftInternal(temp, left);
            temp = left;
        }else{
            mergeWithRightInternal(temp, right);
        }

        temp = temp->parent;
    }
}

void BpTree::takeKeyFromLeftInternal(Node* n, Node* l)
{
    Node* temp = n;
    Node* left = l;
    
    //copy the key and child from left
    for(int i = temp->nkey; i >= 0; i--){
        temp->child[i+1] = temp->child[i];
        temp->key[i] = findSuc(temp->child[i+1]);
    }
    temp->child[0] = left->child[left->nkey];
    temp->child[0]->parent = temp;
    left->child[left->nkey] = NULL;
    left->key[left->nkey-1] = 0;
    temp->nkey++;
    left->nkey--;
    
    //fix the parents pointers
    fixKey(temp);
}

void BpTree::takeKeyFromRightInternal(Node* n, Node* r)
{
    Node* temp = n;
    Node* right = r;
    
    //copy the key and child from right
    temp->child[temp->nkey+1] = right->child[0];
    temp->child[temp->nkey+1]->parent = temp;
    temp->key[temp->nkey] = findSuc(temp->child[temp->nkey+1]);
    
    right->child[0] = right->child[1];
    for(int i = 1; i < right->nkey; i++){
        right->child[i] = right->child[i+1];
        right->key[i-1] = findSuc(right->child[i]);
    }
    right->child[right->nkey] = NULL;
    right->key[right->nkey-1] = 0;
    temp->nkey++;
    right->nkey--;
    
    //fix the parents pointers
    fixKey(right);
}

void BpTree::mergeWithRightInternal(Node* n, Node* r)
{
    Node* temp = n;
    Node* right = r;
    int p = whichParent(right);
    int tnkey = temp->nkey;
    //copy right node items to left
    for(int i = 0; i <= right->nkey; i++){
        temp->child[i+tnkey+1] = right->child[i];
        temp->child[i+tnkey+1]->parent = temp;
        temp->key[i+tnkey] = findSuc(temp->child[i+tnkey+1]);
        temp->nkey++;
    }
    
    //delete right node
    delete right;
    
    //fix parent pointer
    int i;
    for(i = p; i < temp->parent->nkey; i++){
        temp->parent->child[i] = temp->parent->child[i+1];
        temp->parent->key[i-1] = temp->parent->key[i];
    }
    
    temp->parent->child[i] = NULL;
    temp->parent->key[i-1] = 0;
    temp->parent->nkey--;
}

void BpTree::mergeWithLeftInternal(Node* n, Node* l)
{
    Node* temp = n;
    Node* left = l;
    int p = whichParent(temp);
    int lnkey = left->nkey;
    //copy right node items to left
    for(int i = 0; i <= temp->nkey; i++){
        left->child[i+lnkey+1] = temp->child[i];
        left->child[i+lnkey+1]->parent = left;
        left->key[i+lnkey] = findSuc(left->child[i+lnkey+1]);
        left->nkey++;
    }
    //delete right node
    delete temp;
    
    //fix parent pointer
    int i;
    for(i = p; i < left->parent->nkey; i++){
        left->parent->child[i] = left->parent->child[i+1];
        left->parent->key[i-1] = left->parent->key[i];
    }
    
    left->parent->child[i] = NULL;
    left->parent->key[i-1] = 0;
    left->parent->nkey--;
}

void BpTree::fixKey(Node* n)
{
    Node* temp = n;
    bool ok = false;
    while(ok == false){
    //fix up keys until root
        int p = whichParent(temp);
        if(p > 0){
            temp->parent->key[p-1] = findSuc(temp->parent->child[p]);
        }
        temp = temp->parent;
        if(temp == root){
            ok = true;
        }
    }
    
}
