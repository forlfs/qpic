#include "widget.h"
#include "ui_widget.h"
#include <stdlib.h>
#include <time.h>
#include <QMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QTime>
#include <QDebug>

//----------------------------------------------------------------------
#include <stdio.h>
#include <map>
using namespace std;
#define	N	400007
int tree[10];
int a[10],a_[10];
struct dir{
	int x,y;
}dir[4]={{-1,0},{1,0},{0,-1},{0,1}};
struct dir_{
	int x[4];
}dir_[9];
struct a{
	int x,i;
};
struct q{
	struct a q[N];
	int l,r;
}q;
map<int,int> dp;
void init(){
	int i,j,x,y;
	for(i=0;i<9;i++){
		for(j=0;j<4;j++){
			x=i/3+dir[j].x;
			y=i%3+dir[j].y;
			if(x<0||y<0||x>2||y>2){
				dir_[i].x[j]=-1;
				continue;
			}
			dir_[i].x[j]=x*3+y;
		}
	}
}
int swap(int x,int i,int j){
	char x_[10];
	sprintf(x_,"%d",x);
	char t=x_[i];
	x_[i]=x_[j];
	x_[j]=t;
	sscanf(x_,"%d",&x);
	return x;
}
void bfs(){
	int i,j,k,x,y;
	init();
	dp.clear();
	dp[0]=0;
	q.l=0;
	q.r=0;
	q.q[q.r].x=123456789;
	q.q[q.r].i=0;
	q.r++;
	while(q.l<q.r){
		x=q.q[q.l].x;
		i=q.q[q.l].i;
		q.l++;
		for(k=0;k<4;k++){
			j=dir_[i].x[k];
			if(j==-1){
				continue;
			}
			y=swap(x,i,j);
			if(dp.count(y)>0){
				continue;
			}
			dp[y]=dp[x]+1;
			q.q[q.r].x=y;
			q.q[q.r].i=j;
			q.r++;
		}
	}
	qDebug() << q.r;
}
void run(){
	bfs();
}
//----------------------------------------------------------------------

Widget::Widget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::Widget)
{
	ui->setupUi(this);

	run();

	srand(time(0));

	//加载图片
	QPixmap pic;
	pic.load("pic_0.jpg");
	for(int i = 0; i < 9; i++)
	{
		tile[i] = pic.copy(i % 3 * 100, i / 3 * 100, 100, 100);
		QPainter painter(&tile[i]);
		painter.drawText(3, 11, QString::number(i + 1));
	}

	m_x = 123456789;
	m_i = 0;
	init();
	time1 = 60;
	ui->label->setText(QString("剩余 %1 秒").arg(time1));
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(pushButtonClick()));
	tmr1 = new QTimer(this);
	connect(tmr1, SIGNAL(timeout()), this, SLOT(tmr1Timer()));
	tmr1->start(1000);
}

Widget::~Widget()
{
	//释放资源
	if(tmr1 != NULL)
	{
		delete tmr1;
		tmr1 = NULL;
	}

	delete ui;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
	int j = event->pos().y() / 100 * 3 + event->pos().x() / 100;
	for(int i = 0; i < 4; i++)
	{
		if(dir_[m_i].x[i] != j)
		{
			continue;
		}
		m_x = swap(m_x, m_i, j);
		m_i = j;
		update();
		if(ac())
		{
			tmr1->stop();
			ui->label->setText("犀利！");
		}
		return;
	}
}

void Widget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	char x_[10];
	sprintf(x_, "%d", m_x);
	for(int i = 0; i < 9; i++)
	{
		painter.drawPixmap(QPoint(i % 3 * 100, i / 3 * 100), tile[x_[i] - '1']);
	}
	painter.setPen(Qt::gray);
	painter.drawRect(0, 0, 300, 300);
	painter.drawLine(QPoint(100, 0), QPoint(100, 300));
	painter.drawLine(QPoint(200, 0), QPoint(200, 300));
	painter.drawLine(QPoint(0, 100), QPoint(300, 100));
	painter.drawLine(QPoint(0, 200), QPoint(300, 200));
}

void Widget::init()
{
	srand(time(0));
	//打乱顺序
	int i;
	for(int j = 0; j < 1000 || ac() || dp.count(m_x) == 0; j++)
	{
		i = rand() % 9;
		m_x = swap(m_x, m_i, i);
		m_i = i;
	}
	update();
}

bool Widget::ac()
{
	if(m_x != 123456789)
	{
		return false;
	}
	return true;
}

void Widget::tmr1Timer()
{
	time1--;
	ui->label->setText(QString("剩余 %1 秒").arg(time1));
	if(time1 == 0)
	{
		tmr1->stop();
		if(!ac())
		{
			ui->label->setText("渣渣！");
			return;
		}
	}
}

void Widget::pushButtonClick()
{
	int x, i;
	for(int j = 0; j < 4; j++)
	{
		i = dir_[m_i].x[j];
		if(i == -1)
		{
			continue;
		}
		x = swap(m_x, m_i, i);
		if(dp[x] != dp[m_x] - 1)
		{
			continue;
		}
		m_x = x;
		m_i = i;
		update();
		if(ac())
		{
			tmr1->stop();
			ui->label->setText("犀利！");
		}
		return;
	}
}
