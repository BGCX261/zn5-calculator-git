#ifndef MZWIDGET_H_
#define MZWIDGET_H_

#include <ZWidget.h>
#include <ZMessageDlg.h>
#include <qpixmap.h>

#include "debug.h"
#include "Data.h"
#include "key.h"
#include "qdir.h"

using namespace std;

class mZWidget : public ZWidget
{
	public:
		mZWidget();
		~mZWidget();
		virtual void paintEvent(QPaintEvent *e);
		virtual void keyPressEvent(QKeyEvent *e);
		virtual void keyReleaseEvent(QKeyEvent *e);
	private:
		QPixmap screenBuffer;
		QPixmap bgImg;
		QString appPath;
		QString express;
		QString ans;
		QString getProgramDir();
		Data *data;
		enum {num, func}sta;//处于的状态
		void drawButtonNum(QPainter *p);
		void drawButtonFunc(QPainter *p);
		void deleteEle();
		bool keyPressed;
		int key;
};

#endif /* MZWIDGET_H_ */

