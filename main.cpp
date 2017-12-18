//
//  main.cpp
//  BitTorrent
//
//  Created by Faraz Khan on 12/13/17.
//  Copyright © 2017 Faraz Khan. All rights reserved.
//

#include <iostream>
#include "BitTorrent.hpp"
#include <vector>
#include <unistd.h>
#include <string>
#include <fstream>

using std::string;
using std::cout;
using std::endl;
using std::vector;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::vector<std::string>  arguments;
    
    string torrFile = "";
    string binFile = "";
    for(int i = 0; i < argc; ++i) arguments.push_back(argv[i]);
    if(arguments.size() != 4){
        cout << "Error: not enough/too many arguments" << endl;
        return EXIT_FAILURE;
    }
    if(arguments.at(1) != "-v" && arguments.at(1) != "-c"){
        cout << "Error: invalid arguments" << endl;
        return EXIT_FAILURE;
    }
    torrFile = arguments.at(2);
    binFile = arguments.at(3);
    
    if(arguments.at(1) == "-v"){
        if(readTorrent(binFile, torrFile)){
            std::fstream in(binFile,std::ios::binary|std::ios::in);
            std::fstream out("./data.bin",std::ios::binary|std::ios::out);
            char c;
            while(true) {
                c=in.get();
                if(in.eof()) break;
                out.put(c);
            }
            in.close();
            out.close();
            cout << "data is valid" << endl;
        }
        else{
            cout << "data is invalid" << endl;
            return EXIT_FAILURE;
        }
    }
    else{ //if it's "-c"
        
        makeTorr(torrFile, binFile);
        cout << "succesfully created torrent" << endl;
    }

    
    
    
//    char cwd[1024];
//    if (getcwd(cwd, sizeof(cwd)) != NULL)
//        fprintf(stdout, "Current working dir: %s\n", cwd);
//    else
//        perror("getcwd() error");
    
    return EXIT_SUCCESS;
}
