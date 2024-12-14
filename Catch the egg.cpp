# include "iGraphics.h"
#include<stdlib.h>
#include<string.h>
int width=1200,height=750;
int buttonWidth=200,buttonHeight=40;
int mouseX, mouseY,randomEggX,randomEggY,randomEggType,randomPerkX,randomPerkY,randomPerkType,basketX=width/2,basketY=150;
int gameTime=40,cTime=0,eggNum=0,scoreVal=0,perkShow=0,timeCounter=0,dropSpeedY=5, dropSpeedX=0, perkTime=10, textInput=0, nameLength=1, textShow=1, resumeStatus=0, loadSave=0;
int t0=0,t1=0,t2=0,t3=0,t4=0,t5=0,t6=0,tC0,tC1,tC2,tC3,tC4,tC5,tC6,tC5time=0,tC6time=0,tC5draw=0,tC6draw=0;
int stickWidth=1200,stickHeight=10,chickenWidth=100,chickenHeight=100,eggWidth=30,eggHeight=40,basketWidth=140,basketHeight=60,perkWidth=40, perkHeight=40, perkBoxWidth=120, perkBoxHeight=20;
char timeS[21]="Time :      ", scoreS[21]="Score :      ", name[22]=" ";
enum state
{
    mainMenu,
    newGame,
    normalNewGame,
    hardNewGame,
    gameOn,
    pause,
    resume,
    highScore,
    normalHighScore,
    hardHighScore,
    controls,
    score,
    exitMenu,
    confirmExit
};
enum state gameState=mainMenu;

enum _eggType
{
    normal=1,
    blue=5,
    golden=10,
    shit=-10
};
enum _eggType eggType;

enum _perkType
{
    slow,
    largeBasket,
    extraTime=15
};
enum _perkType perkType;

struct chicken
{
    int stickX;
    int stickY;
    int chickenX;
    int chickenY;
    int egg[10][4];
};

struct chicken ckn1={width/2,600-stickHeight/2,width/2,600+chickenHeight/2};

struct participant
{
    int score;
    char name[22];
};
/*
	function iDraw() is called again and again by the system.
*/

void displayVariable (char* p, int pos, int value)
{
    int i=10,sze=1;
    while (value/i>0) {
        i*=10;
        sze++;
    }
    p[pos+sze]='\0';
    sze--;
    while (sze>=0) {
        p[pos+sze]='0'+(value%10);
        value/=10;
        sze--;
    }
}

void button (int x, int y, char* p, int unavailable=0, int bWidth=buttonWidth, int bHeight=buttonHeight)
{
    iSetColor(250,250,250);
    iFilledRectangle(x-bWidth/2,y-bHeight/2,bWidth,bHeight);
    iSetColor(0,0,0);
    if (unavailable==1) iSetColor(100,100,100);
    iText(x-(strlen(p)*9)/2,y-5,p,GLUT_BITMAP_9_BY_15);
}

int buttonClickCheck (int x, int y, enum state p, int bWidth=buttonWidth, int bHeight=buttonHeight)
{
    int r=0;
    if ((mouseX>=x-bWidth/2 && mouseX<=x+bWidth/2) && (mouseY>=y-bHeight/2 && mouseY<=y+bHeight/2)) {
        gameState=p;
        r=1;
    }
    return r;
}

void menu()
{
    iShowBMP(0,0,"menubackground.bmp");
    if (resumeStatus==1) button(width/2,height/2-0,"Resume");
    else button(width/2,height/2-0,"Resume",1);
    button(width/2,height/2-60,"New Game");
    button(width/2,height/2-120,"High Score");
    button(width/2,height/2-180,"Instructions");
    button(width/2,height/2-240,"Exit");
}

void _pause()
{
    iShowBMP(0,0,"menubackground.bmp");
    iSetColor(255,0,0);
    button(width/2,height/2-60,"Resume");
    button(width/2,height/2-120,"Exit to Main Menu");
}

void scoreSort(struct participant lst[],int n)
{
    struct participant t;
    for (int i=0;i<n-1;i++) {
        for (int j=0;j<n-1-i;j++) {
            if (lst[j].score<lst[j+1].score) {
                strcpy(t.name,lst[j].name);
                strcpy(lst[j].name,lst[j+1].name);
                strcpy(lst[j+1].name,t.name);
                t.score=lst[j].score;
                lst[j].score=lst[j+1].score;
                lst[j+1].score=t.score;
            }
        }
    }
}

