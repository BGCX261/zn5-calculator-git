#include "mZWidget.h"

static QString funcStr[]={
	")",
	"sin("	,"cos("		,"tan(",
	"^"		,"abs("		,"PI",
	"exp("	,"sqrt("	,"ln(",
	"("
};

mZWidget::mZWidget()
{
	debug("start %d\n", 0);
	this->setGeometry(0, 0, 240, 320);
	screenBuffer = QPixmap(240, 320);
	QString path = getProgramDir();
	debug("path = %s\n", path.latin1());
	bgImg = QPixmap(path + "/res/cal.png");
	screenBuffer.fill(QColor(255,255,255));
	data = new Data();
	sta = num;//默认为数字输入状态
	express = "";
	//debug("appPath = %s\n", appPath.latin1());
	keyPressed = false;
}

mZWidget::~mZWidget()
{
	delete data;
}

void mZWidget::paintEvent(QPaintEvent *e)
{
	QPainter *p = new QPainter(&screenBuffer);
	/* 画背景图 */
	p->drawPixmap(0,0, bgImg);
	/* 画表达式 */
	p->drawText(30, 50, express);
	/* 画隔离线 */
	p->setPen(QColor(255,255,255));
	p->drawLine(25, 55, 215, 55);
	p->setPen(QColor(0,0,0));
	/* 画结果 */
	p->drawText(30, 75, ans);
	/* 根据状态画出按钮值 */
	switch(sta)
	{
		case num:
			drawButtonNum(p);
			break;
		case func:
			drawButtonFunc(p);
			break;
	}
	bitBlt(this, 0, 0, &screenBuffer);
	delete p;
}

void mZWidget::keyReleaseEvent(QKeyEvent *e)
{
	keyPressed = false;
	repaint();
}

void mZWidget::keyPressEvent(QKeyEvent *e)
{
	keyPressed = true;
	key = e->key();
	repaint();
	int code = e->key();
	double ansNum;
	if(code == KEY_SOFT_RIGHT) qApp->quit();
	else if(code == KEY_SOFT_LEFT || code == KEY_HANGDOWN){
	   	express="";
		ans = "";
	}
	if(code != KEY_CENTER)//只要按下的不是求值清空
		ans = "";
	if(code == KEY_CLEAR)//删除一个字符
	{
		deleteEle();
		repaint();
		return;
	}
	if(code == KEY_HANGUP)//显示关于信息
	{
		debug("show about\n");
		ZMessageDlg *dlg = new ZMessageDlg(
				QString::fromUtf8("感谢您的使用"),
				QString::fromUtf8(
					"科学计算器for zn5\n"
					"by 横笛"
					"<fuzhuo233@gmail.com>\n"
					"特殊操作:\n"
					"左键清除，右键退出\n"
					"播号键显示关于"),
					ZMessageDlg::TypeOK
					);
		dlg->exec();
		delete dlg;
		return;
	}
	switch(code)
	{
		case KEY_UP:
			express+="+";
			break;
		case KEY_DOWN:
			express+="-";
			break;
		case KEY_LEFT:
			express+="*";
			break;
		case KEY_RIGHT:
			express+="/";
			break;
		case KEY_CENTER:
			if(express.length() == 0){
				ans = QString::fromUtf8("请输入表达式");
				break;
			}
			if(data->isValid(express)){
				ansNum = data->genAns(express);
				//ans = QString("=%1").arg(ansNum);
				ans = data->stringFromDouble(ansNum, 8);
			}else{
				ans = QString::fromUtf8("表达式错误");
			}
			break;
	}
	switch(sta)
	{
		case num:
			{
				if(code>=KEY_0 && code<=KEY_9)
					express+=('0' + code - KEY_0);
				else if(code == KEY_STAR)
					express+=".";
				else if(code == KEY_SPOND)
					sta=func;
			}
			break;
		case func:
			{
				if(code>=KEY_0 && code<=KEY_9)
					express +=funcStr[code - KEY_0];
				else if(code == KEY_STAR)
					express+=funcStr[10];
				else if(code == KEY_SPOND)
					sta=num;
			}
			break;
	}
	repaint();
}

void mZWidget::drawButtonNum(QPainter *p)
{
	if(keyPressed && key==KEY_1) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(44,	180, QString("1"));
	if(keyPressed && key==KEY_2) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(115, 180, QString("2"));
	if(keyPressed && key==KEY_3) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(185, 180, QString("3"));

	if(keyPressed && key==KEY_4) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(44, 215, QString("4"));
	if(keyPressed && key==KEY_5) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(115, 215, QString("5"));
	if(keyPressed && key==KEY_6) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(185, 215, QString("6"));

	if(keyPressed && key==KEY_7) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(44, 255, QString("7"));
	if(keyPressed && key==KEY_8) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(115, 255, QString("8"));
	if(keyPressed && key==KEY_9) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(185, 255, QString("9"));

	if(keyPressed && key==KEY_STAR) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(44, 295, QString("."));
	if(keyPressed && key==KEY_0) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(115, 295, QString("0"));
}

void mZWidget::drawButtonFunc(QPainter *p)
{
	if(keyPressed && key==KEY_1) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(24,	180, funcStr[1]);
	if(keyPressed && key==KEY_2) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(95, 180, funcStr[2]);
	if(keyPressed && key==KEY_3) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(165, 180,funcStr[3]);

	if(keyPressed && key==KEY_4) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(24,	215, funcStr[4]);
	if(keyPressed && key==KEY_5) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(95,  215,funcStr[5]);
	if(keyPressed && key==KEY_6) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(165, 215,funcStr[6]);

	if(keyPressed && key==KEY_7) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(24,	255, funcStr[7]);
	if(keyPressed && key==KEY_8) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(95,  255,funcStr[8]);
	if(keyPressed && key==KEY_9) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(165, 255,funcStr[9]);

	if(keyPressed && key==KEY_STAR) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(24, 295, funcStr[10]);
	if(keyPressed && key==KEY_0) p->setPen(QColor(255,255,0));
	else p->setPen(QColor(0,0,0));
	p->drawText(95, 295, funcStr[0]);
}

void mZWidget::deleteEle()
{
#if 0
	if(express.length() == 0)
		return;
	data->praserStr(express);//解析字符
	item tItem = mStrList[mStrList.size() - 1];
	if(tItem.value == QString("(") && mStrList.size()>1)
	{
		item tItem2 = mStrList[mStrList.size() - 2];
		if(data->isFunc(tItem2.vale)){
			int len = tItem2.value.length();
			int oriLen = express.lenght();
			int start = oriLen - len;
			/* 删掉括号和前面的函数式 */
			express.remove(start-1, len+1);
			return;
		}
	}
#endif
	/*否则只删除最后一个字符*/
	if(express.length() > 0)
		express.remove(express.length()-1, 1);
}

QString mZWidget::getProgramDir()
{
	QString m_sProgramDir;
	m_sProgramDir = QString ( qApp->argv() [0] ) ;
	int i = m_sProgramDir.findRev ( "/" );
	m_sProgramDir.remove ( i+1, m_sProgramDir.length() - i );
	return m_sProgramDir;
}

