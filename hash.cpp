//  Created by Faraz Khan on 12/14/17.
//  Copyright © 2017 Faraz Khan. All rights reserved.
//  ECE 2574
//  Professor Gerdes
#include "hash.hpp"

using std::string;
using std::vector;

// -------------------------------------------------------
template <typename T>
Hash<T>::Hash(const Hash<T> & x, const Hash<T> & y)
{
    string left = picosha2::bytes_to_hex_string(x.h.begin(), x.h.end());
    string right = picosha2::bytes_to_hex_string(y.h.begin(), y.h.end());
    unsigned char combo[64] = {0}; //create 64bit char array to combine strings left and right
    for(int i = 0; i < 32; i++){
        //h.push_back(rhs.h.at(i));
        combo[i] = (x.h.at(i));
        combo[i+32] = (y.h.at(i));
    }
    
    Hash<T> add(combo, 64);
    h = add.h;
}
// -------------------------------------------------------
template <typename T>
Hash<T>::Hash()
{

}

template <typename T>
Hash<T>::Hash(const unsigned char *data, size_t size)
{
  // picosha2 requires h to be preallocated
  h = std::vector<unsigned char>(32);
  
  picosha2::hash256(data, data+size, h);
}

template <typename T>
Hash<T>::Hash(const T& data)
{
  // picosha2 requires h to be preallocated
  h = std::vector<unsigned char>(32);

  picosha2::hash256(data, h);
}

// Destructor
template <typename T>
Hash<T>::~Hash()
{

}

// copy constructor
template <typename T>
Hash<T>::Hash(const Hash<T> & x)
{
    h = x.h;
}

// copy assignment
template <typename T>
Hash<T>& Hash<T>::operator=(Hash <T> x)
{
    swap(*this, x);
    return *this;
}

//for copy-swap idiom
template <typename T>
void Hash<T>::swap(Hash<T>& x, Hash<T>& y)
{
    std::swap(x.h, y.h);
}

//overload ostream operator
template <typename U>
std::ostream& operator<<(std::ostream& os,const Hash<U>& list)
{
    Hash<U> hashTemp(list);
    os << hashTemp.returnHashString();
    return os;
}

//comparison == (lhs == rhs)
template <typename T>
bool Hash<T>::operator==(const Hash<T> & rhs) const
{
    string left = picosha2::bytes_to_hex_string(h.begin(), h.end());
    string right = picosha2::bytes_to_hex_string(rhs.h.begin(), rhs.h.end());
    return ( left.compare(right) == 0 );
}

//comparison != (lhs != rhs)
template <typename T>
bool Hash<T>::operator!=(const Hash<T> & rhs) const
{

    return (!(*this == rhs));
}

//addition operator: result = hash(lhs + rhs)
template <typename T>
Hash<T> Hash<T>::operator+(const Hash<T> & rhs) const
{
    if(h.empty() || rhs.h.empty()){
        throw std::runtime_error("Hash is empty");
    }

    Hash<T> combined(*this, rhs);
    return combined;
}

// assign hash (in byte form): should be rarely used (use constructors instead)
template <typename T>
void Hash<T>::setHash(const std::vector<unsigned char>& x)
{
    if(x.size() == 32){
        h = x;
    }
    
}

//return hash to user (in byte form)
template <typename T>
std::vector<unsigned char> Hash<T>::returnHash()
{
    if(h.empty()){
        throw std::runtime_error("Hash is empty");
    }
    return h;
}

//return has to user (in hex string form)
template <typename T>
std::string Hash<T>::returnHashString()
{
    if(h.empty()){
        throw std::runtime_error("Hash is empty");
    }
    std::string stringHash = picosha2::bytes_to_hex_string(h.begin(), h.end());
    return stringHash;
    
}

// tells us whether hash has been set or only the default (all zeros)
template <typename T>
bool Hash<T>::isEmpty()
{
    return (h.empty());
}