void save(char *p)
{
    char ch[5];
    struct participant normal[11];
    FILE* fp=NULL,*fpout=NULL;
    fp=fopen(p,"r");
    for (int i=0;i<10;i++) {
        fscanf(fp,"%s",ch);
        fscanf(fp,"%[^\n]s",normal[i].name);
        fscanf(fp,"%d",&normal[i].score);
    }
    fpout=fopen(p,"w");
    normal[10].score=scoreVal;
    strcpy(normal[10].name,name);
    scoreSort(normal,11);
    for (int i=0;i<10;i++) {
        fprintf(fpout,"n%s\n%d\n",normal[i].name,normal[i].score);
    }
    fclose(fp);
    fclose(fpout);
}

void _score()
{
    iShowBMP(0,0,"menubackground.bmp");
    iSetColor(0,0,0);
    iText(width/2-(strlen(scoreS)*9)/2,height/2+100,scoreS,GLUT_BITMAP_9_BY_15);
    iText(width/2-250,height/2-5,"Enter you name",GLUT_BITMAP_9_BY_15);
    iRectangle(width/2-buttonWidth/2,height/2-buttonHeight/2,buttonWidth,buttonHeight);
    if (textShow==1) iText(width/2-(9*13)/2,height/2-5,"Click to type",GLUT_BITMAP_9_BY_15);
    iText(width/2-buttonWidth/2+10,height/2-5,name+1,GLUT_BITMAP_9_BY_15);
    if (nameLength==1) button(width/2,height/2-120,"Save",1);
    else button(width/2,height/2-120,"Save");
    button(width/2,height/2-180,"Exit to Main Menu");
}

void game ()
{
    iShowBMP(0,0,"gamebackground.bmp");
    iSetColor(255,255,255);
    //iLine(75,150,1125,150);
    if (basketWidth==140) iShowBMP2(basketX-basketWidth/2,basketY-basketHeight/2-40,"smallbasket.bmp",0xffffff);
    else iShowBMP2(basketX-basketWidth/2,basketY-basketHeight/2-60,"largebasket.bmp",0xffffff);
    //iEllipse(basketX,basketY,basketWidth/2,basketHeight/2);
    //iRectangle(basketX-basketWidth/2,basketY-basketHeight/2,basketWidth,basketHeight);
    //iFilledRectangle(ckn1.stickX-stickWidth/2,ckn1.stickY-stickHeight/2,stickWidth,stickHeight);
    iShowBMP2(ckn1.stickX-stickWidth/2,ckn1.stickY-stickHeight/2,"rope.bmp",0xffffff);
    iSetColor(255,255,255);
    if (tC5draw==1) {
        iRectangle(width-150,height-200,perkBoxWidth,perkBoxHeight);
        iFilledRectangle(width-150+5,height-200+5,(perkTime-tC5time)*((perkBoxWidth-2*5)/perkTime),perkBoxHeight-2*5);
    }
    if (tC6draw==1) {
        iRectangle(width-150,height-230,perkBoxWidth,perkBoxHeight);
        iFilledRectangle(width-150+5,height-230+5,(perkTime-tC6time)*((perkBoxWidth-2*5)/perkTime),perkBoxHeight-2*5);
    }
    iSetColor(0,255,0);
    for (int i=0;i<10;i++) {
        if (ckn1.egg[i][0]==1) {
            if (ckn1.egg[i][1]==normal) {
                iSetColor(255,255,255);
                //iFilledRectangle(ckn1.egg[i][2]-eggWidth/2,ckn1.egg[i][3]-eggHeight/2,eggWidth,eggHeight);
                if (ckn1.egg[i][3]>80) iShowBMP2(ckn1.egg[i][2]-eggWidth/2,ckn1.egg[i][3]-eggHeight/2,"normalegg.bmp",0xffffff);
                else iShowBMP2(ckn1.egg[i][2]-80/2,60,"brokenegg.bmp",0x000000);
            }
            else if (ckn1.egg[i][1]==blue) {
                iSetColor(15,181,223);
                //iFilledRectangle(ckn1.egg[i][2]-eggWidth/2,ckn1.egg[i][3]-eggHeight/2,eggWidth,eggHeight);
                //iFilledEllipse(ckn1.egg[i][2],ckn1.egg[i][3],eggWidth/2,eggHeight/2);
                if (ckn1.egg[i][3]>80) iShowBMP2(ckn1.egg[i][2]-eggWidth/2,ckn1.egg[i][3]-eggHeight/2,"blueegg.bmp",0xffffff);
                else iShowBMP2(ckn1.egg[i][2]-80/2,60,"brokenegg.bmp",0x000000);
            }
            else if (ckn1.egg[i][1]==golden) {
                iSetColor(255,215,0);
                //iFilledRectangle(ckn1.egg[i][2]-eggWidth/2,ckn1.egg[i][3]-eggHeight/2,eggWidth,eggHeight);
                if (ckn1.egg[i][3]>80) iShowBMP2(ckn1.egg[i][2]-eggWidth/2,ckn1.egg[i][3]-eggHeight/2,"goldenegg.bmp",0xffffff);
                else iShowBMP2(ckn1.egg[i][2]-80/2,60,"brokenegg.bmp",0x000000);
            }
            else if (ckn1.egg[i][1]==shit) {
                iSetColor(165,42,42);
                //iFilledRectangle(ckn1.egg[i][2]-eggWidth/2,ckn1.egg[i][3]-eggHeight/2,eggWidth,eggHeight);
                if (ckn1.egg[i][3]>80) iShowBMP2(ckn1.egg[i][2]-eggWidth/2,ckn1.egg[i][3]-eggHeight/2,"poop.bmp",0xffffff);
                else iShowBMP2(ckn1.egg[i][2]-80/2,60,"brokenpoop.bmp",0xffffff);
            }
        }
    }
    iSetColor(255,0,0);
    iShowBMP2(ckn1.chickenX-chickenWidth/2,ckn1.chickenY-chickenHeight/2,"chicken.bmp",0xffffff);
    //iFilledRectangle(ckn1.chickenX-chickenWidth/2,ckn1.chickenY-chickenHeight/2,chickenWidth,chickenHeight);
    if (perkShow==1) {
        if (perkType==slow) {
            iSetColor(255,255,0);
            //iFilledRectangle(randomPerkX-perkWidth/2,randomPerkY-perkHeight/2,perkWidth,perkHeight);
            iShowBMP2(randomPerkX-perkWidth/2,randomPerkY-perkHeight/2,"slow.bmp",0xffffff);
        }
        else if (perkType==largeBasket) {
            iSetColor(255,0,0);
            //iFilledRectangle(randomPerkX-perkWidth/2,randomPerkY-perkHeight/2,perkWidth,perkHeight);
            iShowBMP2(randomPerkX-perkWidth/2,randomPerkY-perkHeight/2,"big.bmp",0xffffff);
        }
        else if (perkType==extraTime) {
            iSetColor(0,255,0);
            //iFilledRectangle(randomPerkX-perkWidth/2,randomPerkY-perkHeight/2,perkWidth,perkHeight);
            iShowBMP2(randomPerkX-perkWidth/2,randomPerkY-perkHeight/2,"time.bmp",0xffffff);
        }
    }
}

