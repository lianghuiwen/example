#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

void fstream_test()
{
    ifstream fin("stream_test.cpp");
    ifstream fin("stream_test.cpp", ios::in|ios::binary);
    if(!fin) {
        cout<<"open file fail!"<<endl;
        return;
    }
    int size = fin.teelg();
    fin.seekg(ios::beg);
    fin.seekg(ios::end);
    string line;
    while(getline(fin, line)) {
        cout<<line<<endl;
    }
    string word;
    while(fin>>word) {
        cout<<word<<endl;
    }
    char buf[30];
    while(fin.read(buf, sizeof(buf))) {
        cout<<buf<<endl;
    }
    char ch;
    while(fin.get(ch)) {
        cout<<ch<<endl;
    }
    fin.close();

    ofstream fout("stream_test.log");
    if(!fout) {
        cout<<"open file fail!"<<endl;
        return;
    }
    for(int i=0; i<10; ++i) {
        fout<<"test"<<i<<endl;
    }
    fout.put('A');
    fout.write("test", 4);
    fout.close();
}

void sstream_test()
{
    stringstream ss;
    string str;
    ss<<"just a test "<<2013<<"!";
    cout<<ss.str()<<endl;
    while(ss>>str) {
        cout<<str<<endl;
    }
    cout<<ss.str()<<endl;   // just a test 2013!
}

void file_test()
{
    FILE* fp;
    fp = fopen("abc.txt", "r");
    fp = fopen("abc.txt", "w");
    fp = fopen("abc.txt", "w+");
    fp = fopen("abc.txt", "wb+");

    int off = 0;
    int pos = 0;
    int len = ftell(fp);
    fseek(fp, off, pos);

    char buf[255];
    fread(buf, sizeof(buf), 1, fp); 
    fwrite(buf, sizeof(buf), 1, fp);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    fstream_test();
    sstream_test();
    file_test();
}