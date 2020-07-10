#include<iostream>
#include<fstream>
#include<cmath>
#include<cstring>
#include<ctime>
#include<cstdlib>

using namespace std;

struct jogador
{
    char nome[40];
    int time;
};


void verifica(char matriz[50][50],char matriz2[50][50],int m,int n);
int definir(char matriz[50][50],int m,int n);
void novojogo(char matriz[50][50],char matriz2[50][50],char player[40], ofstream &grava,ifstream &le,int escolha,jogador p1[20],jogador p2[20]);
int menu(int escolha,char matriz[50][50],char matriz2[50][50],char player[40],ifstream &le,ofstream &grava);
void troca (int a, int b,int c,jogador p1[10]);

void ordena (jogador p[20])
{
    int i,j,menor,nome;

    for (i=1; i<9; i=i+2)
    {
        nome=i-1;
        menor = i;
        for (j=i+2; j<10; j=j+2)
        {
            if(p[j].time<p[menor].time)
                menor = j ;
        }
        troca (i,menor,nome,p);
    }
}
void troca (int a, int b,int c,jogador p[20])
{
    int aux;
    jogador nome[40];

    aux = p[a].time;
    p[a].time=p[b].time;
    p[b].time = aux;
    strcpy(nome[0].nome,p[a-1].nome);
    strcpy(p[a-1].nome,p[b-1].nome);
    strcpy(p[b-1].nome,nome[0].nome);
}
void salvar(int tempo,char player[40],jogador p1[20],jogador p2[20],ofstream &grava,ifstream &le,int escolha)
{
    le.open("tempos.txt");

    if(!le.is_open())
    {
        cout << "FALHA AO ABRIR O ARQUIVO " << endl;
        exit(1);
    }
    for(int i=0; i<5; i=i+2)
    {
        le >> p1[i].nome;
        le >> p1[i+1].time;
    }
    for(int i=0; i<5; i=i+2)
    {
        le >> p2[i].nome;
        le >> p2[i+1].time;
    }

    le.close();

    grava.open("tempos.txt");
    if(escolha==1)
    {
        if(tempo<p1[5].time)
        {
            p1[5].time=tempo;
            strcpy(p1[4].nome,player);
            ordena(p1);
        }
    }
    else
    {
        if(escolha==2)
        {
            if(tempo<p2[5].time)
            {
                p2[5].time=tempo;
                strcpy(p2[4].nome,player);
                ordena(p2);
            }
        }
    }


    cout << endl;
    for(int i=0; i<5; i=i+2)
    {
        grava << p1[i].nome << " " << p1[i+1].time << endl;
    }
    cout << endl;

    for(int i=0; i<5; i=i+2)
    {
        grava << p2[i].nome << " " << p2[i+1].time << endl;
    }

    grava.close();
}
char recomecar(char matriz[50][50],char matriz2[50][50],char acao,int altura,int largura,char player[40],ofstream &grava,ifstream &le, jogador p1[20],jogador p2[20],int escolha)
{
    char jogarNovamente;
    do
    {
        cout <<"   ";
        for(int i=1; i<=largura; i++)
        {
            if(i<10)
            {
                cout << " ";
            }
            cout << " " << i;
        }
        cout << endl;
        int a=1;
        for(int i=1; i<=altura; i++)
        {
            cout << a++ << " ";
            if(a<=10)
            {
                cout << " ";
            }
            for(int j=1; j<=largura; j++)
            {
                if(matriz[i][j]!='*' && (matriz2[i][j]=='M' || matriz2[i][j]=='T'))
                {
                    matriz2[i][j] = 'X';
                }
                if(matriz[i][j]=='*' && matriz2[i][j]=='.')
                {
                    matriz2[i][j]='*';
                }
                if(matriz2[i][j]=='0')
                {
                    matriz[i][j]='_';
                }

                cout << "  " <<matriz2 [i][j];

            }
            cout << endl;
        }
        cout << endl;

        cout << player << ", DESEJA JOGAR NOVAMENTE ? (S PARA SIM, N PARA NAO) : " << endl;
        cin >> jogarNovamente;
        if (jogarNovamente == 's' || jogarNovamente == 'S')
        {
            srand(time(NULL));
            novojogo(matriz,matriz2,player,grava,le,escolha,p1,p2);
        }
        else
        {
            if (jogarNovamente == 'n' || jogarNovamente == 'N')
            {
                cout << "ATE A PROXIMA! " << player << endl;
                acao = 's';
                break;
            }
            else
            {
                if((jogarNovamente != 's' && jogarNovamente != 'S') || (jogarNovamente != 'n' && jogarNovamente != 'N'))
                {
                    cout << "DIGITO INVALIDO! DIGITE NOVAMENTE. (S PARA SIM, N PARA NAO)" << endl ;
                    cin >> jogarNovamente;
                    if (jogarNovamente == 's' || jogarNovamente == 'S')
                    {
                        srand(time(NULL));
                        novojogo(matriz,matriz2,player,grava,le,escolha,p1,p2);
                    }

                }

            }



        }


    }
    while (jogarNovamente != 's' && jogarNovamente != 'S' && jogarNovamente != 'n' && jogarNovamente != 'N' );
    return acao;

}