void savingGame()
{
    FILE* fp=NULL;
    fp=fopen("save.txt","w");
    fprintf(fp,"%d\n",resumeStatus);
    fprintf(fp,"%d\n",basketX);
    fprintf(fp,"%d\n",basketY);
    fprintf(fp,"%d\n",eggNum);
    fprintf(fp,"%d\n",scoreVal);
    fprintf(fp,"%d\n",perkShow);
    fprintf(fp,"%d\n",randomPerkX);
    fprintf(fp,"%d\n",randomPerkY);
    fprintf(fp,"%d\n",randomPerkType);
    fprintf(fp,"%d\n",gameTime);
    fprintf(fp,"%d\n",cTime);
    fprintf(fp,"%d\n",dropSpeedY);
    fprintf(fp,"%d\n",dropSpeedX);
    fprintf(fp,"%d\n",perkTime);
    fprintf(fp,"%d\n",tC5time);
    fprintf(fp,"%d\n",tC6time);
    fprintf(fp,"%d\n",tC5draw);
    fprintf(fp,"%d\n",tC6draw);
    fprintf(fp,"%d\n",basketWidth);
    fprintf(fp,"%d\n",basketHeight);
    fprintf(fp,"%d\n",ckn1.chickenX);
    fprintf(fp,"%d\n",ckn1.chickenY);
    for (int i=0;i<10;i++) {
        fprintf(fp,"%d\n",ckn1.egg[i][0]);
        fprintf(fp,"%d\n",ckn1.egg[i][1]);
        fprintf(fp,"%d\n",ckn1.egg[i][2]);
        fprintf(fp,"%d\n",ckn1.egg[i][3]);
    }

    //resumeStatus=0;basketX=width/2; basketY=100; eggNum=0; scoreVal=0; perkShow=0; randomPerkX,randomPerkY,randomPerkType gameTime=40,cTime=0
    //dropSpeedY=5, dropSpeedX=0, perkTime=10 tC5time=0,tC6time=0,tC5draw=0,tC6draw=0; basketWidth=140,basketHeight=60,
    //ckn1
    fclose(fp);
}

