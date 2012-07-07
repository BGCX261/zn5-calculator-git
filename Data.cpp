#include "Data.h"

item::item(QString _str, bool _isOp)
{
	value = _str;
	isOp = _isOp;
}

/********************************************/

Data::Data()
{
	initFuncList();
}

double Data::genAns(QString str)
{
	str = preCompile(str);
	praserStr(str); //TODO 需要去掉空格
	genLast();
	printList();
	return calResult();
}

QString Data::preCompile(QString _str)
{
	QString str=_str;
	/*去掉空格*/
	int index=0;
	cout<<"strOri = "<<str.latin1()<<endl;
	for(;;)
	{
		index = str.find(QString(" "));
		if(index != -1)
			str.replace(index, 1, "");
		else
			break;
	}
	/*替换常数*/
	index=0;
	for(;;)
	{
		index = str.find(QString("PI"));//圆周率
		if(index != -1)
			str.replace(index, 2, "3.141592653589793");
		else
			break;
	}
	index=0;
	for(;;)
	{
		index = str.find(QString("NA"));//阿伏加德罗常数
		if(index != -1)
			str.replace(index, 2, "6.02*10^23");
		else
			break;
	}
	index=0;
	for(;;)
	{
		index = str.find(QString("e"));//自然底数
		if(index != -1)
			str.replace(index, 1, "2.718281828");
		else
			break;
	}
	return str;
}

void Data::praserStr(QString str)//分解出操作数和操作符
{
	cout<<"原生式为:\t"<<str.latin1()<<endl;
	mStrList.clear();
	QString tmp=QString("");
	for(unsigned int i=0; i<str.length(); i++)
	{
		QChar ch = str.at(i);
		/* 负号单独处理 */
		if(i==0 
				&& ch==QChar('-') 
				&& str.length()>1 
				&& !isOperator(str[1]))//如果第一个符号是-则其为-号不为减号
		{
			tmp.append(ch);
			continue;
		}
		if(i>0 && ch==QChar('-'))//如果-前一个是操作符，则其为负号
		{
			QChar ch_pre = str.at(i-1);
			if(i == str.length() -1) continue;
			QChar ch_next = str.at(i+1);
			if(isOperator(ch_pre) 
					&& ch_pre != QChar(')') 
					&&!isOperator(ch_next))
			{
				tmp.append(ch);
				continue;
			}
		}
		if(isOperator(ch) && tmp.length()>0)
		{
			if(i>0 && !isOperator(str.at(i-1))){
				//cout<<"append tmp = "<<tmp.latin1()<<endl;
				item mItem = item(tmp, isFunc(tmp));
				tmp = QString("");//重置tmp为空
				mStrList.push_back(mItem);
			}
			item mItem = item(ch, true);
			mStrList.push_back(mItem);
		}else if(isOperator(ch) && tmp.length()==0){
			item mItem = item(ch, true);
			mStrList.push_back(mItem);
		}else
			tmp.append(ch);
	}
	if(tmp.length()>0)//最后可能的情况下补一个操作数
	{
		item mItem = item(tmp, false);
		mStrList.push_back(mItem);
	}
	cout<<"---------前缀式分解为----------"<<endl;
	for(unsigned int i=0; i<mStrList.size(); i++)
	{
		cout<<mStrList[i].value.latin1();
		cout<<"\t\t\t["<<mStrList[i].isOp<<"]"<<endl;
	}
	cout<<"-------------------------------"<<endl;
}

bool Data::isOperator(QChar ch)//判断是否为操作符
{
	if(ch == QChar('+') 
		|| ch==QChar('-') 
		|| ch==QChar('*') 
		|| ch==QChar('/') 
		|| ch==QChar('^') 
		|| ch==QChar('(') 
		|| ch==QChar(')')
		)
		return true;
	else
		return false;
}

