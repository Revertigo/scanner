


          _______.  ______     ___      .__   __. .__   __.  _______ .______      
         /       | /      |   /   \     |  \ |  | |  \ |  | |   ____||   _  \     
        |   (----`|  ,----'  /  ^  \    |   \|  | |   \|  | |  |__   |  |_)  |    
         \   \    |  |      /  /_\  \   |  . `  | |  . `  | |   __|  |      /     
     .----)   |   |  `----./  _____  \  |  |\   | |  |\   | |  |____ |  |\  \----.
     |_______/     \______/__/     \__\ |__| \__| |__| \__| |_______|| _| `._____|
                                                                             



## Introduction
This project is partial implementation for a scanner program (second phase of compilation process), also known as lexical analysis. It has two main purposes:
1. tokenizing ==> reading the text and return the different tokens inside it.
2. Cross referencing(xref) ==> listing every file name and line number an identifier occurs within the program. In my case, there is no support for scanning
multiple files so only line numbers are listed.

## Installation
```
mkdir build
cd build
cmake ..
make
```