void playTime()
{
    cTime++;
    displayVariable(timeS,8,gameTime-cTime);
    if (cTime==gameTime) {
        if (tC5draw==1) iPauseTimer(tC5);
        if (tC6draw==1) iPauseTimer(tC6);
        iPauseTimer(tC4);
        iPauseTimer(tC3);
        iPauseTimer(tC2);
        iPauseTimer(tC1);
        iPauseTimer(tC0);
        resumeStatus=0;
        savingGame();
        gameState=score;
    }
}

void eggMovement()
{
    for (int i=0;i<10;i++) {
        if (ckn1.egg[i][0]==1) {
            if (ckn1.egg[i][3]>80) ckn1.egg[i][2]-=dropSpeedX;
            ckn1.egg[i][3]-=dropSpeedY;
            if ((ckn1.egg[i][2]+eggWidth/2>=basketX-basketWidth/2 && ckn1.egg[i][2]-eggWidth/2<=basketX+basketWidth/2) && (ckn1.egg[i][3]+eggHeight/2>=basketY-basketHeight/2 && ckn1.egg[i][3]-eggHeight/2<=basketY+basketHeight/6)) {
                ckn1.egg[i][0]=0;
                scoreVal+=ckn1.egg[i][1];
                if (scoreVal<0) scoreVal=0;
                displayVariable(scoreS,9,scoreVal);
            }
            else if (ckn1.egg[i][3]<0) ckn1.egg[i][0]=0;
        }
    }
}

void eggDrop()
{
    randomEggX=rand()%1050+75;
    randomEggY=600+chickenHeight/2;
    randomEggType=rand()%10;
    if (randomEggType==3) eggType=golden;
    else if (randomEggType==8 || randomEggType==1) eggType=blue;
    else if (randomEggType==7 || randomEggType==4) eggType=shit;
    else eggType=normal;
    ckn1.egg[eggNum][0]=1;
    ckn1.egg[eggNum][1]=eggType;
    ckn1.egg[eggNum][2]=randomEggX;
    ckn1.egg[eggNum][3]=randomEggY;
    ckn1.chickenX=randomEggX;
    ckn1.chickenY=randomEggY;
    eggNum++;
    if (eggNum>9) eggNum=0;

}

void bigBasket()
{
    tC5time++;
    if (tC5time==perkTime) {
        basketWidth=140;
        basketHeight=60;
        tC5draw=0;
        iPauseTimer(tC5);
    }
}

void speedFix()
{
    tC6time++;
    if (tC6time==perkTime) {
        dropSpeedY=5;
        tC6draw=0;
        iPauseTimer(tC6);
    }
}

void perkMovement()
{
    if (perkShow==1) {
        randomPerkY-=dropSpeedY;
        randomPerkX-=dropSpeedX;
        if ((randomPerkX+perkWidth/2>=basketX-basketWidth/2 && randomPerkX-perkWidth/2<=basketX+basketWidth/2) && (randomPerkY+perkHeight/2>=basketY-basketHeight/2 && randomPerkY-perkHeight/2<=basketY+basketHeight/6)) {
            perkShow=0;
            if (perkType==slow) {
                dropSpeedY=3;
                if (t6==0) {
                    t6=1;
                    tC6time=0;
                    tC6draw=1;
                    tC6=timeCounter;
                    timeCounter++;
                    iSetTimer(1000,speedFix);
                }
                else if (t6==1) {
                    tC6time=0;
                    tC6draw=1;
                    iResumeTimer(tC6);
                }
            }
            else if (perkType==largeBasket) {
                basketWidth=210;
                basketHeight=90;
                if (t5==0) {
                    t5=1;
                    tC5time=0;
                    tC5draw=1;
                    tC5=timeCounter;
                    timeCounter++;
                    iSetTimer(1000,bigBasket);
                }
                else if (t5==1) {
                    tC5time=0;
                    tC5draw=1;
                    iResumeTimer(tC5);
                }
            }
            else if (perkType==extraTime) gameTime+=15;
        }
    }
}

void perkDrop()
{
    perkShow=1;
    randomPerkX=rand()%1050+75;
    randomPerkY=height+perkHeight/2;
    randomPerkType=rand()%3;
    if (randomPerkType==0) perkType=slow;
    else if (randomPerkType==1) perkType=largeBasket;
    else if (randomPerkType==2) perkType=extraTime;
}