void Data::printList()
{
	//qDebug("==================================");
	//cout<<"=================================="<<endl;
	//qDebug("list = ");
	//qDebug()"print mStrList.size = "<<mStrList.size();
	for(unsigned int i=0; i<mStrList.size(); i++)
	{
		//qDebug()mStrList[i].value;
		//cout<<mStrList[i].value.latin1()<<endl;
		//qDebug()mStrList[i].isOp;
	}
	QString outStr = "";
	for(unsigned int i=0; i<outList.size(); i++)
	{
		outStr += outList[i].value;
		outStr += " ";
	}
	//qDebug()"outStr = "<<outStr;
	//cout<<"outStr = "<<outStr.latin1()<<endl;
	//qDebug("==================================");
}

void Data::genLast()//求解后缀表达式(只针对+-*/())有效
{
	QString str;
	QChar ch;
	stackList.clear();
	outList.clear();
	////qDebug()"mStrList.size = "<<mStrList.size();
	for(unsigned int i=0; i<mStrList.size(); i++)
	{
		if(!mStrList[i].isOp)
		{
			str = mStrList[i].value;
			item mItem = item(str, false);
			outList.push_back(mItem);
		}else{
			ch = mStrList[i].value.at(0);
			QString funcStr = mStrList[i].value;
			if(isFunc(funcStr))//说明遇到子函数了,则直接压栈
			{
				stackList.push_back(funcStr);
				continue;
			}
			if(ch == QChar('('))//遇到开括号
			{
				stackList.push_back(ch);//直接压栈开括号
				continue;
			}
			if(ch == QChar(')'))//遇到闭括号，则弹出各操作符至开括号
			{
				while(stackList.size()>0)
				{
					QString str = stackList[stackList.size() -1];
					stackList.pop_back();
					QChar tmpCh = str.at(0);
					if(tmpCh != QChar('('))
					{
						item mItem = item(tmpCh, true);
						outList.push_back(mItem);
					}
					else
						break;
				}
				if(stackList.size() > 0){
					/* 如果开括号前是子函数，则弹出子函数 */
					QString strFunc = stackList[stackList.size() -1];
					if(isFunc(strFunc)){
						stackList.pop_back();//移除最后一个
						item mItem = item(strFunc, true);
						outList.push_back(mItem);
					}
				}
				continue;
			}
			if(stackList.size()==0)//如果操作符栈为空，则直接添加
			{
				stackList.push_back(ch);
			}
			else{
					QChar tmpCh = stackList[stackList.size()-1].at(0);
					int ans = AbigB(tmpCh, ch);
					if(ans == 0)//优先级相等
					{
						/* 弹出操作符栈中的符号 */
						QChar mCh = stackList[stackList.size() -1].at(0);
						stackList.pop_back();
						item mItem = item(mCh, true);
						outList.push_back(mItem);

						/* 压栈当前符号 */
						stackList.push_back(ch);
					}
					else if(ans == -1)//tmpCh < ch
					{
						stackList.push_back(ch);
					}
					else if(ans == 1)//tmpCh > ch
					{
						while(stackList.size()>0)
						{
							QChar mCh = stackList[stackList.size() -1].at(0);
							stackList.pop_back();
							if(mCh == QChar('('))
							{
								stackList.push_back(mCh);
								break;
							}
							item mItem = item(mCh, true);
							outList.push_back(mItem);
						}
						stackList.push_back(ch);
					}
			}
		}
	}
	//qDebug("++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	while(stackList.size()>0)//最后弹出所有操作符栈中的符号
	{
		//QChar mCh = stackList.pop_back().at(0);
		QChar mCh = stackList[stackList.size()-1].at(0);
		stackList.pop_back();
		////qDebug()"mCh = "<<mCh;
		item mItem = item(mCh, true);
		outList.push_back(mItem);
	}

	//qDebug("++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
#if 1
	QString outstr;//输出后缀表达式
	for(unsigned int i=0; i<outList.size(); i++)
		outstr += (outList[i].value + " ");
	cout<<"后缀式outList=\t["<<outstr.latin1()<<"]"<<endl;
#endif
}

