//
//  BitTorrent.hpp
//  BitTorrent
//
//  Created by Faraz Khan on 12/14/17.
//  Copyright © 2017 Faraz Khan. All rights reserved.
//  ECE 2574
//  Professor Gerdes

//#ifndef BitTorrent_h
//#define BitTorrent_h
#include <stdio.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "merkle_tree.hpp"
#include "hash.hpp"

//#include "BitTorrent.cpp"


    int bytesToInt(unsigned char* b, unsigned length);
    bool readData(string fname, Hash<string> &root, int numBlocks);
    bool readTorrent(string binfname, string torrfname);
    void makeTorr(string torrName, string binName);




//#endif /* BitTorrent_h */
