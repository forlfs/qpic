#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
	Q_OBJECT

public:
	explicit Widget(QWidget *parent = 0);
	~Widget();
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);

private:
	Ui::Widget *ui;
	QTimer *tmr1;
	//QPixmap logo;
	//QPixmap pic_tile[3][3];
	QPixmap tile[9];

	//int a[3][3];
	//int x, y;
	int m_x;
	int m_i;
	int time1;

	void init();
	bool ac();

private slots:
	void tmr1Timer();
	void pushButtonClick();
};

#endif // WIDGET_H