bool gameover(char matriz[50][50],char matriz2[50][50],int m,int n)
{
    int cont=0;
    for(int i=1; i<=m; i++)
    {
        for(int j=1; j<=n; j++)
        {
            if(matriz2[i][j] == '.' && matriz[i][j]!='*')
            {
                cont++;
            }

        }
    }


    if(cont==0)
    {
        return true;
    }
    else
        return false;
}
void bombas (char matriz[50][50],int altura,int largura,int qtdbomb)
{

    int mina1,mina2;
    srand(time(NULL));
    for(int i=qtdbomb; i>0; i--)
    {
        mina1 = rand()%altura + 1;
        mina2 = rand()%largura + 1;


        while(matriz[mina1][mina2]=='*')
        {
            mina1 = rand()%altura + 1;
            mina2 = rand()%largura + 1;

        }

        matriz[mina1][mina2]='*';
    }
}

void resposta (char matriz[50][50],int altura, int largura, int qtdbomb)
{

    for(int i=1; i<=altura; i++)
    {
        for(int j=1; j<=largura; j++)
        {
            matriz[i][j] = '0';
        }
    }
    bombas(matriz,altura,largura,qtdbomb);

    for(int i=1; i<=altura; i++)
    {
        for(int j=1; j<=largura; j++)
        {
            if(matriz[i][j]!='*')
                matriz[i][j] = '0' + definir(matriz,i,j);
        }
    }
}



void recursiva(char matriz[50][50],char matriz2[50][50],int m,int n)

{
    matriz[m][n]='_';

    for(int k=m-1; k<=m+1; k++)
    {
        for(int l=n-1; l<=n+1; l++)
        {
            if((k>=1 && k<=m) || (l>=1 && l<=n+1))
            {
                if (matriz2[k][l] != 'T' && matriz2[k][l] != 'M')
                {
                    matriz2[k][l]=matriz[k][l];


                    if(k!=m || l!=n)
                    {
                        if(matriz[k][l]=='0')
                        {
                            matriz[k][l]='_';
                            recursiva(matriz,matriz2,k,l);

                        }
                    }
                }
            }

        }
    }



}

void verifica(char matriz[50][50],char matriz2[50][50],int m,int n)
{

    if(matriz[m][n]>'0')
    {
        matriz2[m][n] = matriz[m][n];
    }

    if(matriz[m][n]=='0')
    {
        matriz2[m][n]=matriz[m][n];
        recursiva(matriz,matriz2,m,n);
    }
}

int definir(char matriz[50][50],int m,int n)
{
    int cont=0;
    for(int i=m-1; i<=m+1; i++)
    {
        for(int j=n-1; j<=n+1; j++)
        {
            if(matriz[i][j]=='*')
            {
                cont++;
            }
        }
    }
    return cont;

}

void preencheMatriz (char matriz2[50][50],int altura, int largura)
{
    for(int i=1; i<=altura; i++)
    {
        for(int j=1; j<=largura; j++)
        {
            matriz2[i][j]='.';
        }
    }
}