int Data::AbigB(QChar a, QChar b)/*优先级比较 0相等 -1小于 1大于 */
{
	if(a=='-') a='+';
	if(a=='/') a='*';
	if(b=='-') b='+';
	if(b=='/') b='*';
	//if(a==')') a='(';
	if(a=='(') return -1;
	int indexA=0;
	int indexB=0;
	int ans=0;
	QChar op[4] = {'+', '*', '^', '('};
	for(int i=0; i<4; i++)
	{
		if(a==op[i]) indexA=i;
		if(b==op[i]) indexB=i;
	}
	//for(int i=0; i<4; i++)
	if(indexA < indexB) ans = -1;
	if(indexA == indexB) ans = 0;
	if(indexA > indexB) ans = 1;
	return ans;
}

double Data::calResult()//根据后缀表达式，出栈求出结果
{
	//QStringList dList;//操作数
	vector<QString> dList;//操作数
	QString ans;
	dList.clear();
	if(outList.size() == 1){
		cout<<"outList 只有一个数"<<endl;
		ans = outList[0].value;
		return ans.toDouble();
	}
	////qDebug()"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
	while(outList.size()>0)
	{
		//item tmpItem = outList.takeFirst();
		item tmpItem = outList[0];
		//outList.pop_front();
		outList.erase(outList.begin());
		if(!tmpItem.isOp)
			dList.push_back(tmpItem.value);
		else{
			/* 如果遇到子函数，则弹出一个操作数，运算并将结果压栈*/
			QString funcStr = tmpItem.value;
			if(isFunc(funcStr))
			{
				QString a = dList[dList.size() -1];
				dList.pop_back();//弹出操作数
				ans = calFunc(funcStr, a);//计算子函数结果
				dList.push_back(ans);
			}else{//计算四则运算
				QString a = dList[dList.size() -1];
				dList.pop_back();
				QString b = dList[dList.size() -1];
				dList.pop_back();
				//QString b = dList.pop_back();
				QChar ch = tmpItem.value.at(0);
				ans = cal(a, b, ch);
				dList.push_back(ans);
			}
		}
	}
	return ans.toDouble();
	////qDebug()"ans ="<<ans;
	////qDebug()"<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<";
}

QString Data::cal(QString _a, QString _b, QChar op)//计算四则运算
{
	double a = _a.toDouble();
	double b = _b.toDouble();
	double ans=0.0;
	if(op == QChar('+'))
	{
		ans = b+a;
	}
	else if(op == QChar('-'))
	{
		ans = b-a;
	}
	else if(op == QChar('*'))
	{
		ans = b*a;
	}
	else if(op == QChar('/'))
	{
		ans = (double)b/a;
	}
	else if(op == QChar('^'))//计算次方
	{
		/*
		ans = 1;
		for(int i=0; i<a; i++)
			ans*=b;
			*/
		ans = pow(b, a);
	}
	QString ansStr = QString("%1").arg(ans);
	return ansStr;
}

void Data::initFuncList()
{
	funcList.clear();
	QString funcStr[] = {
		"sin",
		"abs",
		"sqrt",
		"asin",
		"cos",
		"acos",
		"tan",
		"atan",
		"exp",
		"ln",
		"log"
	};
	for(unsigned int i=0; i<sizeof(funcStr)/sizeof(QString); i++)
	{
		funcList.push_back(funcStr[i]);
	}
}

bool Data::isFunc(QString name)
{
	bool ans = false;
	for(unsigned int i=0; i<funcList.size(); i++)
		if(name == funcList[i]){
			ans = true;
			break;
		}
	return ans;
}