void play()
{
    iSetColor(255,0,0);
    if (t4==0) {
        t4=2;
        tC4=timeCounter;
        timeCounter++;
        iSetTimer(30,perkMovement);
    }
    else if (t4==1) {
        t4=2;
        iResumeTimer(tC4);
    }
    if (t3==0) {
        t3=2;
        tC3=timeCounter;
        timeCounter++;
        iSetTimer(10500,perkDrop);
    }
    else if (t3==1) {
        t3=2;
        iResumeTimer(tC3);
    }
    if (t1==0) {
        t1=2;
        tC1=timeCounter;
        timeCounter++;
        iSetTimer(2000,eggDrop);
        eggDrop();
    }
    else if (t1==1) {
        t1=2;
        eggDrop();
        iResumeTimer(tC1);
    }
    if (t2==0) {
        t2=2;
        tC2=timeCounter;
        timeCounter++;
        iSetTimer(20,eggMovement);
    }
    else if (t2==1) {
        t2=2;
        iResumeTimer(tC2);
    }
    if (t0==0) {
        t0=2;
        tC0=timeCounter;
        timeCounter++;
        iSetTimer(1000,playTime);
        displayVariable(timeS,8,gameTime-cTime);
        displayVariable(scoreS,9,scoreVal);
    }
    else if (t0==1) {
        //cTime=0;
        displayVariable(timeS,8,gameTime-cTime);
        displayVariable(scoreS,9,scoreVal);
        t0=2;
        iResumeTimer(tC0);//
    }
    game();
    iSetColor(0,0,0);
    iText(width/2-(strlen(timeS)*9)/2,height-45,timeS,GLUT_BITMAP_9_BY_15);
    iText(width-100-(strlen(scoreS)*9)/2,height-45,scoreS,GLUT_BITMAP_9_BY_15);
    //printf("%s\n",timeS);
}

void diffHighScore(char* p)
{
    iShowBMP(0,0,"newmenu.bmp");
    iSetColor(0,0,0);
    char ch[5],chr[22],cr[6];
    int x,y=40;
    FILE* fp=NULL;
    fp=fopen(p,"r");
    iText(300,600,"No",GLUT_BITMAP_9_BY_15);
    iText(350,600,"Name",GLUT_BITMAP_9_BY_15);
    iText(650,600,"Score",GLUT_BITMAP_9_BY_15);
    for (int i=0;i<10;i++) {
        fscanf(fp,"%s",ch);
        fscanf(fp,"%[^\n]s",chr);
        fscanf(fp,"%d",&x);
        displayVariable(cr,0,i+1);
        iText(300,600-y,cr,GLUT_BITMAP_9_BY_15);
        if (strlen(chr)>1) {
            iText(350,600-y,chr+1,GLUT_BITMAP_9_BY_15);
            displayVariable(cr,0,x);
            iText(650,600-y,cr,GLUT_BITMAP_9_BY_15);
        }
        y+=40;
    }
    fclose(fp);
    button(width-200,80,"Go Back");
}

void hScore()
{
    iShowBMP(0,0,"menubackground.bmp");
    iSetColor(255,0,0);
    button(width/2,height/2+20,"Normal");
    button(width/2,height/2-40,"Hard");
    button(width-200,80,"Exit to Main Menu");
}

