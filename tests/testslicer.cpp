#include "../source/core/slicer/slicer.hpp"
#include<iostream>
#include<cassert>
int main(const int argc, char *argv[]){

    //false buffer ""
   /* std::vector<char> test_buffer = {'i', 'n', 't', ' ', 'm', 'a', 'i', 'n', '(', 'c', 'o', 'n', 's', 't', ' ',
                                     'i', 'n', 't', ' ', 'a', 'r', 'g', 'c', ',', ' ', 'c', 'h', 'a', 'r', ' ',
                                     '*', 'a', 'r', 'g', 'v', '[', ']', ')', '{', '\n', ' ', ' ', 'm', 't', 'v',
                                     ':', ':', 'A', 'r', 'g', 's', ' ', '&', 'a', 'r', 'g', 's', ' ', '=', ' ',
                                     'm', 't', 'v', ':', ':', 'A', 'r', 'g', 's', ':', ':', 'g', 'e', 't', '_',
                                     'i', 'n', 's', 't', 'a', 'n', 'c', 'e', '(', 'a', 'r', 'g', 'c', ',', ' ',
                                     'a', 'r', 'g', 'v', ')', ';', '\n', ' ', ' ', 'a', 'r', 'g', 's', '.', 'r',
                                     'e', 'c', 'o', 'n', '(', ')', ';', '\n', ' ', ' ', 'i', 'f', ' ', '(', 'a',
                                     'r', 'g', 's', '.', 's', 'h', 'o', 'u', 'l', 'd', '_', 'e', 'x', 'i', 't', '(',
                                     ')', ')', ' ', '{', '\n', ' ', ' ', ' ', ' ', 'r', 'e', 't', 'u', 'r', 'n',
                                     ' ', '0', ';', '\n', ' ', ' ', '}', '\n', ' ', ' ', 'a', 'u', 't', 'o', ' ',
                                     'o', 'u', 't', 'p', 'u', 't', '_', 'f', 'i', 'l', 'e', ' ', '=', ' ', 'a',
                                     'r', 'g', 's', '.', 'g', 'e', 't', '_', 'o', 'u', 't', 'p', 'u'};*/
    /*mtv::slicer &slicer = mtv::slicer::get_instance(test_buffer);

    for(int i = 0; i < test_buffer.size(); i++){
        slicer.getnext_token();
    }*/

    // false buffer "se obtendrá de clase buffer"
    std::string str = "entero a = 12;\nsi (a = 12){}\nsino{ float b = 0.1; \n far.get();}";
    std::vector<char> test_buffer(str.begin(), str.end());

    mtv::slicer &slicer = mtv::slicer::get_instance(test_buffer);

    for(int i = 0; i < test_buffer.size(); i++){
        std::string token = slicer.getnext_token();
        while(token != "fin"){
            std::cout << token << std::endl;
            token = slicer.getnext_token();
        }
    }
    for(const auto &token : slicer.get_tokens()){
        std::cout <<"token del vector : " << token << std::endl;
    }

    return 0;
}
