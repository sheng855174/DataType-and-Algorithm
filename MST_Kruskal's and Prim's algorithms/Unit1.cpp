//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <time.h>
#include <stdlib.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
const int maxsize = 99999;
int **graph;
int **sortGraph;
int *mst;
int *heapIndeces;
int heap[maxsize];
int heapLength;
int weightsNumber;
int sortGraphLength;
int v;
String output = "";
void swap(int&,int&);
void insertHeap(int);
int deleteHeap();
void heapSort();
void swap(int &x,int &y)
{
        int temp = x;
        x = y;
        y = temp;
}
void insertHeap(int x)
{
    heapLength++;
    int i = heapLength;
    heap[i] = x;
    while((i>1) && (x<heap[i/2])){
        swap(heap[i],heap[i/2]);
        swap(heapIndeces[i],heapIndeces[i/2]);
        i/=2;
    }
}
int deleteHeap(){
    int x,i;
    x = heap[1];
    heap[1] = heap[heapLength];
    heapIndeces[1] = heapIndeces[heapLength];
    heapLength--;
    i=1;
    while(i<=heapLength/2){
        int j;
        if(heap[2*i] < heap[2*i+1]) j=2*i;
        else j=2*i+1;
        if(heap[i]<heap[j]) break;
        swap(heap[i],heap[j]);
        swap(heapIndeces[i],heapIndeces[j]);
        i=j;
    }
    return x;
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        srand(time(NULL));
        Memo1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{
        //初始設定  intput
        Memo1->Clear();
        int weightsRange = StrToInt(Edit2->Text);
        int weightsJudge = StrToInt(Edit3->Text);
        const int maxInt = StrToInt(Edit4->Text);
        sortGraphLength = 0;
        v = StrToInt(Edit1->Text);
        weightsNumber = 0;
        heapLength = 0;
        output = "";
        graph = new int*[v];
        for(int i=0;i<v;i++)
        {
            graph[i] = new int[v];
        }
        if(CheckBox1->Checked == true)
        {
            StringGrid1->RowCount = v;
            StringGrid1->ColCount = v;
        }
        for(int i=0;i<v;i++)
        {
            for(int j=0+i;j<v;j++)
            {
                if(i==j)
                {
                    graph[i][j] = maxInt;
                }
                else
                {
                    int random = rand()%weightsRange+1;
                    if(random > weightsJudge)
                    {
                        graph[i][j] = maxInt;
                        graph[j][i] = maxInt;
                    }
                    else
                    {
                        graph[i][j] = random;
                        graph[j][i] = random;
                        weightsNumber++;
                    }
                }
                if(CheckBox1->Checked == true)
                {
                    StringGrid1->Cells[j][i] = IntToStr(graph[i][j]);
                    StringGrid1->Cells[i][j] = IntToStr(graph[j][i]);
                }
            }
        }
        //原來圖形矩陣轉成排序用矩陣(較小)
        sortGraph = new int*[weightsNumber+1];
        for(int i=0;i<=weightsNumber;i++)
        {
            sortGraph[i] = new int[3];
        }
        for(int i=0;i<v;i++)
        {
            for(int j=0+i;j<v;j++)
            {
                if(graph[i][j] != maxInt)
                {
                    sortGraphLength++;
                    sortGraph[sortGraphLength][0] = i;
                    sortGraph[sortGraphLength][1] = j;
                    sortGraph[sortGraphLength][2] = graph[i][j];
                }
            }
        }
        if(CheckBox1->Checked == true)
        {
            StringGrid2->RowCount = sortGraphLength+1;
            StringGrid2->ColCount = 3;
            StringGrid2->Cells[0][0] = "A點";
            StringGrid2->Cells[1][0] = "B點";
            StringGrid2->Cells[2][0] = "成本";
            for(int i=1;i<=weightsNumber;i++)
            {
                StringGrid2->Cells[0][i] = IntToStr(sortGraph[i][0]);
                StringGrid2->Cells[1][i] = IntToStr(sortGraph[i][1]);
                StringGrid2->Cells[2][i] = IntToStr(sortGraph[i][2]);
            }
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid1DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    StringGrid1->Canvas->FillRect(Rect);
    DrawText(StringGrid1->Canvas->Handle,
    StringGrid1->Cells[ACol][ARow].c_str(),
    StringGrid1->Cells[ACol][ARow].Length(),
    &Rect,DT_VCENTER | DT_SINGLELINE | DT_CENTER);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::StringGrid2DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
    StringGrid2->Canvas->FillRect(Rect);
    DrawText(StringGrid2->Canvas->Handle,
    StringGrid2->Cells[ACol][ARow].c_str(),
    StringGrid2->Cells[ACol][ARow].Length(),
    &Rect,DT_VCENTER | DT_SINGLELINE | DT_CENTER);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
        clock_t start = clock();
        int mstLength = 0;
        int *loop;
        int sum =0;
        output = "";
        mst = new int [weightsNumber+1];
        loop = new int[weightsNumber+1];
        heapIndeces = new int[weightsNumber+1];
        for(int i=0;i<=weightsNumber;i++)
        {
            mst[i] = 0;
            loop[i] = i;
            heapIndeces[i] = i;
        }
        for(int i=1;weightsNumber!=0 && i<=sortGraphLength;i++)
        {
            insertHeap(sortGraph[i][2]);
        }
        for(int i=1;weightsNumber!=0 && i<=weightsNumber;i++)
        {
            int min = heapIndeces[1];
            if(min<0 || min>weightsNumber) break;
            int u = sortGraph[min][0];
            int v = sortGraph[min][1];
            if(loop[u] != loop[v])
            {
                mst[mstLength] = min;
                sum += sortGraph[mst[mstLength]][2];
                mstLength++;
                int temp = loop[v];
                for(int j=0;j<weightsNumber;j++)
                {
                    if(loop[j] == temp)
                    {
                        loop[j] = loop[u];
                    }
                }
            }
            deleteHeap();
        }
        if(CheckBox2->Checked == true)
        {
            for(int i=0;i<mstLength;i++)
            {
                int u = sortGraph[mst[i]][0];
                int v = sortGraph[mst[i]][1];
                int r = sortGraph[mst[i]][2];
                output += "Edge" + IntToStr(i+1) + ":(" + IntToStr(u) + "," + IntToStr(v) + "):" + IntToStr(r);
                output += "\r\n";
            }
            if(mstLength == v-1)
            {
                Memo1->Lines->Add("MST:");
                Memo1->Lines->Add(output);
            }
            else
            {
                Memo1->Lines->Add("無MST");
            }
        }
        delete[] loop;
        delete[] mst;
        delete[] heapIndeces;
        clock_t end = clock();
        float costTime = (float)(end - start)/CLK_TCK;
        Memo1->Lines->Add("Kruskal's alogorithm(" + IntToStr(v) + "V," + IntToStr(sum) + "," + FloatToStr(costTime) + "s)" + "\r\n");
}
//---------------------------------------------------------------------------



void __fastcall TForm1::Button3Click(TObject *Sender)
{
        bool *check;
        int **data;
        int min[2];
        int sum = 0;
        min[0] = 0;
        check = new bool[v];
        data = new int*[v];
        for(int i=0;i<v;i++)
        {
            data[i] = new int[2];
            check[i] = 0;
            data[i][1] = maxsize;
        }
        clock_t start = clock();
        for(int i=0;i<v;i++)
        {
            min[1] = 9999;
            for(int j=0;j<v;j++)
            {
                if(graph[min[0]][j] < data[j][1] && check[j] == 0)
                {
                     data[j][0] = min[0];
                     data[j][1] = graph[min[0]][j];
                }
            }
            check[min[0]]=1;
            for(int j=0;j<v;j++)
            {
                if(data[j][1] < min[1] && check[j]==0)
                {
                    min[1] = data[j][1];
                    min[0] = j;
                }
            }
        }
        clock_t end = clock();
        float costTime = (float)(end-start)/CLK_TCK;
        if(CheckBox2->Checked)
        {
            for(int i=1;i<v;i++)
            {
                Memo1->Lines->Add("Edge"+IntToStr(i-1)+": (" + IntToStr(i)+ "," + IntToStr(data[i][0]) +"):" + IntToStr(data[i][1]));
            }
            Memo1->Lines->Add("");
        }
        for(int i=1;i<v;i++)
        {
            sum += data[i][1];
        }
        Memo1->Lines->Add("Prim's alogorithm(" + IntToStr(v) + "V," + IntToStr(sum) + "," + FloatToStr(costTime) + "s)" + "\r\n");
}
//---------------------------------------------------------------------------