void fControls()
{
    iShowBMP(0,0,"newmenu.bmp");
    iSetColor(0,0,0);
    iText(width/2-(strlen("Game Controls")*9)/2,600,"Game Controls",GLUT_BITMAP_9_BY_15);
    iText(200,550,"Move your basket to left using A or Left Arrow key and to right using D or Right Arrow Key.",GLUT_BITMAP_9_BY_15);
    iText(200,520,"Your can also move your basket using Left mouse button click and drag. Your goal is to ",GLUT_BITMAP_9_BY_15);
    iText(200,490,"catch as many eggs you can. White egg gives you 1 point, Blue egg gives you 5 points, Golden",GLUT_BITMAP_9_BY_15);
    iText(200,460,"egg gives you 10 points. Don't catch poop. Catching poop will reduce your point by 10. Try ",GLUT_BITMAP_9_BY_15);
    iText(200,430,"to catch perks. Perks will give you some benefits. Time perk adds extra 15 seconds. Slow ",GLUT_BITMAP_9_BY_15);
    iText(200,400,"perk reduces falling speed for few seconds. Large basket perk makes your basket larger for ",GLUT_BITMAP_9_BY_15);
    iText(200,370,"few seconds. Press ESC key to go to pause menu.",GLUT_BITMAP_9_BY_15);
    iShowBMP2(220,320-eggHeight/2,"normalegg.bmp",0xffffff);
    iText(220+eggWidth/2+20,320-5,"= 1 point",GLUT_BITMAP_9_BY_15);
    iShowBMP2(220+150,320-eggHeight/2,"blueegg.bmp",0xffffff);
    iText(220+eggWidth/2+150+20,320-5,"= 5 points",GLUT_BITMAP_9_BY_15);
    iShowBMP2(220+400-perkWidth/2,320-perkHeight/2,"slow.bmp",0xffffff);
    iText(220+400+40,320-5,"reduces speed",GLUT_BITMAP_9_BY_15);
    iShowBMP2(220+600-perkWidth/2,320-perkHeight/2,"big.bmp",0xffffff);
    iText(220+600+40,320-5,"increase basket size",GLUT_BITMAP_9_BY_15);
    iFilledEllipse(220+eggWidth/2,270,eggWidth/2,eggHeight/2);
    iShowBMP2(220,270-eggHeight/2,"goldenegg.bmp",0xffffff);
    iText(220+eggWidth/2+20,270-5,"= 10 points",GLUT_BITMAP_9_BY_15);
    iShowBMP2(220+eggWidth/2+150-eggWidth/2,270-eggHeight/2,"poop.bmp",0xffffff);
    iText(220+eggWidth/2+150+20,270-5,"= -10 points",GLUT_BITMAP_9_BY_15);
    iShowBMP2(220+400-perkWidth/2,270-perkHeight/2,"time.bmp",0xffffff);
    iText(220+400+40,270-5,"extra 15 seconds",GLUT_BITMAP_9_BY_15);
    iText(width/2-(strlen("Menu Controls")*9)/2,200,"Menu Controls",GLUT_BITMAP_9_BY_15);
    iText(200,150,"Click buttons to do the task. Press ESC key to go back one step in menu section.",GLUT_BITMAP_9_BY_15);
    button(width-200,80,"Exit to Main Menu");
}

void close()
{
    savingGame();
    exit(0);
}

void _exitMenu()
{
    char s[]="Are you sure you want to exit?";
    iShowBMP(0,0,"menubackground.bmp");
    if (resumeStatus==1) button(width/2,height/2-0,"Resume");
    else button(width/2,height/2-0,"Resume",1);
    button(width/2,height/2-60,"New Game");
    button(width/2,height/2-120,"High Score");
    button(width/2,height/2-180,"Instructions");
    button(width/2,height/2-240,"Exit");
    iSetColor(230,230,230);
    iFilledRectangle(width/2-250,height/2-75,500,150);
    iSetColor(0,0,0);
    iText(width/2-(strlen(s)*9)/2,height/2+30,s,GLUT_BITMAP_9_BY_15);
    button(width/2-150,height/2-40,"Yes",0,130);
    button(width/2+150,height/2-40,"No",0,130);
}

void _resume()
{
    gameState=gameOn;
    if (loadSave==0) {
        iResumeTimer(tC0);
        iResumeTimer(tC1);
        iResumeTimer(tC2);
        iResumeTimer(tC3);
        iResumeTimer(tC4);
        if (tC5draw==1) iResumeTimer(tC5);
        if (tC6draw==1) iResumeTimer(tC6);
    }
    else if (loadSave==1) {
        loadSave=0;
        if (tC6draw==1) {
            if (t6==0) {
                t6=1;
                tC6=timeCounter;
                timeCounter++;
                iSetTimer(1000,speedFix);
            }
        }
        if (tC5draw==1) {
            if (t5==0) {
                t5=1;
                tC5=timeCounter;
                timeCounter++;
                iSetTimer(1000,bigBasket);
            }
        }
    }
}

void _hardNewGame()
{
    basketX=width/2; basketY=150; eggNum=0; scoreVal=0; perkShow=0; perkTime=6;
    for (int i=0;i<10;i++) ckn1.egg[i][0]=0;
    tC5draw=0;
    tC6draw=0;
    basketWidth=140;
    basketHeight=60;
    dropSpeedY=5;
    dropSpeedX=1;
    loadSave=0;
    cTime=0;
    gameTime=40;
    if (t0==2) {
        t0=1;
        t1=1;
        t2=1;
        t3=1;
        t4=1;
    }
    gameState=gameOn;
}

void _normalNewGame()
{
    basketX=width/2; basketY=150; eggNum=0; scoreVal=0; perkShow=0; perkTime=8;
    for (int i=0;i<10;i++) ckn1.egg[i][0]=0;
    tC5draw=0;
    tC6draw=0;
    basketWidth=140;
    basketHeight=60;
    dropSpeedY=5;
    dropSpeedX=0;
    loadSave=0;
    cTime=0;
    gameTime=60;
    if (t0==2) {
        t0=1;
        t1=1;
        t2=1;
        t3=1;
        t4=1;
    }
    gameState=gameOn;
}

