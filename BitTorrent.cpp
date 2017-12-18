//
//  BitTorrent.cpp
//  BitTorrent
//
//  Created by Faraz Khan on 12/13/17.
//  Copyright © 2017 Faraz Khan. All rights reserved.
//
#include "BitTorrent.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::fstream;

struct dataB {
    unsigned char a[256];
};


int bytesToInt(unsigned char* b, unsigned length)

{
    int val = 0;
    int j = 0;
    for (int i = length-1; i >= 0; --i)
    {
        val += (b[i] & 0xFF) << (8*j);
        ++j;
    }
    
    return val;
}


bool readTorrent(string binfname, string torrfname)
{
    fstream torr(torrfname, std::ios::binary | std::ios::in);
    if(!torr.is_open()){
        throw std::runtime_error("file opening error");
    }
    char c;
    vector<unsigned char> torrie;
    torr.seekg(0); //start at pos 0
    while(torr.good()){
        torr.get(c);
        torrie.push_back(c);
    }
    torr.close();
    vector<unsigned char> root(32); //root hash
    unsigned char tempSize[4]; //arr for size bytes
    string fileName = "";
    for(int i = 0; i < 32; i++) root[i] = torrie[i];
    for(int x = 32; x < 36; x++) tempSize[35-x] = torrie[x];
    for(int y = 36; y < torrie.size(); y++) fileName += torrie[y];
    fileName.erase(std::remove(fileName.begin(), fileName.end(), '\0'), fileName.end());
    int sizeBlock = bytesToInt(tempSize, 4);
    
    Hash<string> roothashie;
    roothashie.setHash(root);
    return (readData(binfname, roothashie, sizeBlock));
    
    
}

bool readData(string fname, Hash<string> &root, int numBlocks)
{
    Hash<string> * mktree;
    mktree = nullptr;
    int size = 0; //second 8 bytes of metadata
    int blockID = 0; //first 8 bytes of metadata
    int i = 0; //count variable for file inputting
    unsigned char tempB[4];
    unsigned char tempS[4] = {0};
    vector<unsigned char> hashie; //temp hash storage
    vector<Hash<string>> hashList(50000);
    
    vector<vector<unsigned char>> dataStorage(50000);
    
    //vector<unsigned char[]> data;
    
    
    fstream file(fname, std::ios::binary | std::ios::in);
    
    if(!file.is_open()){
        //throw std::runtime_error("file opening error");
    }
    char c;
    int count = 0;
    int dcount = 0;
    file.seekg(0);
    while(file.good()){
        
        file.get(c);
        hashie.push_back(c);
    }
    
//    for(int i = 0; i < 4; i++){
//        tempB[3-i] = hashie[i]; // grab block little endian
//        tempS[3-i] = hashie[i+4]; // grab size little endian
//    }
    file.close();
    vector<int> blockind;
    vector<int> sizes;
    int j = 0;
    int ext = 0;
    int ext2 = 0;
    int hashc = 0;
    int dc = 0;
    for(int i = 0; i < hashie.size(); i++){

        if(j < 4){
            tempB[3-j] = hashie[i];
            
            if(j == 3){
                blockID = bytesToInt(tempB, 4);
                blockind.push_back(blockID);
            }
        }
        else if(j < 8){
            tempS[7-j] = hashie[i];
            if(j == 7){
                size = bytesToInt(tempS, 4); //create size integer
                sizes.push_back(size);
            }
        }
        else if(j < (size*32 + 8)){
            count++;
            if(count == 32){
                vector<unsigned char> ins(hashie.begin()+i-31, hashie.begin()+i+1);
                //std::reverse(ins.begin(), ins.end());
                Hash<string> temp;
                temp.setHash(ins);
                hashList[ext+blockID+blockID] = temp;//push back newly created hash
                count = 0;
                ext++;
                hashc++;
            }
            if(j == size*32 +7){
                ext = 0;
            }
            
             //increment hash size counter
            //hash list
        }
        else if(j >= (size*32 + 8) && j < (size*32 +256 +7)){
            //data blocks
            dcount++;
            if(dcount == 255){
                vector<unsigned char> d(hashie.begin()+i-254, hashie.begin()+i+2);
                dataStorage[blockID] = d;
                dcount = 0;
                dc++;
            }

        }
        else{
            
            j = -1;
        }
        j++;
    }

    //dataStorage = data blocks
    //hashList = hash list lol
    vector<dataB> blockHashs;
    for(int i =0; i < dc; i++){
        dataB t;
        for(int x = 0; x < 256; x++) t.a[x] = dataStorage.at(i)[x];
        blockHashs.push_back(t);
    }
    MerkleTree<string> meta(dc);
    meta.setRootHash(root);
    for(int i = 0; i < blockHashs.size(); i++) meta.addBlock(i, blockHashs[i].a, 256);
    for(int i = 0; i < dc; i++){
        int sizzle = sizes.at(i);
        Hash<string> blockie(blockHashs[i].a,256);
        //cout << blockie << endl;
        mktree = new Hash<string>[sizzle];
        for(int x = 0; x < sizzle; x++){
            mktree[x] = hashList[x];
            if(x == sizzle-1) hashList.erase(hashList.begin(), hashList.begin()+x+1);
        }
        //for(int y = 0; y < sizzle; y++) cout << mktree[y] << endl;
        if(!(meta.verifyBlock(i, blockie, mktree, sizzle))){
            return false;
        }
        delete [] mktree;
    }
    return true;
    //cout << meta << endl;
    //contains blocks hashes {data0, data1, etc}
    
    //for(int i = 0; i < dataStorage.at(1).size(); i++) cout << dataStorage.at(1)[i];
}




