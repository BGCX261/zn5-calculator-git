#ifndef DATA_H_
#define DATA_H_

#include <qstring.h>
#include <qlist.h>
#include <qstringlist.h>
#include<vector>
#include<iostream>
#include<cmath>
using namespace std;

class item
{
	public:
		item(QString str, bool isOp);
		QString value;
		bool isOp;
};

//typedef QList<item> itemList;
//typedef vector<item> itemList;
class Data
{
	public:
		//Data(QString str);
		Data();
		void printList();
		double genAns(QString str);
		vector<item> mStrList;
		void praserStr(QString str);
		bool isFunc(QString name);//判断是否是函数式
		/*表达式合法检测*/
		bool isValid(QString str);
	private:
		//itemList mStrList;
		vector<QString> stackList;
		vector<item> outList;
		//void praserStr(QString str);
		bool isOperator(QChar ch);
		void genLast();
		double calResult();//从后缀表达式算出结果
		QString preCompile(QString str);//预处理字符串
		QString cal(QString a, QString b, QChar op);
		int AbigB(QChar a, QChar b);

		/* 函数计算相关 */
		vector<QString> funcList;//一些常用函数
		vector<QString> subList;//分解出的子串
		void initFuncList();//增加常用函数集
		QString calFunc(QString funcName, QString value);
};

#endif /* DATA_H_ */

