#ifndef NetworkCanvas_hxx
#define NetworkCanvas_hxx

#include <QtGui/QWidget>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QMenu>
#include <iostream>

class Map : public QWidget
{

	Q_OBJECT
public:
	enum Action
	{
		NoAction
	};
	enum Region
	{
		NoRegion
	};
	Map(QWidget * parent=0)
		: QWidget(parent)
		, _changed(false)
	{
		setMouseTracking(true);
		setAcceptDrops(true);
		setMinimumSize(200,100);
		resize(600,300);
		example1();
	}

	void example1()
	{
	}

	virtual ~Map();

	void paintEvent(QPaintEvent * event)
	{
		QPainter painter(this);
		paint(painter);
	}
public:
	void paint(QPainter & painter)
	{
	}
	void mouseMoveEvent(QMouseEvent * event)
	{
		std::cout << "Move" << std::endl;
	}
	void mousePressEvent(QMouseEvent * event)
	{
		std::cout << "Press" << std::endl;
	}
	void mouseReleaseEvent(QMouseEvent * event)
	{
		std::cout << "Release" << std::endl;
	}
	void mouseDoubleClickEvent(QMouseEvent * event)
	{
		std::cout << "Double click" << std::endl;
	}

	void contextMenuEvent(QContextMenuEvent * event)
	{
		QMenu menu(this);
		menu.move(event->globalPos());
//		menu.addAction(QIcon(":/icons/images/newprocessing.png"), "Add processing",
//			this, SLOT(onNewProcessing()))->setData(translatedPos(event));
		menu.exec();
	}

	void wheelEvent(QWheelEvent * event)
	{
	}

	bool event(QEvent * event)
	{
		return QWidget::event(event);
	}

signals:
	void changed();
public:
	bool isChanged()
	{
		return _changed;
	}
	void markAsChanged()
	{
		_changed = true;
		emit changed();
	}
	void clearChanges()
	{
		_changed = false;
	}
	
private:
	bool _changed;
};



#endif//NetworkCanvas_hxx
