//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <math.h>
#include "Arka.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#define PI 3.14159265
TForm1 *Form1;
        int x=-5;///predkosc pilki w poziomie
        int y=-5;///predkosc pilki w pionie
        int r= 0;
        int bonus=100;
        int fire=0;///strzelanie z
        int fireindeks=200;
        TShape *Brick[2048];///klocki na mapie
        bool Colision( TShape * balll, TShape * brick)//wykrywanie kolizji
        {
                if(balll->Left >= brick->Left-balll->Width &&
                balll->Left <=brick->Left+brick->Width&&
                balll->Top >= brick->Top-balll->Height&&
                balll->Top <= brick->Top+brick->Height)
                {return true;}
                else {return false;}
        }
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer1_BallsTimer(TObject *Sender)
{

        Ball->Left+=x;///nadanie predkosci pilki w poziomie
        Ball->Top+=y; ///nadanie predkosci pilki w pionie

        if(Ball->Left-5 <= BackGround->Left) x=-x;///odbicie od scian
        if(Ball->Top-5 <= BackGround->Top) y=-y;
        if(Ball->Left+Ball->Width+5 >= BackGround->Width) x=-x;
        if(Ball->Top >= Paddle->Top+Paddle->Height+20)///jesli pilka znajdzie sie ponizej naszej paletki
        {
                Ball->Visible=false;
                Game_Over->Visible=true;
        }else if (Ball->Left > Paddle->Left-Ball->Width/2&& Ball->Left <Paddle->Left+Paddle->Width && Ball->Top+Ball->Height> Paddle->Top)///odbicie od paletki pilki
        {
                int bounce=0;
                bool hit=true;
                int to_angle=0;
                float angle=0;
                float xangle=1;
                while(hit)
                {
                        if (bounce<Paddle->Width/2)
                        {
                                if(Ball->Left > Paddle->Left-Ball->Width/2&& Ball->Left <Paddle->Left+(bounce))///odbicie od lewej strony paletki
                                {
                                //im wieksze bounce tym mniejsze x
                                to_angle=((bounce+35)*120)/Paddle->Width/2;
                                angle=(sin(to_angle*PI/180.0));
                                xangle=xangle-angle;
                                x=-12*xangle;
                                y=12*angle+1;
                                hit=false;
                                Label1->Caption=angle;
                                }
                        }
                        else
                        {
                                if(Ball->Left > Paddle->Left-Ball->Width/2&& Ball->Left <Paddle->Left+(bounce))///odbicie od prawej strony paletki
                                {
                                //im wieksze bounce tym wieksze x
                                to_angle=((bounce+8)*120)/Paddle->Width/2;
                                angle=sin(to_angle*PI/180.0);
                                xangle=xangle-angle;
                                x=12*angle;
                                y=12*xangle+1;
                                hit=false;
                                Label1->Caption=angle;
                                }
                        }
                        bounce++;
                }
                if(y>0) y= -y;
        }
        for(int i=1; i<=60; i++)
        {
        if(Colision(Ball,Brick[i]) && Brick[i]->Visible==true)///zderzenie pilki z blokiem
        {
                x= -x;
                y= -y;
                if(Brick[i]->Brush->Color==clSkyBlue) { Brick[i]->Brush->Color=clBlue;}
                else if(Brick[i]->Brush->Color==clBlue)
                {Brick[i]->Visible=false;

                int my_random=(rand() % 5);
                if(my_random==1)/// co 5 blok zawiera bonus
                {
                        Brick[bonus] = new TShape(BackGround); ///wypuszczanie nowego bloku ktory spada w dol
                        Brick[bonus]->Parent=Form1;
                        Brick[bonus]->Shape=stRectangle;
                        Brick[bonus]->Width=20;
                        Brick[bonus]->Height=20;
                        Brick[bonus]->Left=Brick[i]->Left;
                        Brick[bonus]->Top=Brick[i]->Top;
                        Brick[bonus]->Brush->Color=clRed;
                        bonus++;
                }
                }
        }
        //sprawdzanie czy klocek ktory jest bonusem czy zostal zebrany
        }
        for(int y=100; y<bonus; y++)
        {
                Brick[y]->Top+=8;
                if (Colision(Brick[y],Paddle) && Brick[y]->Visible==true)//jesli tak nastepuje
                {
                        Brick[y]->Visible=false;
                        Paddle->Width+=25; //powiekszenie naszej paletki
                        fire=1; ///oraz dodanie mozliwosci strzalu

                } else if(Brick[y]->Top>580&& Brick[y]->Visible==true&& Paddle->Width>50)// jesli klocek znajduje sie ponizej paletki
                {
                   Brick[y]->Visible=false;
                   Paddle->Width-=50; // pomniejszenie paletki
                   fire=0; //zlikwidowanie mozliwosci strzalu
                }
        }
        ///strzelanie czyli w skrocie tworzenie waskich bloczkow ktore leca w gore
        for(int o=200; o<fireindeks; o++)
        {

                Brick[o]->Top-=8;
                for (int i=1; i<=60; i++)
                {
                if (Colision(Brick[o],Brick[i]) && Brick[o]->Visible==true&&Brick[i]->Visible==true)// wykrycie kolizji
                {
                Brick[o]->Visible=false;
                Brick[i]->Visible=false;
                }
                if(Brick[o]->Top<10&& Brick[o]->Visible==true)
                {
                Brick[o]->Visible=false;
                }
                }
        }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer2_LeftTimer(TObject *Sender)
{
        if(Paddle->Left> 5) Paddle->Left-=10;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Timer3_RightTimer(TObject *Sender)
{
        if(Paddle->Left+Paddle->Width < BackGround ->Width -5)Paddle->Left+=10;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        if (Key == VK_LEFT) Timer2_Left->Enabled = true;
        if (Key == VK_RIGHT) Timer3_Right->Enabled = true;

}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
        if (Key == VK_LEFT) Timer2_Left->Enabled = false;
        if (Key == VK_RIGHT) Timer3_Right->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormKeyPress(TObject *Sender, char &Key)
{

      if(fire==1&&Key==VK_SPACE)
       {
       Brick[fireindeks]= new TShape(BackGround);
       Brick[fireindeks]->Parent=Form1;
       Brick[fireindeks]->Shape=stRectangle;
       Brick[fireindeks]->Width=3;
       Brick[fireindeks]->Height=10;
       Brick[fireindeks]->Left=Paddle->Left+Paddle->Width/2;
       Brick[fireindeks]->Top=Paddle->Top;
       Brick[fireindeks]->Brush->Color=clYellow;
       fireindeks++;
       }
      if (Key == 0x52)
      {
        Timer1_Balls->Enabled=false;
        Ball->Left=600;
        Ball->Top=520;
        x=-5;
        y=-5;
        Ball->Visible=true;
        Paddle->Width=200;

        if(r==0)
        {

                for(int j=0; j<=3; j++)
                {
                        for(int i=1; i<=15; i++)
                        {
                                Brick[i+(j*15)] = new TShape(BackGround);
                                Brick[i+(j*15)]->Parent=Form1;
                                Brick[i+(j*15)]->Shape=stRectangle;
                                Brick[i+(j*15)]->Width=70;
                                Brick[i+(j*15)]->Height=30;
                                Brick[i+(j*15)]->Left=i*75-35;
                                Brick[i+(j*15)]->Top=40+(j)*40;
                                Brick[i+(j*15)]->Brush->Color=clSkyBlue;
                        }
                }
        }else{for(int i=1; i<=60; i++){Brick[i]->Visible=true;Brick[i]->Brush->Color=clSkyBlue;Brick[i]->Visible=true;}
        /*for(int m=1; m<=60; m++){
        try{Brick[m]->Destroying();}catch(...) {;}}*/
      }

       r=1;
       Game_Over->Visible=false;
       Timer1_Balls->Enabled=true;
      }
}
//---------------------------------------------------------------------------








