//  Created by Faraz Khan on 12/14/17.
//  Copyright © 2017 Faraz Khan. All rights reserved.
//  ECE 2574
//  Professor Gerdes
#ifndef _MERKLE_TREE_H_
#define _MERKLE_TREE_H_

#include <iostream>
#include <string>
#include <cmath>
#include <locale>
#include <numeric>
#include <cstddef>
#include <inttypes.h>

#include "hash.hpp"

template <typename T>
class MerkleTree
{
public:
  // Constructor: default...shouldn't ever be called directly by user
  MerkleTree();
  
  // Constructor: empty tree without root hash large enough to accomodate n blocks
  MerkleTree(size_t n);
  
  // Constructor: empty tree with root hash large enough to accomodate n blocks
  MerkleTree(size_t n, const Hash<T>& rootHash);
  
  // Destructor
  ~MerkleTree();

  // copy constructor
  MerkleTree(const MerkleTree<T> & x);
  
  // copy assignment
  MerkleTree<T>& operator=(MerkleTree <T> x);

  //for copy-swap idiom
  void swap(MerkleTree<T>& x, MerkleTree<T>& y);

  //overload ostream operator (useful for debug)
  template <typename U>
  friend std::ostream& operator<<(std::ostream& os,const MerkleTree<U>& t);

  // assign root hash of Merkle Tree
  void setRootHash(const Hash<T>& rootHash);
  
  // return root hash to user
  Hash<T> getRootHash();

  // add data block number blockID to the tree (calculate descendent hashes if possible)
  // return range_error if not block-id not in tree
  bool addBlock(size_t blockID, const T& block);

  // same as above but block data is in array form (size is the number of bytes in block)
  bool addBlock(size_t blockID, const unsigned char* block, size_t size);

  // verify integrity of block (use sibling and descendents if hash of block isn't in the tree)
  // if block is verified add hash to tree and calculate descendent hashes, if necessary
  bool verifyBlock(size_t blockID, const Hash<T>& blockHash);

  // verify integrity of block using attached list of sibling and descendent hashes (if hash of block isn't in the tree)
  // if block is verified add hash to tree and incorporate sibling/descendent hashes, if necessary
  // hashList contains (in order) hashes for block's sibling and all descendents' hashes up until root node (size is
  // number of hashes in hashList; i.e., number of hashes in in hashList)
  bool verifyBlock(size_t blockID, const Hash<T>& blockHash, const Hash<T> hashList[], size_t size);
  
  
private:
  // Array-based implementation of Merkle tree (root node at index zero)
  // Pointer to an array of Hash<T> objects
  Hash<T> * mktree;
  // number of nodes (including root) in the tree
  size_t treeSize;
    
    size_t levels;

  // note: the following provide indices into mktree (i.e., absolute index of node and not with respect to blockID)
  size_t getLeftChild(size_t parentNode); //left child of parent node
  size_t getRightChild(size_t parentNode); //right child of parent node
  size_t getParent(size_t childNode); //parent node
  size_t getSibling(size_t childNode); //sister node
  size_t getAunt(size_t childNode);  //parent's sibling

  // NOTE: the following are recommended but not required
  size_t numBlocks; // number of non-padding blocks in the tree
  size_t numPads; // number of padding blocks in the tree

  size_t block2ind(size_t blockID); //convert blockID to index of block's hash in mktree
  void pad(); //set hash of padding blocks; also update hashes of descendents, if possible
  void updateTree(size_t blockID); //calculate descendent hashes after adding block, if necessary
};

#include "merkle_tree.cpp"

#endif // _MERKLE_TREE_H_