void _newGame()
{
    iShowBMP(0,0,"menubackground.bmp");
    iSetColor(255,0,0);
    button(width/2,height/2+20,"Normal (60s)");
    button(width/2,height/2-40,"Hard (40s)");
    button(width-200,80,"Exit to Main Menu");
}

void iDraw()
{
    //place your drawing codes here
    iClear();
    if (gameState==mainMenu) menu();
    else if (gameState==resume) _resume();
    else if (gameState==newGame) _newGame();
    else if (gameState==normalNewGame) _normalNewGame();
    else if (gameState==hardNewGame) _hardNewGame();
    else if (gameState==gameOn) play();
    else if (gameState==highScore) hScore();
    else if (gameState==normalHighScore) diffHighScore("normal.txt");
    else if (gameState==hardHighScore) diffHighScore("hard.txt");
    else if (gameState==score) _score();
    else if (gameState==pause) _pause();
    else if (gameState==controls) fControls();
    else if (gameState==exitMenu) _exitMenu();
    else if (gameState==confirmExit) close();

}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    //printf("x = %d, y= %d\n",mx,my);
    //place your codes here
    if (gameState==gameOn) {
        if (mouseX-mx<0) {
            if (basketX<=width-basketWidth/2-25) basketX+=9;
        }
        else if (mouseX-mx>0) {
            if (basketX>=basketWidth/2+25) basketX-=9;
        }
        mouseX=mx;
        mouseY=my;
    }
}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        mouseX=mx;
        mouseY=my;
        if (gameState==mainMenu) {
            if (resumeStatus==1) buttonClickCheck(width/2,height/2-0,resume);
            buttonClickCheck(width/2,height/2-60,newGame);
            buttonClickCheck(width/2,height/2-120,highScore);
            buttonClickCheck(width/2,height/2-180,controls);
            buttonClickCheck(width/2,height/2-240,exitMenu);
        }
        else if (gameState==newGame) {
            buttonClickCheck(width/2,height/2+20,normalNewGame);
            buttonClickCheck(width/2,height/2-40,hardNewGame);
            buttonClickCheck(width-200,80,mainMenu);
        }
        else if (gameState==score) {
            if(buttonClickCheck(width/2,height/2,score)) {
                textInput=1;
                textShow=0;
            }
            if (nameLength>1) if(buttonClickCheck(width/2,height/2-120,mainMenu)) {
                if (dropSpeedX==0) save("normal.txt");
                else save("hard.txt");
                textInput=0;
                textShow=1;
                strcpy(name," ");
                nameLength=1;
                //t0=1;
                //resumeStatus=0;
            }
            if(buttonClickCheck(width/2,height/2-180,mainMenu)) {
                //t0=1;
                textInput=0;
                textShow=1;
                strcpy(name," ");
                nameLength=1;
                //resumeStatus=0;
            }
        }
        else if (gameState==highScore) {
            buttonClickCheck(width/2,height/2+20,normalHighScore);
            buttonClickCheck(width/2,height/2-40,hardHighScore);
            buttonClickCheck(width-200,80,mainMenu);
        }
        else if (gameState==normalHighScore) {
            buttonClickCheck(width-200,80,highScore);
        }
        else if (gameState==hardHighScore) {
            buttonClickCheck(width-200,80,highScore);
        }
        else if (gameState==controls) {
            buttonClickCheck(width-200,80,mainMenu);
        }
        else if (gameState==pause) {
            if(buttonClickCheck(width/2,height/2-60,gameOn)) {
                iResumeTimer(tC0);
                iResumeTimer(tC1);
                iResumeTimer(tC2);
                iResumeTimer(tC3);
                iResumeTimer(tC4);
                if (tC5draw==1) iResumeTimer(tC5);
                if (tC6draw==1) iResumeTimer(tC6);
            }
            if(buttonClickCheck(width/2,height/2-120,mainMenu)) {
                resumeStatus=1;
                savingGame();
            }
        }
        else if (gameState==exitMenu) {
            buttonClickCheck(width/2-150,height/2-40,confirmExit,130);
            buttonClickCheck(width/2+150,height/2-40,mainMenu,130);
        }
    }
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if(key == 27) {
        if (gameState==mainMenu) gameState=exitMenu;
        else if (gameState==gameOn) {
            if (tC5draw==1) iPauseTimer(tC5);
            if (tC6draw==1) iPauseTimer(tC6);
            iPauseTimer(tC4);
            iPauseTimer(tC3);
            iPauseTimer(tC2);
            iPauseTimer(tC1);
            iPauseTimer(tC0);
            gameState=pause;
        }
        else if (gameState==pause) {
            gameState=gameOn;
            iResumeTimer(tC0);
            iResumeTimer(tC1);
            iResumeTimer(tC2);
            iResumeTimer(tC3);
            iResumeTimer(tC4);
            if (tC5draw==1) iResumeTimer(tC5);
            if (tC6draw==1) iResumeTimer(tC6);
        }
        else if (gameState==newGame) gameState=mainMenu;
        else if (gameState==highScore) gameState=mainMenu;
        else if (gameState==normalHighScore) gameState=highScore;
        else if (gameState==hardHighScore) gameState=highScore;
        else if (gameState==controls) gameState=mainMenu;
        else if (gameState==exitMenu) gameState=mainMenu;
        else if (textInput==1) {
            textInput=0;
            if (nameLength==1) textShow=1;
        }
        //else if (gameState==mainMenu) gameState=exit;
    }
    if (gameState==gameOn) {
        if (key=='a') {
            if (basketX>=basketWidth/2+25) basketX-=25;
        }
        if (key=='d') {
            if (basketX<=width-basketWidth/2-25) basketX+=25;
        }
    }
    if (textInput==1) {
        if (key=='\b') {
            if (nameLength>1) {
                nameLength--;
                name[nameLength]='\0';
            }
        }
        else if (key==13) {
            if (nameLength>1) {
                gameState=mainMenu;
                if (dropSpeedX==0) save("normal.txt");
                else save("hard.txt");
                textInput=0;
                textShow=1;
                strcpy(name," ");
                nameLength=1;
                //t0=1;
                //resumeStatus=0;
            }
        }
        else {
            if (nameLength<=20) {
                name[nameLength]=key;
                name[nameLength+1]='\0';
                nameLength++;
            }
        }
    }
    //place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{
    if (gameState==gameOn) {
        if (key==GLUT_KEY_LEFT) {
            if (basketX>=basketWidth/2+25) basketX-=25;
        }
        if (key==GLUT_KEY_RIGHT) {
            if (basketX<=width-basketWidth/2-25) basketX+=25;
        }
    }
    //place your codes for other keys here
}