void makeTorr(string torrName, string binName)
{
    Hash<string> * mktree;
    mktree = nullptr;
    int size = 0; //second 8 bytes of metadata
    int blockID = 0; //first 8 bytes of metadata
    unsigned char tempB[4]; //char array for block id
    unsigned char tempS[4] = {0}; //char array for size
    vector<unsigned char> hashie; //temp hash storage
    vector<Hash<string>> hashList(50000);
    
    vector<vector<unsigned char>> dataStorage(50000);
    
    //vector<unsigned char[]> data;
    
    
    fstream file(binName, std::ios::binary | std::ios::in);
    
    if(!file.is_open()){
        //throw std::runtime_error("file opening error");
    }
    char c;
    int count = 0;
    int dcount = 0;
    file.seekg(0);
    while(file.good()){
        
        file.get(c);
        hashie.push_back(c);
    }
    
    //    for(int i = 0; i < 4; i++){
    //        tempB[3-i] = hashie[i]; // grab block little endian
    //        tempS[3-i] = hashie[i+4]; // grab size little endian
    //    }
    file.close();
    vector<int> blockind;
    vector<int> sizes;
    int j = 0;
    int ext = 0;
    int ext2 = 0;
    int hashc = 0;
    int dc = 0;
    for(int i = 0; i < hashie.size(); i++){
        
        if(j < 4){
            tempB[3-j] = hashie[i];
            
            if(j == 3){
                blockID = bytesToInt(tempB, 4);
                blockind.push_back(blockID);
            }
        }
        else if(j < 8){
            tempS[7-j] = hashie[i];
            if(j == 7){
                size = bytesToInt(tempS, 4); //create size integer
                sizes.push_back(size);
            }
        }
        else if(j < (size*32 + 8)){
            count++;
            if(count == 32){
                vector<unsigned char> ins(hashie.begin()+i-31, hashie.begin()+i+1);
                //std::reverse(ins.begin(), ins.end());
                Hash<string> temp;
                temp.setHash(ins);
                //cout << blockID << endl;
                //cout << temp.returnHashString() << endl;
                hashList[ext+blockID+blockID] = temp;//push back newly created hash
                count = 0;
                ext++;
                hashc++;
            }
            if(j == size*32 +7){
                ext = 0;
            }
            
            //increment hash size counter
            //hash list
        }
        else if(j >= (size*32 + 8) && j < (size*32 +256 +7)){
            //data blocks
            dcount++;
            if(dcount == 255){
                vector<unsigned char> d(hashie.begin()+i-254, hashie.begin()+i+2);
                dataStorage[blockID] = d;
                dcount = 0;
                dc++;
            }
            
        }
        else{
            
            j = -1;
        }
        j++;
    }
    
    //dataStorage = data blocks
    //hashList = hash list lol
    vector<dataB> blockHashs;
    for(int i =0; i < dc; i++){
        dataB t;
        for(int x = 0; x < 256; x++) t.a[x] = dataStorage.at(i)[x];
        blockHashs.push_back(t);
    }
    MerkleTree<string> meta(dc);
    for(int i = 0; i < blockHashs.size(); i++) meta.addBlock(i, blockHashs[i].a, 256);
    //meta.setRootHash(root); not needed
    vector<unsigned char> rootie = meta.getRootHash().returnHash();
    //dc = block count
    //fname
    //
    char dc2byte[4];
    dc2byte[3] = dc>>24;
    dc2byte[2] = dc>>16;
    dc2byte[1] = dc>>8;
    dc2byte[0] = dc;
    
    std::fstream out(torrName,std::ios::binary|std::ios::out);
    for(int i = 0; i < 32; i++) out.put(rootie[i]);
    for(int i = 0; i < 4; i++) out.put(dc2byte[3-i]);
    for(int i = 0; i < binName.size(); i++) out.put(binName[i]);
    out.close();

}

