//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
class BTreeNode
{
        public:
                BTreeNode *leftChild;
                int data;
                BTreeNode *rightChild;
};
class StackNode
{
        public:
                BTreeNode *treeNode;
                StackNode *next;
                StackNode()
                {
                        treeNode = NULL;
                        next = NULL;
                }
};
class   StackData
{
        public:
                int data;
                StackData *next;
                StackData()
                {
                        data = NULL;
                        next = NULL;
                }
};
BTreeNode *root;
StackNode *top;
StackData *top_data;
String output="";
void inorder(BTreeNode *root);
void preoder(BTreeNode *root);
void postorder(BTreeNode *root);
BTreeNode* Search(BTreeNode *root,int target);
BTreeNode* InsertBST(BTreeNode *node,int x);
BTreeNode* NewBSTNode(int x);
void push(BTreeNode *node);
BTreeNode* pop();
void push_data(int data);
int pop_data();

void inorder(BTreeNode *root)
{
        if(root!=NULL)
        {
                inorder(root->leftChild);
                output += IntToStr(root->data) + "_";
                inorder(root->rightChild);
        }
}
void preorder(BTreeNode *root)
{
        if(root != NULL)
        {
                output += IntToStr(root->data)+ "_";
                preorder(root->leftChild);
                preorder(root->rightChild);
        }
}
void postorder(BTreeNode *root)
{
        if(root != NULL)
        {
                postorder(root->leftChild);
                postorder(root->rightChild);
                output += IntToStr(root->data) + "_";
        }
}
BTreeNode* Search(BTreeNode *root,int target)
{
        if(root==NULL) return root;//�S���
        else if(root->data == target) return root;//���
        else if(root->data > target) return Search(root->leftChild,target);
        else if(root->data < target) return Search(root->rightChild,target);
}
BTreeNode* NewBSTNode(int x)
{
        BTreeNode *node = new BTreeNode;
        node->leftChild = NULL;
        node->data = x;
        node->rightChild = NULL;
        return node;
}
BTreeNode* InsertBST(BTreeNode *node,int x)
{
        if(node == NULL) return NewBSTNode(x);
        if(x < node->data)
        {
                node->leftChild = InsertBST(node->leftChild,x);
        }
        else
        {
                node->rightChild = InsertBST(node->rightChild,x);
        }
        return node;
}
void push(BTreeNode *node)
{
        StackNode *old_top;
        old_top = top;
        top = new StackNode;
        top->treeNode = node;
        top->next = old_top;
}
BTreeNode* pop()
{
        BTreeNode *Tnode;
        StackNode *old_top;
        if(top == NULL) return NULL;
        Tnode = top->treeNode;
        old_top = top;
        top = top->next;
        delete []old_top;
        return Tnode;
}
void push_data(int data)
{
        StackData *old_top;
        old_top = top_data;
        top_data = new StackData;
        top_data->data = data;
        top_data->next = old_top;
}
int pop_data()
{
        int temp;
        StackData *old_top;
        if(top_data == NULL) return NULL;
        temp = top_data->data;
        old_top = top_data;
        top_data = top_data->next;
        delete []old_top;
        return temp;
}
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
        Form1->Memo1->Clear();
        srand(time(NULL));
        root = new BTreeNode;
        root = NULL;
        top = NULL;
        top_data = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
        int number = StrToInt(Edit2->Text);
        int range = StrToInt(Edit3->Text);
        output = "�üƦ��\ :";
        for(int i=0;i<number;i++)
        {
                root = InsertBST(root,rand()%range+1);
        }
        inorder(root);
        Memo1->Lines->Add(output);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
        int target = StrToInt(Edit1->Text);
        if(RadioButton4->Checked == true)
        {//���j
                BTreeNode *p = Search(root,target);
                if(p == NULL) Memo1->Lines->Add("(���j)�S��� :" + Edit1->Text);
                else if(p->data == target) Memo1->Lines->Add("(���j)��� :" + Edit1->Text);
        }
        if(RadioButton5->Checked == true)
        {//�D���j
                BTreeNode *node = root;
                while(node != NULL)
                {
                        if(node->data == target)
                        {
                                Memo1->Lines->Add("(�D���j)��� :" + Edit1->Text);
                                break;
                        }
                        else if(target < node->data) node = node->leftChild;
                        else if(target > node->data) node = node->rightChild;
                }
                if(node == NULL)
                {
                        Memo1->Lines->Add("(�D���j)�S��� :" + Edit1->Text);
                }
        }
}
//---------------------------------------------------------------------------