void loadingSave()
{
    loadSave=1;
    FILE* fp=NULL;
    fp=fopen("save.txt","r");
    fscanf(fp,"%d",&resumeStatus);
    if (resumeStatus==1) {
        fscanf(fp,"%d",&basketX);
        fscanf(fp,"%d",&basketY);
        fscanf(fp,"%d",&eggNum);
        fscanf(fp,"%d",&scoreVal);
        fscanf(fp,"%d",&perkShow);
        fscanf(fp,"%d",&randomPerkX);
        fscanf(fp,"%d",&randomPerkY);
        fscanf(fp,"%d",&randomPerkType);
        fscanf(fp,"%d",&gameTime);
        fscanf(fp,"%d",&cTime);
        fscanf(fp,"%d",&dropSpeedY);
        fscanf(fp,"%d",&dropSpeedX);
        fscanf(fp,"%d",&perkTime);
        fscanf(fp,"%d",&tC5time);
        fscanf(fp,"%d",&tC6time);
        fscanf(fp,"%d",&tC5draw);
        fscanf(fp,"%d",&tC6draw);
        fscanf(fp,"%d",&basketWidth);
        fscanf(fp,"%d",&basketHeight);
        fscanf(fp,"%d",&ckn1.chickenX);
        fscanf(fp,"%d",&ckn1.chickenY);
        for (int i=0;i<10;i++) {
            fscanf(fp,"%d",&ckn1.egg[i][0]);
            fscanf(fp,"%d",&ckn1.egg[i][1]);
            fscanf(fp,"%d",&ckn1.egg[i][2]);
            fscanf(fp,"%d",&ckn1.egg[i][3]);
        }
    }

    //resumeStatus=0;basketX=width/2; basketY=100; eggNum=0; scoreVal=0; perkShow=0; randomPerkX,randomPerkY,randomPerkType gameTime=40,cTime=0
    //dropSpeedY=5, dropSpeedX=0, perkTime=10 tC5time=0,tC6time=0,tC5draw=0,tC6draw=0; basketWidth=140,basketHeight=60,
    //ckn1
    fclose(fp);
}


int main()
{
    //place your own initialization codes here.
    srand(time(NULL));
    loadingSave();
    iInitialize(width, height, "Catch the egg");
    return 0;
}
