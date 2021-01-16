#include "LogAnalyse.h"
 

//过滤器
Filter::Filter(QString title, QString t_and, QString t_or, QString t_exclude) {
	text_title = title;
	text_and = t_and;
	text_or = t_or;
	text_exclude = t_exclude;
}

QString Filter::toString()
{
	//return "";
	return QObject::tr("%1 -> %2,%3,%4").arg(text_title, text_and, text_or, text_exclude);
}

bool Filter::operator==(const Filter& other) const
{
	return (text_title == other.text_title) && (text_and == other.text_and) && (text_or == other.text_or) && (text_exclude == other.text_exclude);
}

//UI
LogAnalyse::LogAnalyse(QWidget* parent)	: QMainWindow(parent)
{
	ui.setupUi(this);
	//ui.label->setTextInteractionFlags(Qt::TextSelectableByMouse); //可选择

	ui.Edit_IfAnd->setPlaceholderText(From8Bit("一行日志中必须包含的全部内容，多个内容以‘;’号分割"));
	ui.Edit_IfOr->setPlaceholderText(From8Bit("一行日志中只需要包含其中之一，多个内容以‘;’号分割"));
	ui.Edit_IfExclude->setPlaceholderText(From8Bit("一行日志不能包含的内容，多个内容以‘;’号分割"));

	ui.Edit_Create_IfAnd->setPlaceholderText(From8Bit("过滤器条件 And"));
	ui.Edit_Create_IfOr->setPlaceholderText(From8Bit("过滤器条件 Or"));
	ui.Edit_Create_IfExclude->setPlaceholderText(From8Bit("过滤器条件 Exclude"));
	ui.Edit_Create_Title->setPlaceholderText(From8Bit("过滤器标题（必须输入）"));

	clear_AnalyseResult();

	//ui.Edit_InputFilePath->setText(From8Bit("C:/Users/A·Cap/Desktop/debug.log"));


	mFilterFilePath = qApp->applicationDirPath() + QDir::separator() + "filter.json";


	//显示过滤器列表
	readFilterFile();
	int size = mFilterArray.size();
	for (int i = 0; i != size; i++) {
		Filter filter = mFilterArray.at(i);
		insertFilterToListWidget(i, filter);
	}
}