void __fastcall TForm1::Button3Click(TObject *Sender)
{
        if(RadioButton2->Checked == true)
        {//����
                if(RadioButton4->Checked == true)
                {//���j
                        output = "���ǻ��j :";
                        inorder(root);
                        Memo1->Lines->Add(output);
                }
                if(RadioButton5->Checked == true)
                {//�D���j
                        BTreeNode *node = root;
                        output = "���ǫD���j :";
                        do
                        {
                                while(node != NULL)
                                {
                                        push(node);
                                        node = node->leftChild;
                                }
                                if(top != NULL)
                                {
                                        node = pop();
                                        output += IntToStr(node->data)+ "_";
                                        node = node->rightChild;
                                }
                        }while(top!=NULL || node!=NULL);
                        Memo1->Lines->Add(output);
                }
        }
        if(RadioButton1->Checked == true)
        {//�e��
                if(RadioButton4->Checked == true)
                {//���j
                        output = "�e�ǻ��j :";
                        preorder(root);
                        Memo1->Lines->Add(output);
                }
                if(RadioButton5->Checked == true)
                {//�D���j
                        BTreeNode *node = root;
                        output = "�e�ǫD���j :";
                        do
                        {
                                while(node != NULL)
                                {
                                        output += IntToStr(node->data) + "_";
                                        push(node);
                                        node = node->leftChild;
                                }
                                if(top != NULL)
                                {
                                        node = pop();
                                        node = node->rightChild;
                                }
                        }while(top!=NULL || node!=NULL);
                        Memo1->Lines->Add(output);
                }
        }
        if(RadioButton3->Checked == true)
        {//���
                if(RadioButton4->Checked == true)
                {//���j
                        output = "��ǻ��j :";
                        postorder(root);
                        Memo1->Lines->Add(output);
                }
                if(RadioButton5->Checked == true)
                {//�D���j
                        BTreeNode *node = root;
                        output = "��ǫD���j :";
                        do
                        {
                                while(node != NULL)
                                {
                                        push_data(node->data);
                                        push(node);
                                        node = node->rightChild;
                                }
                                if(top != NULL)
                                {
                                        node = pop();
                                        node = node->leftChild;
                                }
                        }while(top!=NULL || node!=NULL);
                        while(top_data != NULL) output += IntToStr(pop_data()) + "_";
                        Memo1->Lines->Add(output);
                }
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
        int data = StrToInt(Edit4->Text);
        output = "�s�W���\ :";
        if(RadioButton4->Checked == true)
        {//���j
                root = InsertBST(root,data);
        }
        if(RadioButton5->Checked == true)
        {//�D���j
                BTreeNode *p;
                BTreeNode *node = root;
                node = p = root;
                while(node != NULL)
                {
                        p = node;
                        if(node->data == data) break;
                        else if(node->data > data) node = node->leftChild;
                        else if(node->data < data) node = node->rightChild;
                }
                if(root == NULL) root = NewBSTNode(data);
                else if(p->data > data)
                {
                        p->leftChild = NewBSTNode(data);
                }
                else if(p->data < data)
                {
                        p->rightChild = NewBSTNode(data);
                }
        }
        inorder(root);
        Memo1->Lines->Add(output);
}
//---------------------------------------------------------------------------

