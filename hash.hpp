//  Created by Faraz Khan on 12/14/17.
//  Copyright © 2017 Faraz Khan. All rights reserved.
//  ECE 2574
//  Professor Gerdes
#ifndef _HASH_H_
#define _HASH_H_

#include <iostream>
#include <string>
#include <vector>

#include "picosha2.h"

using std::vector;
using std::string;

template <typename T>
class Hash
{
public:
  // Constructor: default
  Hash();
  
  // Constructor: hash from byte sequence
  Hash(const unsigned char* data, size_t size);
  
  // Constructor: hash from any STL sequential container
  Hash(const T& data);
  
  // Destructor
  ~Hash();

  // copy constructor
  Hash(const Hash<T> & x);
  
  // copy assignment
  Hash<T>& operator=(Hash <T> x);

  //for copy-swap idiom
  void swap(Hash<T>& x, Hash<T>& y);

  //overload ostream operator
  template <typename U>
  friend std::ostream& operator<<(std::ostream& os,const Hash<U>& list);

  //comparison == (lhs == rhs)
  bool operator==(const Hash<T> & rhs) const;

  //comparison != (lhs != rhs)
  bool operator!=(const Hash<T> & rhs) const;

  //addition operator: result = hash(lhs + rhs)
  Hash<T> operator+(const Hash<T> & rhs) const;

  // assign hash (in byte form): should be rarely used (use constructors instead)
  void setHash(const std::vector<unsigned char>& x);
  
  //return hash to user (in byte form)
  std::vector<unsigned char> returnHash();

  //return has to user (in hex string form)
  std::string returnHashString();

  // tells us whether hash has been set or only the default (all zeros)
  bool isEmpty();
  
private:
  // Private Constructor: used to take two Hashes and combine into one
  Hash(const Hash<T> & x, const Hash<T> & y);
  
  // our hash: SHA256 is 32 bytes (unsigned chars) in length
  std::vector<unsigned char> h;
};

#include "hash.cpp"

#endif // _HASH_H_
