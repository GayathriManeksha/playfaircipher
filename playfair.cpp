// C++ program to implement Playfair Cipher

#include <bits/stdc++.h>
using namespace std;

// Function to convert the string to lowercase
void toLowerCase(char plain[], int ps)
{
    int i;
    for (i = 0; i < ps; i++)
    {
        if (plain[i] > 64 && plain[i] < 91)
            plain[i] += 32;
    }
}

// Function to remove all spaces in a string
int removenonalphabets(char *plain, int ps)
{
    int i, count = 0;
    for (i = 0; i < ps; i++)
        // if (plain[i] != ' ')
        if ((plain[i] >= 'A' && plain[i] <= 'Z') || (plain[i] >= 'a' && plain[i] <= 'z'))
            plain[count++] = plain[i];
    plain[count] = '\0';
    // cout << "plain: " << plain << endl;
    return count;
}

// Function to generate the 5x5 key square
void generateKeyTable(char key[], int ks, char keyT[5][5])
{
    int i, j, k, flag = 0;

    // a 26 character hashmap
    // to store count of the alphabet
    int dicty[26] = {0};
    for (i = 0; i < ks; i++)
    {
        if (key[i] != 'j')
            dicty[key[i] - 97] = 2;
    }

    dicty['j' - 97] = 1;

    i = 0;
    j = 0;

    for (k = 0; k < ks; k++)
    {
        if (dicty[key[k] - 97] == 2)
        {
            dicty[key[k] - 97] -= 1;
            keyT[i][j] = key[k];
            j++;
            if (j == 5)
            {
                i++;
                j = 0;
            }
        }
    }

    for (k = 0; k < 26; k++)
    {
        if (dicty[k] == 0)
        {
            keyT[i][j] = (char)(k + 97);
            j++;
            if (j == 5)
            {
                i++;
                j = 0;
            }
        }
    }

    // keybox

    cout << "keybox" << endl;

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cout << keyT[i][j] << " ";
        }
        cout << endl;
    }
}

// Function to search for the characters of a digraph
// in the key square and return their position
void search(char keyT[5][5], char a, char b, int arr[])
{
    int i, j;

    if (a == 'j')
        a = 'i';
    else if (b == 'j')
        b = 'i';

    for (i = 0; i < 5; i++)
    {

        for (j = 0; j < 5; j++)
        {

            if (keyT[i][j] == a)
            {
                arr[0] = i;
                arr[1] = j;
            }
            else if (keyT[i][j] == b)
            {
                arr[2] = i;
                arr[3] = j;
            }
        }
    }
}

// Function to find the modulus with 5
int mod5(int a) { return (a % 5); }

// Function to make the plain text length to be even
int prepare(char str[], int ptrs)
{
    if (ptrs % 2 != 0)
    {
        str[ptrs++] = 'z';
        str[ptrs] = '\0';
    }
    return ptrs;
}

// Function for performing the encryption
void encrypt(char str[], char keyT[5][5], int ps, char cipher[])
{
    int i, a[4], j = 0;
    char x, y;
    for (i = 0; i < ps;)
    {
        if (str[i] == str[i + 1])
        {
            x = str[i];
            y = 'x';
            i++;
        }
        else if (str[i + 1] == '\0')
        {
            x = str[i];
            y = 'z';
            i++;
        }
        else
        {
            x = str[i];
            y = str[i + 1];
            i += 2;
        }

        search(keyT, x, y, a);

        if (a[0] == a[2])
        {
            cipher[j] = keyT[a[0]][mod5(a[1] + 1)];
            cipher[j + 1] = keyT[a[0]][mod5(a[3] + 1)];
        }
        else if (a[1] == a[3])
        {
            cipher[j] = keyT[mod5(a[0] + 1)][a[1]];
            cipher[j + 1] = keyT[mod5(a[2] + 1)][a[1]];
        }
        else
        {
            cipher[j] = keyT[a[0]][a[3]];
            cipher[j + 1] = keyT[a[2]][a[1]];
        }
        j += 2;
    }
    cipher[j] = '\0';
}

// Function to encrypt using Playfair Cipher
void encryptByPlayfairCipher(char str[], char key[])
{
    char ks, keyT[5][5];
    int ps;

    // Key
    ks = strlen(key);
    ks = removenonalphabets(key, ks);
    toLowerCase(key, ks);

    // Plaintext
    ps = strlen(str);
    // cout << "str" << str << endl;
    // cout << "ps " << ps << endl;
    toLowerCase(str, ps);
    ps = removenonalphabets(str, ps);

    // ps = prepare(str, ps);

    generateKeyTable(key, ks, keyT);

    char cipher[4000];
    encrypt(str, keyT, ps, cipher);
    // cout << "CIPHER: " << cipher << endl;
    int l = strlen(cipher);

    FILE *fp = fopen("ciphertext.txt", "w");
    fwrite(cipher, sizeof(char), l, fp);
}

// Driver code
int main()
{
    char key[30];
    char s[10000];
    // string str = "";

    // Key to be encrypted
    strcpy(key, "stijostephen");
    // cout << "Key text: " << key << "\n";

    // Plaintext to be encrypted
    // strcpy(str, "communicate");
    // cout << "Plain text: " << str << "\n";

    // read from a file

    // ifstream in("plaintext.txt");
    FILE *fp = fopen("plaintext.txt", "r");
    int i = 0;
    while (!feof(fp))
    {
        char text;
        text = getc(fp);
        s[i] = text;
        i++;
    }
    s[i] = '\0';
    // cout << "hey " << s << endl;
    // encrypt using Playfair Cipher

    encryptByPlayfairCipher(s, key);

    // cout << "Cipher text: " << s << "\n";

    return 0;
}
