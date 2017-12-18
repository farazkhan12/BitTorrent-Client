//  Created by Faraz Khan on 12/14/17.
//  Copyright © 2017 Faraz Khan. All rights reserved.
//  ECE 2574
//  Professor Gerdes
#include <iostream>
#include <string>
#include <cmath>
#include <math.h>
#include <tgmath.h>

#include "merkle_tree.hpp"
using std::cout;
using std::endl;
//PRIVATE FUNCTION --------------------------------------------------------------

// note: the following provide indices into mktree (i.e., absolute index of node and not with respect to blockID)

template <typename T>
void MerkleTree<T>::pad()
{
    size_t v = treeSize;
    //treeSize = (v*2) -1; //update tree size
    std::vector<unsigned char> stuff; //0 pads
    for(int w = 0; w < 32; w++){
        stuff.push_back(0);
    }
    Hash<T> padded;
    //cout << padded << endl;
    padded.setHash(stuff); //num of pad blocks needed
    for(int i = 0; i < numPads; i++){
        int x = block2ind(i); //index of that specified block
        mktree[x] = padded;
    }
}
template <typename T>
size_t MerkleTree<T>::getLeftChild(size_t parentNode) //left child of parent node
{
    size_t left;
    
    left = parentNode*2 + 1;
    
    return left;
}

template <typename T>
size_t MerkleTree<T>::getRightChild(size_t parentNode) //right child of parent node
{
    size_t right;
    right = getLeftChild(parentNode) + 1;
    
    return right;
}

template <typename T>
size_t MerkleTree<T>::getParent(size_t childNode) //parent node
{
    size_t parent;
    parent = (childNode - 1 )/2;
    return parent;
}

template <typename T>
size_t MerkleTree<T>::getSibling(size_t childNode) //sister node
{
    size_t sibling;
    if(childNode % 2 == 0){ //even
        sibling = childNode -1;
    }
    else{ //odd
        sibling = childNode + 1;
    }
    
    return sibling;
}

template <typename T>
size_t MerkleTree<T>::getAunt(size_t childNode)  //parent's sibling
{
    size_t aunt = getSibling(getParent(childNode));
    if(childNode == 1 || childNode == 2){
        aunt = 0;
    }
    return aunt;
}

template <typename T>
size_t MerkleTree<T>::block2ind(size_t blockID)
{
    return (blockID + (treeSize/2)); //returns index of block
}

template <typename T>
void MerkleTree<T>::updateTree(size_t blockID)
{
    //assume block has been added
    size_t bIdx = block2ind(blockID);
    size_t parentIndex = getParent(bIdx);
    while(parentIndex != 0){
        //rehash parent index with left and right childs from new block
        Hash<T> leftie = mktree[getLeftChild(parentIndex)]; //left child hash
        Hash<T> rightie = mktree[getRightChild(parentIndex)]; //right child hash
        if(!(leftie.isEmpty() || rightie.isEmpty())){
            mktree[parentIndex] = mktree[getLeftChild(parentIndex)] + mktree[getRightChild(parentIndex)];
        }
        
        parentIndex = getParent(parentIndex); //get next parent
    }
    Hash<T> leftie1 = mktree[getLeftChild(parentIndex)]; //left child hash
    Hash<T> rightie1 = mktree[getRightChild(parentIndex)]; //right child hash
    if(!(leftie1.isEmpty() || rightie1.isEmpty())){
        mktree[parentIndex] = mktree[getLeftChild(parentIndex)] + mktree[getRightChild(parentIndex)];
    }
    
    
}
//PRIVATE FUNCTION --------------------------------------------------------------

// ------------------------------------------------------------------------------
// Constructor: default...shouldn't ever be called directly by user
template <typename T>
MerkleTree<T>::MerkleTree()
{
    mktree = nullptr;
    numBlocks = 0;
    numPads = 0;
    levels = 0;
    treeSize = 0;
    //mktree = new Hash<T>[treeSize];
}

// Constructor: empty tree without root hash large enough to accomodate n blocks
template <typename T>
MerkleTree<T>::MerkleTree(size_t n)
{
    //treeSize = n*2 -1;
    unsigned int v = n; // compute the next highest power of 2 of 32-bit v
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    mktree = nullptr;
    numBlocks = n;
    numPads = v;
    treeSize = (v*2) -1;
    if(n == 1 || n == 2){
        treeSize = 3;
    }//update tree size
    levels = log2(v);
    mktree = new Hash<T>[treeSize];
    pad();
    
}

// Constructor: empty tree with root hash large enough to accomodate n blocks
template <typename T>
MerkleTree<T>::MerkleTree(size_t n, const Hash<T>& rootHash)
{
    unsigned int v = n; // compute the next highest power of 2 of 32-bit v
    v--;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;
    numBlocks = n;
    numPads = v;
    mktree = nullptr;
    treeSize = (v*2) -1; //update tree size
    if(n == 1 || n == 2){
        treeSize = 3;
    }//update tree size
    levels = log2(v);
    mktree = new Hash<T>[treeSize];
    pad();
    mktree[0] = rootHash;
}

// Destructor
template <typename T>
MerkleTree<T>::~MerkleTree()
{
    if(mktree != nullptr){
        delete [] mktree;
    }
    
}

// copy constructor
template <typename T>
MerkleTree<T>::MerkleTree(const MerkleTree<T> & x)
{
    
    treeSize = x.treeSize;
    numBlocks = x.numBlocks;
    numPads = x.numPads;
    mktree = treeSize ? new Hash<T>[treeSize] : nullptr;
    std::copy(x.mktree, x.mktree+x.treeSize, mktree);
}

