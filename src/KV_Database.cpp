#include "KV_Database.h"

int main(){
    int m;
    cin>>m;
    BlockList<int> block_list("theProject") ;
    string command;
    for (int i=0;i<m;++i){
        try{
     cin>>command;
     if(command=="insert"){
         char obj[indexLen];
         int value;
         cin>>obj>>value;
         auto pair=Pair<int>(obj,value);
         block_list.Add(pair);
     }
     if(command=="delete"){
         char obj[indexLen];
         int value;
         cin>>obj>>value;
         auto pair=Pair<int>(obj,value);
         block_list.Delete(pair);
     }
     if(command=="find"){
         char obj[indexLen];
         cin>>obj;
         block_list.Find(obj);
     }
        }
        catch (...){}
    }
    return 0;
}