QString Data::calFunc(QString funcName, QString _value)//计算函数值
{
	double ans=0.0;
	double value = _value.toDouble();
	if(funcName == QString("abs"))
	{
		ans = abs(value);
	}
	else if(funcName == QString("sqrt"))
	{
		ans = sqrt(value);
	}
	else if(funcName == QString("sin"))
	{
		ans = sin(value);
	}
	else if(funcName == QString("asin"))
	{
		ans = asin(value);
	}
	else if(funcName == QString("cos"))
	{
		ans = cos(value);
	}
	else if(funcName == QString("acos"))
	{
		ans = acos(value);
	}
	else if(funcName == QString("tan"))
	{
		ans = tan(value);
	}
	else if(funcName == QString("atan"))
	{
		ans = atan(value);
	}
	else if(funcName == QString("exp"))
	{
		ans = exp(value);
	}
	else if(funcName == QString("ln"))
	{
		ans = log(value);
	}
	else if(funcName == QString("log"))
	{
		ans = log10(value);
	}
	return QString("%1").arg(ans);
}

bool Data::isValid(QString str)
{
	bool ans = true;
	str = preCompile(str);
	praserStr(str);
	/* 1.括号匹配 */
	int num=0;
	int indexA=0;
	int indexB=0;
	for(;;)
	{
		indexA = str.find(QString("("), indexA);
		if(indexA != -1) {num++;indexA++;}
		indexB = str.find(QString(")"), indexB);
		if(indexB != -1) {num--;indexB++;}
		cout<<"indexA="<<indexA<<",indexB="<<indexB<<endl;
		if(indexB<indexA && indexA!=-1 &&indexB!=-1){
			cout<<"闭括号在开括号之前"<<endl;
			ans = false;
			goto over;
		}
		if(indexA==-1 || indexB==-1) break;
	}
	if(num != 0){
		cout<<"开闭括号数目不对"<<endl;
		ans = false;
		goto over;
	}
	/* 2. 开括号后，闭括号前，不能为操作符*/
	for(unsigned int i=0; i<mStrList.size(); i++)
	{
		if(mStrList[i].value == "("){
			if(i > 0){
				if(!mStrList[i-1].isOp){
					cout<<"开括号前非操作符"<<endl;
					ans = false;
					goto over;
				}
			}
			if(i < mStrList.size()-1 
					&& mStrList[i+1].isOp 
					&& mStrList[i+1].value!="-"
					&& !isFunc(mStrList[i+1].value)){
				cout<<"开括号后为非负号的操作符"<<endl;
				ans = false;
				goto over;
			}
		}
		if(mStrList[i].value == ")"){
			if(i>0 && mStrList[i-1].isOp
					&& mStrList[i-1].value != ")"){
				cout<<"闭括号前为符号"<<endl;
				ans = false;
				goto over;
			}
		}
	}
	/* 3.连续操作符*/
	for(unsigned int i=0; i<mStrList.size(); i++)
	{
		if(mStrList[i].isOp 
				&& mStrList[i].value != "(" 
				&& mStrList[i].value != ")")//括号的情况前面已经计算好了
		{
			if(i<mStrList.size() - 1){
				if(mStrList[i+1].isOp 
						&& mStrList[i+1].value != "-" 
						&& mStrList[i+1].value != "("
						&& !isFunc(mStrList[i+1].value))//前面为非负号字符
				{
					cout<<"连续两个非负号非亦非括号运算符"<<endl;
					ans = false;
					goto over;
				}
				if(mStrList[i+1].value == "-" && i==mStrList.size() -2)
				{
					cout<<"最后一个为负号"<<endl;
					goto over;
				}
			}else{
				cout<<"最后一个字符为操作符"<<endl;
				ans = false;
				goto over;
			}
		}
	}
	/* 第一个为操作符*/
	if(mStrList.size() > 0)
	{
		if(mStrList[0].isOp 
				&& mStrList[0].value != "-"
				&& mStrList[0].value != "("
				&& !isFunc(mStrList[0].value)){
			cout<<"第一个字符为操作符"<<endl;
			ans = false;
			goto over;
		}
		if(mStrList[0].isOp && mStrList.size() == 1){
			cout<<"仅有一个符号"<<endl;
			ans = false;
			goto over;
		}
		if(mStrList[0].value == "-" && mStrList.size() > 1 && mStrList[1].isOp){
			cout<<"首负号连接新号"<<endl;
			ans = false;
			goto over;
		}
	}
over:
	return ans;
}
