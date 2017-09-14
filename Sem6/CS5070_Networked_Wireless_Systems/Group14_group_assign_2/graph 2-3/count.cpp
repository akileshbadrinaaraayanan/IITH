#include<iostream>
using namespace std;
int main(){
	
	int a[25];
	for(int i=0;i<=25;i++){
		a[i]=0;
	}

	for(int i=0;i<21;i++){
		int temp;
		cin>>temp;
		a[temp]++;
	}
	for(int i=1;i<=25;i++)
		cout<< i <<"     "<<a[i]<<endl;
	return 0;
}


