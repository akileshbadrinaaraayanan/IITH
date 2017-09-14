#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

int main() {
    for(int i=0; i<10; i++)
    {
        FILE *in;
        char buff[512];
        string str = "./waf --run \"scratch/first --RngRun=";
        int t = 42+i;
        str += to_string(t);

        str += " --nPackets=100 --errorModelType=BurstErrorModel\" > log.out 2>&1";

        //cout << "string "<<str;
        if(!(in = popen(str.c_str(), "r"))){
            return 1;
        }

        while(fgets(buff, sizeof(buff), in)!=NULL){
            cout << buff;
        }

        int p = system("grep sent log.out | wc -l");
        int q = system("grep received log.out | wc -l");
        
        pclose(in);
        cout << endl;
    }
    return 0;
}