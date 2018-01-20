/* Assignment: 2
Author1: Victor Martinov,
ID: 307835249
Author2: Tsur Avneri,
ID: 308134485
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define numOfBitsInByte 8

typedef struct HuffmanTree {
    int freq;
    char ch;
    struct HuffmanTree* parent;
    struct HuffmanTree* left;
    struct HuffmanTree* right;
} HuffmanTree;
typedef struct LinkedListNode {
    HuffmanTree *tree;
    struct LinkedListNode *next;
} LinkedListNode;

int menu ();
int menu2();
HuffmanTree * case1();
HuffmanTree * case2();
LinkedListNode* insertbypriority (LinkedListNode * curr,LinkedListNode * head);
HuffmanTree* buildtree(LinkedListNode * list);
char* encodetext(HuffmanTree *tree);
int gethuffcode(HuffmanTree *tree,char letter,int index,int found,char binarystr[9]);
void BitEncode(char* str);
void decodeStr(HuffmanTree* tree,HuffmanTree* orig,char *binarystr,int index);
void BitDecode (HuffmanTree *huff);
void inOrderPrint(HuffmanTree * huff);
void postFree(HuffmanTree * huff);

int menu ()//prints the main menu
{
    int choice;
    do {
        printf("1. Create a Huffman tree from text\n");
        printf("2. Deserialize a Huffman tree\n");
        printf("3. Exit\n");
        printf("Enter you choice: ");
        scanf ("%d",&choice);
    }while (choice < 0 || choice >3);
    return choice;
}
int menu2()//prints the 2nd menu
{
    int choice;
    do
    {
        printf("1. Encode text using the Huffman Tree\n");
        printf("2. Bit-encode text using the Huffman Tree\n");
        printf("3. Decode using the Huffman Tree\n");
        printf("4. Bit-Decode using the Huffman Tree\n");
        printf("5. Serialize the Huffman Tree\n");
        printf("6. Free the in-memory Huffman Tree\n");
        printf("Enter you choice: ");
        scanf ("%d",&choice);
    }while(choice < 0 || choice >6);
    return choice;
}
HuffmanTree * case1() //option 1 of the main menu,returns a huffmantree from a given string
{
    int occ[256]= {0},i=0;
    char str [10000];
    HuffmanTree * huff;
    LinkedListNode * curr, * head=0;
    while(getchar()!='\n');
    gets(str);
    for (i=0; i < 10000;i++)
    {
        occ[(int)str[i]]++; // adds +1 the the occ cell of the ASCII value of the char currently shown in string.
        if(str[i]=='@')
            break;
    }
    for (i=0 ; i<256;i++)
    {
        if(occ[i]!=0)
        {
            curr = (LinkedListNode *)malloc(sizeof(LinkedListNode));
            curr->tree = (HuffmanTree *)malloc(sizeof(HuffmanTree));
            curr->next=NULL;
            curr->tree->left=NULL;
            curr->tree->right=NULL;
            curr->tree->parent=NULL;
            curr->tree->ch = i;
            curr->tree->freq = occ[i];
            head = insertbypriority(curr , head); // inserts curr node to the linked list by the priorities requested.
        }
    }
    return (huff = buildtree(head)); // builds the tree and returns the head.
}
HuffmanTree * case2()//option 2 for of the main menu,returns a huffmantree from a given string in the format- "char:occurences" and so on.
{
    char str2[16];
    int  i=0;
    HuffmanTree * huff;
    LinkedListNode * curr, * head=0;
    while(getchar()!='\n');
    do{
        gets(str2);
        if(str2[0]!='\0')
        {
            curr = (LinkedListNode *)malloc(sizeof(LinkedListNode));
            curr->tree = (HuffmanTree *)malloc(sizeof(HuffmanTree));
            curr->next=NULL;
            curr->tree->left=NULL;
            curr->tree->right=NULL;
            curr->tree->parent=NULL;
            curr->tree->ch=*str2;
            curr->tree->freq=str2[2]-'0';
            i=3;
            while(str2[i]!='\0')
            {
                curr->tree->freq=curr->tree->freq*10 + (str2[i]-'0'); //builds the frequency to be the number the user typed.
                i++;
            }
            head =     insertbypriority(curr,head); // inserts curr node to the linked list by the priorities requested.
        }
    }while(str2[0]!='\0');
    return(huff = buildtree(head)); // builds the tree and returns the head.
}
LinkedListNode* insertbypriority (LinkedListNode * curr ,LinkedListNode * head)//inserts a given node into the list by priorities requested.
{
    LinkedListNode * tmp ,* tmpnext;
    if (head)
        tmp = head;
    else
        return (head=curr); // empty list - return head set to be curr.
    if (head->next)
        tmpnext = head->next;
    else if (curr->tree->freq > head->tree->freq) //only one link in list. insert curr by priorities.
    {
        head-> next = curr;
        curr->next =0;
        return head;
    }
    else if(curr->tree->freq < head->tree->freq) //only one link in list. insert curr by priorities.
    {
        curr-> next = head;
        head=curr;
        return head;
    }
    else if(curr->tree->freq == head->tree->freq) //only one link in list. insert curr by priorities.
    {
        if(curr-> tree->ch < head-> tree->ch)
        {
            curr-> next = head;
            head=curr;
            return head;
        }
        else if(curr-> tree->ch > head-> tree->ch)
        {
            head-> next = curr;
            curr->next=0;
            return head;
        }
    }
    while (tmp && curr->tree->freq >= tmp->tree->freq) //tmp exists and curr freq bigger or equal to tmp freq.
    {
        if(tmpnext && curr->tree->freq > tmpnext->tree->freq) // current freq bigger than tmpnext freq , move on
        {
            tmp = tmp->next;
            tmpnext = tmpnext->next;
            continue;
        }
        if (tmpnext && curr->tree->freq < tmpnext->tree->freq) // current freq smaller than tmpnext freq. insert between tmp and tmpnext
        {
            curr->next= tmpnext;
            tmp->next = curr;
            return head;
        }
        while (tmpnext && curr->tree->freq == tmpnext->tree->freq && (curr->tree->ch > tmpnext->tree->ch))// curr freq equals tmpnext freq , but char value larger.
        {
            tmp = tmp->next;
            tmpnext = tmpnext->next;
        }
        if (curr->tree->freq == tmp->tree->freq && curr-> tree->ch < tmp-> tree->ch) // curr char value smaller than next. insert curr between tmp and tmpnext.
        {
            curr->next = tmp;
            head=curr;
            return head;
        }
        if(tmpnext && curr->tree->freq==tmpnext->tree->freq && curr->tree->ch < tmpnext->tree->ch)// tmpnext exists and curr freq equals to tmpnext freq but curr char value smaller than tmpnext.
        {
            curr->next=tmpnext;
            tmp->next=curr;
            return head;
        }
        if (tmpnext && curr->tree->freq < tmpnext->tree->freq) // current freq smaller than tmpnext freq. insert between tmp and tmpnext
        {
            curr->next= tmpnext;
            tmp->next = curr;
            return head;
        }
        if (!tmpnext)// next is null. insert curr in the end.
        {
            tmp-> next=curr;
            curr->next = 0;
            return head;
        }
        if (curr-> tree->ch < tmp-> tree->ch) // curr char value smaller than next. insert curr between tmp and tmpnext.
        {
            curr->next = tmp;
            head=curr;
            return head;
        }
        if (curr-> tree->ch < tmpnext-> tree->ch) // curr char value smaller than tmpnext. insert curr between tmp and tmpnext.
        {
            curr->next = tmpnext;
            tmp -> next=curr;
            return head;
        }
    }
    if (tmp && curr->tree->freq < tmp->tree->freq) //tmp exists and curr freq smaller than tmp freq. insert curr before tmp.
    {
        curr-> next = tmp;
        return head = curr;
    }
    return head;
}
HuffmanTree* buildtree(LinkedListNode * list)//builds a tree from the list,free the list and returns the root of the tree
{
    LinkedListNode * newnode;
    HuffmanTree * temptree = (HuffmanTree *)malloc(sizeof(HuffmanTree)),*huff;
    temptree->left=NULL;
    temptree->right=NULL;
    temptree->parent=NULL;
    if (!list)
        return NULL;
    while (list->next->next) // more than two links in the list.
    {
        huff=temptree;
        huff->left = list->tree;
        huff->left->parent=temptree;
        huff->right = list->next->tree;
        huff->right->parent=temptree;
        huff->freq=list->tree->freq + list->next->tree->freq;
        huff->ch = list->tree->ch;
        newnode= (LinkedListNode *)malloc(sizeof(LinkedListNode));
        newnode->tree=huff;
        list=insertbypriority(newnode,list); // inserts curr node to the linked list by the priorities requested.
        newnode=list->next->next;
        free(list->next); //frees the second node of the list->
        list->next=0;
        free(list); //frees the first node of the list->
        list=newnode;
        temptree = (HuffmanTree *)malloc(sizeof(HuffmanTree));
        temptree->left=NULL;
        temptree->right=NULL;
        temptree->parent=NULL;
    }
    if(list && !list->next) // only one link in list.
    {
        free(temptree);
        temptree=list->tree;
        return temptree;
    }
    // builds final tree from two remaining nodes in the list.
    huff=temptree;
    huff->left = list->tree;
    huff->left->parent=temptree;
    huff->right = list->next->tree;
    huff->right->parent=temptree;
    huff->freq=list->tree->freq + list->next->tree->freq;
    huff->ch = list->tree->ch;
    free(list->next); //frees the second node of the list->
    list->next=0;
    free(list); //frees the first node of the list->
    list=0;
    return huff;
}
char* encodetext(HuffmanTree *tree)//calculates the binary representation of a string from the tree and returns it
{
    int i,j,length;
    char *ansStr=0,str[10001],partialroad[9]={0},zero[2]={'0','\0'};
    char *binarystr=(char*)calloc(80000,sizeof(char));//2^8 different possible values for single char therefore the max bytes the string can take is 80k
    gets(str);
    for(i=0;str[i]!='\0' && i<strlen(str);i++)
    {
        gethuffcode(tree,str[i],0,0,partialroad);
        strcat(binarystr,partialroad);
        for(j=0;j<numOfBitsInByte;j++)
            partialroad[j]=0;
    }
    length=numOfBitsInByte-(strlen(binarystr)%numOfBitsInByte);
    if(length!=numOfBitsInByte)
        for(i=0;i<length;i++)
            strcat(binarystr,zero);
    ansStr=(char*)calloc(strlen(binarystr)+1,sizeof(char));
    strcpy(ansStr,binarystr);
    free(binarystr);//freeing excess memory
    return ansStr;
}
int gethuffcode(HuffmanTree *tree,char letter,int index,int found,char binarystr[9])//changes binarystr and calculates the path to each char in the tree
{
    if(tree->parent!=NULL && tree->ch==letter &&tree->left==NULL && tree->right==NULL)
        return 1;//the char was found
    if(tree->left)
        found=gethuffcode(tree->left,letter,index+1,0,binarystr);
    if(found)
    {
        binarystr[index]='0';
        return found;
    }
    if(tree->right)
        found=gethuffcode(tree->right,letter,index+1,0,binarystr);
    if(found)
    {
        binarystr[index]='1';
        return found;
    }
    return 0;//the char wasn't found
}
void BitEncode(char* str)//calculates the ASCII value of a binary string and prints it, letter by letter.
{
    int sum = 0;
    int i=0 ,j=0;
    int length = strlen(str);
    for (i=0 ; i < length && str[i]!= '\0'; i++)
    {
        for (j=numOfBitsInByte-1;j>=0 ; j--)
        {
            if (str[i]=='1')
                sum = sum + (int)pow(2,j); // adds the current sum to the decimal value of the bit in index j (if it's a '1')
            i++;
        }
        i--;
        printf("%c", (char)sum);
        sum=0;
    }
    printf("\n");
}
void decodeStr(HuffmanTree* tree,HuffmanTree* orig , char *str , int index) // decode input bits into letters in the huffman tree.
{
    if (!(tree->left) && !(tree->right)) // if reached leaf
    {
        printf("%c" , tree->ch);
        if (tree->ch== '@') // reached @
            return;
        if (index<strlen(str)) // haven't reached end of input string yet
            decodeStr(orig,orig,str,index); // start decoding from top of tree again and check another path.
    }
    if(tree->left!=NULL && str[index]=='0')
    {
        decodeStr(tree->left,orig,str,index+1); // go to left path.
    }
    else if(tree->right!=NULL && str[index]=='1')
    {
        decodeStr(tree->right,orig,str,index+1); // go to right path.
    }
}
void BitDecode (HuffmanTree *huff) //bit decode input characters into letters in the huffman tree.
{
    char strtobitdec [10001];//, binaryNumber [80000];
    char* binaryNumber = (char*)calloc(80000,sizeof(char));
    int quotient,i,j=0,k=0;
    char temp;
    gets(strtobitdec);
    for (i=0; i < strlen(strtobitdec) ;i++)
    {
        quotient = strtobitdec[i];
        while(j<numOfBitsInByte && k<80000)
        {
            if(quotient % 2)
                binaryNumber[k++]='1';
            else
                binaryNumber[k++]='0';
            quotient = quotient / 2;
            j++;
            // decoding the character to 8 bit reversed order binary number.
        }
        for(j=0;j<4;j++)
        {
            // reverses the last 8 bits of the string to get correct binary number value.
            temp=binaryNumber[strlen(binaryNumber)-1-j];
            binaryNumber[strlen(binaryNumber)-1-j]=binaryNumber[strlen(binaryNumber)-numOfBitsInByte+j];
            binaryNumber[strlen(binaryNumber)-numOfBitsInByte+j]=temp;
        }
        j=0;
    }
    decodeStr(huff,huff,binaryNumber,0);
    free(binaryNumber);
}
void inOrderPrint(HuffmanTree * huff)  // inOrder Huffman Tree printing (huffman tree serializing).
{
    if (huff->left != NULL && huff->right != NULL)
    {
        if (huff->left)
            inOrderPrint(huff->left);
        if(huff->right)
            inOrderPrint(huff->right);
    }
    else
    {
        printf("%c", huff->ch);
        printf(":");
        printf("%d\n", huff->freq);
    }
    return;
}
void postFree(HuffmanTree * huff)  // postorder Huffman Tree freeing.
{
    while(huff->left || huff->right)
    {
        if(huff->left)
        {
            if(!huff->left->left && !huff->left->right) // son's left and right are both NULL
            {
                free(huff->left); // free left son
                huff->left=0;
            }
            else
                postFree(huff->left);
        }
        if(huff->right)
        {
            if(!huff->right->left && !huff->right->right) // son's left and right are both NULL
            {
                free(huff->right); // free right son
                huff->right=0;
            }
            else
                postFree(huff->right);
        }
    }
}
int main(void)
{
    HuffmanTree * huff;
    int option=0 ,option2=0;
    char str [10000],*tempstr;
    do
    {
        option = menu();
        switch (option)
        {
        case 1:
            huff=case1();
            break;
        case 2:
            huff=case2();
            break;
        case 3:
            break;
        }
        if(option!=3)
        {
            printf("*** Huffman Tree in-memory ***\n");
            do
            {
                option2=menu2();
                while(getchar()!='\n');
                switch(option2)
                {
                case 1:
                    tempstr=encodetext(huff);
                    printf("%s\n",tempstr);
                        free(tempstr);
                        tempstr = NULL;
                    break;
                case 2:
                    tempstr=encodetext(huff);
                    BitEncode(tempstr);
                        free(tempstr);
                        tempstr = NULL;
                    break;
                case 3:
                    gets(str);
                    decodeStr(huff,huff,str,0);
                    printf("\n");
                    break;
                case 4:
                    BitDecode(huff);
                    printf("\n");
                    break;
                case 5:
                    inOrderPrint(huff);
                    printf("\n");
                    break;
                case 6:
                    postFree(huff);
                    free(huff);
                    huff=0;
                    break;
                }
            }while(option2!=6);
            printf("--- No Huffman Tree ---\n");
        }
    }while(option!=3);
    return 0;
}