void mostra (char matriz[50][50], char matriz2[50][50],int altura,int largura,char player[40],int qtdbomb,ofstream &grava,ifstream &le,jogador p1[20],jogador p2[20],int escolha)
{

    char acao;
    int a, m, n, q=qtdbomb,Tinicial,Tfinal,tempototal=0;

    Tinicial=time(NULL);
    while(acao != 's' && acao != 'S')
    {


        if(gameover(matriz,matriz2,altura,largura))
        {

            cout << "PARABENS " << player << endl;
            cout << "VOCÊ VENCEU" << endl;
            cout << " SEU TEMPO FOI: " << tempototal << " SEGUNDOS " <<  endl;
            if(escolha==1 || escolha==2)
            {
                salvar(tempototal,player,p1,p2,grava,le,escolha);
            }
            acao=recomecar(matriz,matriz2,acao,altura,largura,player,grava,le,p1,p2,escolha);
            break;
        }


        /*apenas para exibir a matriz com a resposta*/
        a=1;
        for(int i=1; i<=altura; i++)
        {
            cout << a++ << "  ";
            for(int j=1; j<=largura; j++)


            {
                cout << matriz [i][j];

            }
            cout << endl;
        }
        cout << endl;

        Tfinal=time(NULL);
        tempototal=Tfinal-Tinicial;
        cout << "TEMPO DE JOGO: " << tempototal<< " SEGUNDOS " <<  endl;

        cout <<"   ";
        for(int i=1; i<=largura; i++)
        {
            if(i<10)
            {
                cout << " ";
            }
            cout << " " << i;
        }
        cout << endl;
        a=1;
        for(int i=1; i<=altura; i++)
        {
            cout << a++ << " ";
            if(a<=10)
            {
                cout << " ";
            }
            for(int j=1; j<=largura; j++)
            {

                cout << "  " <<matriz2 [i][j];

            }
            cout << endl;
        }
        cout << "MINAS A MARCAR: " << q << endl;
        cout << "D -> " << "DESCOBRIR QUADRADO " << endl;
        cout << "M -> " << "MARCAR MINA " << endl;
        cout << "T -> " << "TALVEZ MINA " << endl;
        cout << "L -> " << "LIMPAR MARCACAO " << endl;
        cout << "S -> " << "SAIR " << endl;
        cout << "DIGITE A OPCAO SEGUIDA DAS COORDENADAS DO QUADRO " << endl;
        cin >> acao;
        if (acao == 's' || acao == 'S')
        {

            break;
        }
        cin >> m >> n;
        Tfinal = time(NULL);
        tempototal=Tfinal-Tinicial;
        if(acao=='D' || acao =='d')
        {
            if(matriz[m][n]=='*')
            {
                cout << " QUE PENA, " << player << endl;
                cout << " VOCÊ PERDEU! " << endl;
                cout << " SEU TEMPO FOI: " << tempototal << " SEGUNDOS " <<  endl;
                acao=recomecar(matriz,matriz2,acao,altura,largura,player,grava,le,p1,p2,escolha);
                break;
            }
            else
            {
                verifica(matriz,matriz2,m,n);
            }
        }
        else if ((acao == 'M' || acao == 'm') && matriz2[m][n]=='.')
        {
            matriz2[m][n] = 'M' ;
            q-- ;
        }
        else if ((acao == 't' || acao == 'T') && matriz2[m][n]=='.')
        {
            matriz2[m][n] = '?' ;
        }
        else if ((acao == 'l' || acao == 'L') && (matriz2[m][n]=='M') ||  (matriz2[m][n] == 'T')  )
        {
            if (matriz2[m][n] == 'M')
            {
                q++ ;
            }
            matriz2[m][n] = '.' ;
        }



    }
}

void novojogo(char matriz[50][50],char matriz2[50][50],char player[40],ofstream &grava,ifstream &le,int escolha,jogador p1[20],jogador p2[20])
{
    int altura,largura,qtdbomb,m,n;



    if(escolha==1)
    {
        altura = 8;
        largura = 8;
        qtdbomb=10;

        preencheMatriz(matriz2,altura,largura);
        resposta(matriz,altura,largura,qtdbomb);
        mostra (matriz,matriz2,altura,largura,player,qtdbomb,grava,le,p1,p2,escolha);

    }
    else
    {
        if(escolha==2)
        {
            altura = 16;
            largura = 16;
            qtdbomb=40;

            preencheMatriz(matriz2,altura,largura);
            resposta(matriz,altura,largura,qtdbomb);
            mostra (matriz,matriz2,altura,largura,player,qtdbomb,grava,le,p1,p2,escolha);
        }
        else
        {
            if(escolha==3)
            {
                do
                {
                    cout << "Digite a dimensão da matriz : " << endl;
                    cin >> altura >> largura ;

                    if (altura > 40 || largura > 40)
                    {
                        cout << "Dimensão da matriz inválida! Digite novamente" << endl;
                        cout << "Digite a dimensão da matriz : " << endl;
                        cin >> altura >> largura ;
                    }
                }
                while (altura >  40 || largura > 40);

                do
                {
                    cout << player << " Digite a quantidade de bombas desejadas : " << endl;
                    cin >> qtdbomb ;

                    if (qtdbomb > ((altura*largura)-10))
                    {
                        cout << "Quantidade de bombas invalida! Digite novamente" << endl;
                        cout << "Digite a quantidade de bombas desejadas : " << endl;
                        cin >> qtdbomb ;

                    }
                }
                while (qtdbomb > ((altura*largura)-10));



                preencheMatriz(matriz2,altura,largura);
                resposta(matriz,altura,largura,qtdbomb);
                mostra (matriz,matriz2,altura,largura,player,qtdbomb,grava,le,p1,p2,escolha);
            }


            else if (escolha == 4)
            {
                menu(escolha,matriz,matriz2,player,le,grava);
            }
        }
    }



}