// copy assignment
template <typename T>
MerkleTree<T>& MerkleTree<T>::operator=(MerkleTree <T> x)
{
    swap(*this, x);
    return *this;
}

//for copy-swap idiom
template <typename T>
void MerkleTree<T>::swap(MerkleTree<T>& x, MerkleTree<T>& y)
{
    std::swap(x.mktree, y.mktree);
    std::swap(x.treeSize, y.treeSize);
    std::swap(x.numBlocks, y.numBlocks);
    std::swap(x.numPads, y.numPads);
    
}

//overload ostream operator (useful for debug)
template <typename U>
std::ostream& operator<<(std::ostream& os,const MerkleTree<U>& t)
{
    for(int i = 0; i < t.treeSize; i++){
        os << t.mktree[i] << std::endl;
    }
    return os;
}

// assign root hash of Merkle Tree
template <typename T>
void MerkleTree<T>::setRootHash(const Hash<T>& rootHash)
{
    if(treeSize == 0){
        throw std::runtime_error("merkle tree is empty");
    }
    mktree[0] = rootHash;
}

// return root hash to user
template <typename T>
Hash<T> MerkleTree<T>::getRootHash()
{
    if(treeSize == 0){
        throw std::runtime_error("merkle tree is empty");
    }
    Hash<T> temp = mktree[0];
    return mktree[0];
}

// add data block number blockID to the tree (calculate descendent hashes if possible)
// return range_error if not block-id not in tree
template <typename T>
bool MerkleTree<T>::addBlock(size_t blockID, const T& block)
{
    
    
    if((blockID >= numBlocks) || blockID < 0){
        return false;
    }
    
    size_t blockIndex = block2ind(blockID);
    Hash<T> addTemp(block); //hash data
    if(addTemp.isEmpty()){
        return false;
    }
    mktree[blockIndex] = addTemp;
    updateTree(blockID);
    return true;
    
    
}

// same as above but block data is in array form (size is the number of bytes in block)
template <typename T>
bool MerkleTree<T>::addBlock(size_t blockID, const unsigned char* block, size_t size)
{
    
    if((blockID >= numBlocks) || blockID < 0){
        return false;
    }
    if(size <= 0){
        return false;
    }
    size_t blockIndex = block2ind(blockID);
    Hash<T> addTemp(block, size); //hash data
    if(addTemp.isEmpty()){
        return false;
    }
    mktree[blockIndex] = addTemp;
    updateTree(blockID);
    return true;
}

// verify integrity of block (use sibling and descendents if hash of block isn't in the tree)
// if block is verified add hash to tree and calculate descendent hashes, if necessary
template <typename T>
bool MerkleTree<T>::verifyBlock(size_t blockID, const Hash<T>& blockHash)
{
    if((blockID >= numBlocks) || blockID < 0){
        return false;
    }
    size_t blkInx = block2ind(blockID);
    string str = "";
    Hash<T> blockie = blockHash;
    Hash<T> root = mktree[0];
    Hash<T> sibling = mktree[getSibling(blkInx)];
    Hash<T> temp;
    if(sibling.isEmpty() || root.isEmpty() || blockie.isEmpty()){
        return false;
    }
    if(blkInx % 2 == 0){
        temp = sibling + blockHash;
    }
    else{
        temp = blockHash + sibling;
    }
    
    while(getAunt(blkInx) != 0){
        size_t auntInd = getAunt(blkInx);
        Hash<T> aunt = mktree[auntInd];
        if(aunt.isEmpty()){
            return false;
        }
        if(auntInd % 2 == 0){ //if even
            temp = temp + aunt;
        }
        else{
            temp = aunt + temp;
        }
        
        
        blkInx = getAunt(blkInx);
    }
    return(root == temp);
    
}

// verify integrity of block using attached list of sibling and descendent hashes (if hash of block isn't in the tree)
// if block is verified add hash to tree and incorporate sibling/descendent hashes, if necessary
// hashList contains (in order) hashes for block's sibling and all descendents' hashes up until root node (size is
// number of hashes in hashList; i.e., number of hashes in in hashList)
template <typename T>
bool MerkleTree<T>::verifyBlock(size_t blockID, const Hash<T>& blockHash, const Hash<T> hashList[], size_t size)
{
    if((blockID >= numBlocks) || blockID < 0){
        return false;
    }
    if(size == 0 || size < (levels) ){
        return false;
    }
    
    
    size_t blkInx = block2ind(blockID);
    string str = "";
    Hash<T> blockie = blockHash;
    Hash<T> root = mktree[0];
    Hash<T> sibling = hashList[0]; //sibling from hashlist
    Hash<T> temp;
    if(root.isEmpty() || sibling.isEmpty() || blockie.isEmpty()){
        return false;
    }
    if(blkInx % 2 == 0){
        temp = sibling + blockHash;
    }
    else{
        temp = blockHash + sibling;
    }
    size_t auntInd = getAunt(blkInx);
    for(int i = 1; i < size; i++){
        Hash<T> curr = hashList[i];
        
        if(curr.isEmpty()){
            return false;
            break;
        }
        size_t validSize = curr.returnHash().size();
        if(validSize != 32){ //chec kto see 32bit hash
            return false;
            break;
        }
        if(auntInd % 2 == 0){ //if even
            temp = temp + hashList[i];
        }
        else{
            temp = hashList[i] + temp;
        }
        auntInd = getAunt(blkInx);
        if(root == temp){
            return true;
        }
    }
    //return false;
    
    //    if(!(verifyBlock(blockID, blockHash))){
    //        return false;
    //    }
    
    //{sibling, aunt, great aunt, great-great aunt, etc.}
    
    return(root == temp);
}


//#endif // _MERKLE_TREE_H_