//清空
void LogAnalyse::on_BT_Clear_clicked() {
	ui.Edit_IfAnd->setText("");
	ui.Edit_IfOr->setText("");
	ui.Edit_IfExclude->setText("");
}
//选择文件
void LogAnalyse::on_BT_CheckFile_clicked() {

	QString fileName = QFileDialog::getOpenFileName(this, From8Bit("请选择日志文件"), ".", tr("All files(*.*)"));
	if (!fileName.isEmpty()) {
		ui.Edit_InputFilePath->setText(fileName);
	}
}
//点击保存
void LogAnalyse::on_BT_Create_clicked()
{
	QString text_title = ui.Edit_Create_Title->text();
	QString text_if_and = ui.Edit_Create_IfAnd->text();
	QString text_if_or = ui.Edit_Create_IfOr->text();
	QString text_if_exclude = ui.Edit_Create_IfExclude->text();

	if (text_title.isEmpty()) {
		QMessageBox::information(NULL, From8Bit("注意"), From8Bit("请输入标题，以便于能快捷的找到过滤器"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}

	if (text_if_and.isEmpty() && text_if_or.isEmpty() && text_if_exclude.isEmpty()) {
		QMessageBox::information(NULL, From8Bit("注意"), From8Bit("无效的过滤器,没有过滤条件!"), QMessageBox::Yes, QMessageBox::Yes);
		return;
	}

	//添加过滤器
	Filter filter = Filter(text_title, text_if_and, text_if_or, text_if_exclude);
	mFilterArray.prepend(filter);
	writeFilterFile();
	insertFilterToListWidget(0, filter);

	//清理
	ui.Edit_Create_Title->clear();
	ui.Edit_Create_IfAnd->clear();
	ui.Edit_Create_IfExclude->clear();
	ui.Edit_Create_IfOr->clear();
}

//读写文件信息
void LogAnalyse::writeFilterFile()
{
	QFile file(mFilterFilePath);
	QJsonDocument document;
	QJsonArray qjar = document.array();
	int size = mFilterArray.size();
	for (int i = 0; i != size; i++) {
		Filter filter = mFilterArray.at(i);

		QJsonObject objs;
		objs.insert("title", filter.text_title);
		objs.insert("ifand", filter.text_and);
		objs.insert("ifor", filter.text_or);
		objs.insert("ifexclude", filter.text_exclude);
		//qjar.insert(i, objs);
		qjar.append(objs);
	}
	document.setArray(qjar);

	if (file.open(QIODevice::WriteOnly)) {
		file.write(document.toJson(QJsonDocument::Compact));
		file.close();
	}


}
//读写文件信息
void LogAnalyse::readFilterFile()
{
	QFile file(mFilterFilePath);
	QJsonDocument jdc;
	if (file.open(QIODevice::ReadOnly)) {
		QJsonParseError* error = new QJsonParseError;
		jdc = QJsonDocument::fromJson(file.readAll(), error);
		file.close();
	}
	if (jdc.isEmpty()) {
		jdc = QJsonDocument();
	}

	QJsonArray qjar = jdc.array();
	int size = qjar.size();
	for (int i = 0; i < size; i++) {
		QJsonObject qjo = qjar.at(i).toObject();
		QString title = qjo.value("title").toString();
		QString if_and = qjo.value("ifand").toString();
		QString if_or = qjo.value("ifor").toString();
		QString if_exclude = qjo.value("ifexclude").toString();

		Filter filter = Filter(title, if_and, if_or, if_exclude);
		mFilterArray.append(filter); 
	}
 
}

//插入过滤器到ListView中
void LogAnalyse::insertFilterToListWidget(int index, Filter& filter)
{
	QListWidgetItem* item = new QListWidgetItem(filter.toString());
	item->setTextAlignment(Qt::AlignLeft);
	item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
	 
	ui.List_Filter->insertItem(index, item); 
}
//列表项双击
void LogAnalyse::on_List_Filter_itemDoubleClicked(QListWidgetItem* item) {
	int index = ui.List_Filter->row(item);
	Filter  filter = mFilterArray.at(index);

	ui.Edit_IfAnd->setText(filter.text_and);
	ui.Edit_IfOr->setText(filter.text_or);
	ui.Edit_IfExclude->setText(filter.text_exclude);

}


//清理分析结果
void LogAnalyse::clear_AnalyseResult(){
	ui.Edit_AnalyzeResult->clear();
	ui.Label_RunInfo->setText(From8Bit("日志分析/耗时0秒"));
	ui.Label_RunNum->setText(From8Bit("共0行"));
}

//开始分析
void LogAnalyse::on_BT_Analyze_clicked() {
	QString FilePath = ui.Edit_InputFilePath->text();
	QString If_And = ui.Edit_IfAnd->text();
	QString If_Or = ui.Edit_IfOr->text();
	QString If_Exclude = ui.Edit_IfExclude->text();
	if (FilePath.isEmpty())return;
	analyse(FilePath , If_And , If_Or , If_Exclude);
}


void LogAnalyse::on_analyse_runing(const int& count, const long& elapsed) {
	ui.Label_RunNum->setText(From8Bit("共 %1 行").arg(count));
	ui.Label_RunInfo->setText(From8Bit("分析中../耗时 %1ms").arg(elapsed));
}
void LogAnalyse::on_analyse_end(const int& count, const long& elapsed) {
	ui.Label_RunNum->setText(From8Bit("共 %1 行").arg(count));
	ui.Label_RunInfo->setText(From8Bit("完成/耗时 %1ms").arg(elapsed));
}
void LogAnalyse::on_analyse_log(const QString& log) {
	ui.Edit_AnalyzeResult->appendHtml(log);
	//ui.label->setText(log);
}