int menu(int escolha,char matriz[50][50],char matriz2[50][50],char player[40],ifstream &le,ofstream &grava)
{
    jogador p1[20];
    jogador p2[20];
    int difi;
    do
    {

        cout << endl;
        cout << endl;
        cout << endl;
        cout << " " << player << ",";
        cout << " ESCOLHA UMA OPCAO " << endl;
        cout << " 1 - NOVO JOGO " << endl;
        cout << " 2 - MELHORES TEMPOS " << endl;
        cout << " 3 - SAIR " << endl;
        cin >> escolha;

        if (escolha!=3 && escolha!=2 && escolha!=1)
        {
            cout << "OPCAO INVALIDA! DIGITE NOVAMENTE" << endl << endl;
            cout << " ESCOLHA UMA OPCAO " << endl;
            cout << " 1 - NOVO JOGO " << endl;
            cout << " 2 - MELHORES TEMPOS " << endl;
            cout << " 3 - SAIR " << endl;
            cin >> escolha;
        }

    }
    while(escolha!=3 && escolha!=2 && escolha!=1);

    if(escolha == 1)
    {
        do
        {
            cout << player <<", ESCOLHA UMA OPCAO " << endl;
            cout << endl;
            cout << endl;
            cout << endl;
            cout << " 1 - INICIANTE " << endl;
            cout << " 2 - INTERMEDIARIO " << endl;
            cout << " 3 - PERSONALIZADO " << endl;
            cout << " 4 - VOLTAR " << endl;
            cin >> difi;
            cin.ignore();
            if (difi != 1 && difi != 2 && difi != 3 && difi != 4)
            {
                cout << "DIGITO INVALIDO. DIGITE NOVAMENTE.";
                cout << endl;
                cout << endl;
                cout << endl;
                cout << " 1 - INICIANTE " << endl;
                cout << " 2 - INTERMEDIARIO " << endl;
                cout << " 3 - PERSONALIZADO " << endl;
                cout << " 4 - VOLTAR " << endl;
                cin >> difi;
                cin.ignore();

            }
        }
        while (difi != 1 && difi != 2 && difi != 3 && difi != 4);

        if(difi==1)
        {
            novojogo(matriz,matriz2,player,grava,le,difi,p1,p2);

        }
        else
        {
            if(difi==2)
            {
                novojogo(matriz,matriz2,player,grava,le,difi,p1,p2);

            }
            else
            {
                if(difi==3)
                {
                    novojogo(matriz,matriz2,player,grava,le,difi,p1,p2);

                }
                else
                {
                    if(difi==4)
                    {
                        menu(escolha,matriz,matriz2,player,le,grava);
                    }
                }
            }
        }



    }
    else
    {
        if(escolha==2)
        {
            ifstream le("tempos.txt");


            if(!le.is_open())
            {
                cout << "FALHA AO ABRIR O ARQUIVO " << endl;
                exit(1);
            }
            cout << " MODO INICIANTE " << endl;
            le>> p1[0].nome >> p1[1].time;
            cout << p1[0].nome << " " << p1[1].time << endl;
            le>> p1[2].nome >> p1[3].time;
            cout << p1[2].nome << " " << p1[3].time << endl;
            le>> p1[4].nome >> p1[5].time;
            cout << p1[4].nome << " " << p1[5].time << endl;
            cout << " MODO INTERMEDIÁRIO " <<endl;
            le>> p1[6].nome >> p1[7].time;
            cout << p1[6].nome << " " << p1[7].time << endl;
            le>> p1[8].nome >> p1[9].time;
            cout << p1[8].nome << " " << p1[9].time << endl;
            le>> p1[10].nome >> p1[11].time;
            cout << p1[10].nome << " " << p1[11].time << endl;

            le.close();

        }
        else if (escolha == 3)
        {
            cout << "ATE A PROXIMA!" << endl;
        }

    }

}
int main()
{

    int escolha;
    char matriz[50][50],matriz2[50][50],player[40],acao;
    setlocale(LC_ALL,"Portuguese");
    srand(time(NULL));

    ifstream le;
    ofstream grava;

    cout << "      BEM-VINDO " << endl;
    cout << "         AO " << endl;
    cout << "    CAMPO MINADO " << endl;
    cout << "  OLA, COMO SE CHAMA? " << endl;
    cin.getline(player,40);
    cout << endl;

    menu(escolha,matriz,matriz2,player,le,grava);

}
