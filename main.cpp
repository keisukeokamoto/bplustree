#include "BpTree.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(int argc, const char * argv[]) {
    
    BpTree bt = BpTree(4);
    
    bt.insert(5, "five");
    bt.insert(3, "three");
    bt.insert(4, "four");
   
    bt.remove(7);
    
    bt.printKeys();
    bt.printValues();
     bt.printKeys();
    
    return 0;
}
