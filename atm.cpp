#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstring>
using namespace std;

ifstream findist ("date.txt");/// fis. cu distante intre ATM
ifstream finorar ("orar.txt");/// fis. cu orarul ATM
ifstream fincard ("card.txt");///fis. cu datele din card si cont

const int inf=1e9;


int G[10][10],n,Suma; ///graf de costuri intre ATM
int atm_vizitat[4];
char C[10];
int Orar[5][24],ora,ora_f;
int ora_start=11,minut_start=30;
int ora_fin=14,min_fin=00;

struct date_card {
   float Fee,Zi,Sold,de_scos;
};

date_card Card[4];

void af (void)
{
   int i,j;
   for(i=0;i<=n;i++)
   {
      for (j=0;j<=n;j++)
         if (G[i][j]==inf)
            cout<<"- ";
            else
               cout<<G[i][j]<<" ";
      cout<<"\n";
   }
   cout<<'\n';
}

void af_orar (void)
{
   int i,j;
   cout<<"ORAR\n";
   for (i=0;i<24;i++)
      cout<<setw(2)<<i<<' ';
   cout<<'\n';
   for (i=1;i<=n;i++)
   {
      for (j=0;j<24;j++)
         cout<<setw(2)<<Orar[i][j]<<' ';
      cout<<'\n';
   }
   cout<<'\n';
}

void af_card(void)
{
   cout<<"\nCarduri\n";
for (int i=1;i<4;i++)
      cout<<"\n"<<Card[i].Fee<<' '<<Card[i].Zi<<' '<<Card[i].Sold<<' '<<Card[i].de_scos;
   cout<<'\n';
}

void citire_atm (void)
{
   int i,j,x,y,co;
   findist>>n>>Suma;
   ///citire costuri graf
   while (findist>>x>>y>>co)
      G[x][y]=co;
   for (i=0;i<=n;i++)
      for (j=0;j<=n;j++)
         if (i!=j && G[i][j]==0)
            G[i][j]=inf;
   findist.close();
}

void citire_orar (void)
{
   ///citire orar
   int i,j,x,y,z;
   for (j=1;j<=n;j++)
   {
      ///x-ATM, y-Ora desc, z-ora inchide
      finorar>>x;
      finorar.getline(C,20);
      y=(C[1]-'0')*10+(C[2]-'0');
      z=(C[7]-'0')*10+(C[8]-'0');
      if (z<=y)
      {
         for (i=y;i<24;i++)
            Orar[x][i]=1;///ore pana la 0
         for (i=0;i<z;i++)
            Orar[x][i]=1; ///ore de la 0
      }
      else
         for (i=y;i<z;i++)
            Orar[x][i]=1;
   }
   finorar.close();
}

void citire_card (void)
{
   int i,j;
   date_card t;
   for (i=1;i<=n;i++)
      fincard>>Card[i].Fee>>Card[i].Zi>>Card[i].Sold;
   for (i=1;i<3;i++)
      for (j=i+1;j<=3;j++)
         if (Card[i].Fee>Card[j].Fee)
         {
            t=Card[i];
            Card[i]=Card[j];
            Card[j]=t;
            //swap (Card[i],Card[j]);
         }
   fincard.close();
}

void floyd (void)
{
   int i,j,k;
   for (k=0;k<=n;k++)
      for (i=0;i<=n;i++)
         for (j=0;j<=n;j++)
            if (G[i][j]>G[i][k]+G[k][j])
               G[i][j]=G[i][k]+G[k][j];
}

void cum_scot(void)
{
   int s,i=1;
   s=Suma;
   while (s>0)
   {
      if (s<=Card[i].Sold)
      {
         Card[i].de_scos=s;
         s=0;
      }
         else
         {
            s=s-Card[i].Sold;
            Card[i].de_scos=Card[i].Sold;
         }
      i++;
   }
}

int atm_deschis(int atm, int ora)
{
   return Orar[atm][ora/60];
}

void scoate (void)
{
   cum_scot();
   int i,j,atm=0,atmv,distanta,card_in_use=0;
   while (Suma>0 && ora<=ora_f)
   {
      distanta=1e9;
      for (i=1;i<=n;i++)
         if (atm_vizitat[i]==0 && G[atm][i]<distanta && atm_deschis(i,ora+G[atm][i]))
         {
            distanta=G[atm][i];
            atmv=i;
         }
            for (i=1;i<=3;i++)
               if (Card[i].de_scos>0)
               {
                  cout<<"ATM: "<<atmv<<" Card: "<<i<<" Suma:"<< min(Card[i].Zi,Card[i].de_scos)<<'\n';
                  Suma=Suma-min(Card[i].Zi,Card[i].de_scos);
                  Card[i].de_scos=Card[i].de_scos-min(Card[i].Zi,Card[i].de_scos);
               }
         atm_vizitat[atmv]=1;///marchez ATM vizitat
         ora=ora+G[atm][atmv];///modific ora dupa deplasarea la atmv
         atm=atmv;
   }
}


int main (void)
{
   ora=ora_start*60+minut_start;
   ora_f=ora_fin*60+min_fin;
   citire_atm();
   citire_orar();
   citire_card();
   //floyd();
  // scoate();
   cout<<Orar;
   return 0;
}
