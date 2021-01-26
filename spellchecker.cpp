#include "spellchecker.h"
#include "ui_spellchecker.h"

typedef struct hashEntry
{
    string palabra;
    hashEntry* sig;
}hashEntry;

class hashTable
{
public:
    int buckets;
    hashEntry** table;

    hashTable(int n)
    {
        buckets = n;
        table = new hashEntry*[buckets];
        for (int i = 0; i < buckets; ++i)
            table[i] = new hashEntry;
    }

    hashEntry* crearPalabra(string str);
    void insertarPalabra(hashEntry* ini, string str);
    int hashFunction(string str, int n);
    void llenarTabla(string file);
    bool buscarPalabra(string word);
};

hashEntry* hashTable::crearPalabra(string str)
{
    hashEntry* n;
    n = new hashEntry;
    n->palabra = str;
    n->sig = nullptr;
    return n;
}

void hashTable::insertarPalabra(hashEntry* ini, string str)
{
    if (!ini)
        ini = crearPalabra(str);
    else
    {
        hashEntry* temp;
        for (temp = ini; temp->sig; temp = temp->sig);
        temp->sig = crearPalabra(str);
    }
}


int hashTable::hashFunction(string str, int n)
{
    int key = 0;
    for (int i = 0; i < str.length(); ++i)
        key = 37 * key + str[i];
    key %= n;
    if (key < 0)
        key += n;
    return key;
}

void hashTable::llenarTabla(string file)
{
    int key = 0;
    string temp;
    ifstream dictionary;
    dictionary.open(file);
    while (getline(dictionary, temp))
    {
        key = hashFunction(temp, buckets);
        insertarPalabra(table[key], temp);
    }
    dictionary.close();
}

bool hashTable::buscarPalabra(string word)
{
    int key = hashFunction(word, buckets);
    hashEntry* temp = table[key];
    if (temp)
    {
        while (temp)
        {
            if (temp->palabra == word)
                return true;
            temp = temp->sig;
        }
    }
    return false;
}

void llenarArreglo(string file, string A[])
{
    ifstream dictionary;
    dictionary.open(file);
    string temp;
    int i = 0;
    while (getline(dictionary, temp))
    {
        A[i] = temp;
        i++;
    }
    dictionary.close();
}

bool buscarFuerzaBruta(string word, string A[])
{
    int i = 0;
    while (!A[i].empty())
    {
        if (A[i] == word)
            return true;
        i++;
    }
    return false;
}

hashTable h(pow(2,16));
string A[70000] = {};
std::vector<string> words;
std::vector<string> errors;

SpellChecker::SpellChecker(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::SpellChecker)
{
    ui->setupUi(this);

    string dictionary = "usa.txt";
    h.llenarTabla(dictionary);
    llenarArreglo(dictionary, A);

    // Connect button signal to appropriate slot
    connect(ui->bttnCheck, SIGNAL (released()), this, SLOT(BttnCheckPressed()));
}

SpellChecker::~SpellChecker()
{
    delete ui;
}

void SpellChecker::BttnCheckPressed(){
    int count = 0, i;
    string mistakes = "";
    bool bruteForce;
    QString Qfrase = ui->txtInput->toPlainText();
    std::string frase = Qfrase.toUtf8().constData();
    istringstream iss(frase);
    for(frase; iss >> frase; )
    {
        frase.resize(remove_if(frase.begin(), frase.end(),[](char x){return !isalnum(x) && !isspace(x);})-frase.begin());

        std::transform(frase.begin(), frase.end(), frase.begin(),
            [](unsigned char c){ return std::tolower(c); });
        words.push_back(frase);
    }
    clock_t tempsExecutionH = clock();
    for (i = 0; i < words.size(); ++i)
    {
        if (!h.buscarPalabra(words.at(i)))
        {
            count++;
            errors.push_back(words.at(i));
        }
    }
    tempsExecutionH = clock() - tempsExecutionH;
    ui->numErrors->display(count);
    for (i = 0; i < errors.size(); ++i)
    {
        mistakes += errors.at(i);
        mistakes += "\t";
    }
    QString Qmistakes = QString::fromStdString(mistakes);
    ui->txtOutput->setText(Qmistakes);
    clock_t tempsExecutionB = clock();
    for (i = 0; i < words.size(); ++i)
    {
        bruteForce = buscarFuerzaBruta(words.at(i), A);
    }
    tempsExecutionB = clock() - tempsExecutionB;
    frase.clear();
    words.clear();
    errors.clear();
    QString stats = "Ejecucion en ";
    stats += QString::number(((float)tempsExecutionH)/CLOCKS_PER_SEC);
    stats += " segundos usando hashing chainning.\n";
    stats += "Ejecucion en ";
    stats += QString::number(((float)tempsExecutionB)/CLOCKS_PER_SEC);
    stats += " segundos usando fuerza bruta.\n";
    stats += "Hashing chainning es ";
    stats += QString::number((((float)tempsExecutionB)/CLOCKS_PER_SEC) / (((float)tempsExecutionH)/CLOCKS_PER_SEC));
    stats += " veces mas rapido.";
    ui->txtStats->setText(stats);
}
