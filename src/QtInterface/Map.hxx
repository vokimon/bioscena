#ifndef NetworkCanvas_hxx
#define NetworkCanvas_hxx

#include <QtGui/QWidget>
#include <QtGui/QApplication>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QThread>
#include <QtCore/QTimerEvent>
#include <QtGui/QPainter>
#include <QtGui/QMouseEvent>
#include <QtGui/QMenu>
#include <iostream>
#include "Toroid.hxx"
#include "Assert.hxx"

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
	enum Dimensions
	{
		cellWidth = 22,
		cellHeight = 22,
		cellGap = 1
	};
	Map(QWidget * parent=0)
		: QWidget(parent)
		, _firstCell(0)
		, _selection(0)
		, _action(NoAction)
		, _updatePending(0)
		, _bugPixmap("/usr/share/emoticons/Default/angry.png")
		, _timer(this)
	{
		setMouseTracking(true);
		setAcceptDrops(true);
		setMinimumSize(200,100);
		resize(600,300);

		QAction * scrollDown = new QAction(tr("Scroll down"),this);
		addAction(scrollDown);
		scrollDown->setShortcut(tr("CTRL+Down"));
		connect( scrollDown, SIGNAL(triggered()), this, SLOT(scrollDown()) );

		QAction * scrollUp = new QAction(tr("Scroll up"),this);
		addAction(scrollUp);
		scrollUp->setShortcut(tr("CTRL+Up"));
		connect( scrollUp, SIGNAL(triggered()), this, SLOT(scrollUp()) );

		QAction * scrollLeft = new QAction(tr("Scroll left"),this);
		addAction(scrollLeft);
		scrollLeft->setShortcut(tr("CTRL+Left"));
		connect( scrollLeft, SIGNAL(triggered()), this, SLOT(scrollLeft()) );

		QAction * scrollRight = new QAction(tr("Scroll right"),this);
		addAction(scrollRight);
		scrollRight->setShortcut(tr("CTRL+Right"));
		connect( scrollRight, SIGNAL(triggered()), this, SLOT(scrollRight()) );

		QAction * moveS = new QAction(tr("Move south"),this);
		addAction(moveS);
		moveS->setShortcut(tr("Down"));
		connect( moveS, SIGNAL(triggered()), this, SLOT(moveS()) );

		QAction * moveN = new QAction(tr("Move north"),this);
		addAction(moveN);
		moveN->setShortcut(tr("Up"));
		connect( moveN, SIGNAL(triggered()), this, SLOT(moveN()) );

		QAction * moveW = new QAction(tr("Move west"),this);
		addAction(moveW);
		moveW->setShortcut(tr("Left"));
		connect( moveW, SIGNAL(triggered()), this, SLOT(moveW()) );

		QAction * moveE = new QAction(tr("Move east"),this);
		addAction(moveE);
		moveE->setShortcut(tr("Right"));
		connect( moveE, SIGNAL(triggered()), this, SLOT(moveE()) );

		QAction * dropBug = new QAction(tr("Drop bug"),this);
		addAction(dropBug);
		dropBug->setShortcut(tr("Return"));
		connect( dropBug, SIGNAL(triggered()), this, SLOT(dropBug()) );

		example1();
	}

	typedef bool Substrat;
	std::vector<Substrat> _substrat;
	void example1()
	{
		_topology = new Bioscena::Toroid(60, 40);
		_substrat.resize(_topology->size()/3, true);
		_substrat.resize(_topology->size(), false);
		connect(&_timer, SIGNAL(timeout()), this, SLOT(tick()));
		_timer.setSingleShot(true);
		_timer.start(500);
	}

	virtual ~Map();

	void paintEvent(QPaintEvent * event)
	{
		QTime startTime;
		startTime.start();
		QPainter painter(this);
		paint(painter);
		static unsigned count = 0;
		_updatePending=0;
		lastPaintTime = startTime.elapsed();
		std::cout << "." << lastPaintTime << std::flush;
	}
	void queryUpdate()
	{
		if (_updatePending) std::cout << "Pending " << _updatePending << std::endl;
		if (_updatePending++) return;
		update();
	}
public slots:
	void tick()
	{
		bool changed = false;
		for (unsigned i=0; i<_topology->size(); i++)
		{
			if (!_substrat[i]) continue;
			Bioscena::Toroid::Position newPos = _topology->displaceRandomly(i,1);
			if (newPos >= _topology->size())
			{
				std::cout << "Moving from " << i << " to illegal " << newPos << std::endl;
				KKEP_ASSERT(false, "Invalid position");
			}
			if (_substrat[newPos]) continue; // Destination busy
			_substrat[i]=false;
			_substrat[newPos]=true;
			changed = true;
		}
		usleep(100000);
		qApp->flush();
		usleep(100000);
		qApp->processEvents();
		qApp->flush();
		if (changed)
		{
			std::cout << "Tick " << std::flush;
			queryUpdate();
		}
		_timer.start(500 + lastPaintTime);
	}
	void scrollDown()
	{
		using Bioscena::Toroid;
		scroll(Toroid::displaceVector(Toroid::S));
		std::cout << "Down:" << _firstCell << std::flush;
	}
	void scrollUp()
	{
		using Bioscena::Toroid;
		scroll(Toroid::displaceVector(Toroid::N));
		std::cout << "Up:" << _firstCell << std::flush;
	}
	void scrollLeft()
	{
		using Bioscena::Toroid;
		scroll(Toroid::displaceVector(Toroid::W));
		std::cout << "Left:" << _firstCell << std::flush;
	}
	void scrollRight()
	{
		using Bioscena::Toroid;
		scroll(Toroid::displaceVector(Toroid::E));
		std::cout << "Right:" << _firstCell << std::flush;
	}
	void moveN()
	{
		uint32 selRow = _topology->row(_selection);
		uint32 firstRow = _topology->row(_firstCell);
		if (selRow==firstRow) scrollUp();
		using Bioscena::Toroid;
		_selection = _topology->displace(_selection,
				Toroid::displaceVector(Toroid::N));
		queryUpdate();
		std::cout << "MoveN " << std::flush;
	}
	void moveS()
	{
		uint32 nRows = _topology->height();
		uint32 selRow = _topology->row(_selection);
		uint32 firstRow = _topology->row(_firstCell);
		if (((nRows+selRow)-firstRow)%nRows >= height()/cellHeight -1) scrollDown();
		using Bioscena::Toroid;
		_selection = _topology->displace(_selection,
				Toroid::displaceVector(Toroid::S));
		queryUpdate();
		std::cout << "MoveS " << std::flush;
	}
	void moveE()
	{
		uint32 nCols = _topology->width();
		uint32 selCol = _topology->col(_selection);
		uint32 firstCol = _topology->col(_firstCell);
		if (((nCols+selCol)-firstCol)%nCols >= width()/cellWidth -1) scrollRight();
		using Bioscena::Toroid;
		_selection = _topology->displace(_selection,
				Toroid::displaceVector(Toroid::E));
		queryUpdate();
		std::cout << "MoveE " << std::flush;
	}
	void moveW()
	{
		uint32 selCol = _topology->col(_selection);
		uint32 firstCol = _topology->col(_firstCell);
		if (selCol==firstCol) scrollLeft();
		using Bioscena::Toroid;
		_selection = _topology->displace(_selection,
				Toroid::displaceVector(Toroid::W));
		queryUpdate();
		std::cout << "MoveW " << std::flush;
	}
	void dropBug()
	{
		_substrat[_selection] = true;
		queryUpdate();
		std::cout << "Drop " << std::flush;
	}
private:
	void scroll(Bioscena::Toroid::Displacement dir)
	{
		_firstCell = _topology->displace(_firstCell, dir);
		queryUpdate();
	}
	unsigned normHeight(int height)
	{
		return height % _topology->height();
	}
	unsigned normWidth(int width)
	{
		return width % _topology->width();
	}
	unsigned normPos(int pos)
	{
		return pos % _topology->size();
	}
	void paint(QPainter & painter)
	{
		painter.setBrush(Qt::transparent);
		unsigned nCols = width() / cellWidth + 1;
		unsigned nRows = height() / cellHeight + 1;
		Bioscena::Toroid::Position rowPos = _firstCell;
		painter.setPen(QPen(Qt::red,10));
		for (unsigned j = 0; j< nRows; j++)
		{
			Bioscena::Toroid::Position pos = rowPos;
			for (unsigned i = 0; i< nCols; i++)
			{
				paintCell(painter, pos, i, j);
				pos++;
				if (pos>=_topology->size()) pos = 0;
			}
			rowPos+=_topology->width();
			rowPos = normPos(rowPos);
		}
		painter.setPen(Qt::black);
		for (unsigned zeroCol = _topology->width() - _topology->col(_firstCell); zeroCol<nCols; zeroCol+=_topology->width())
		{
			painter.drawLine(zeroCol*cellWidth,0,zeroCol*cellWidth,height());
		}
		painter.drawRect(0,0,_topology->width()*cellWidth,_topology->height()*cellHeight);

		painter.setPen(QPen(Qt::blue,2));
		uint32 x = _topology->col(_selection);
		uint32 y = _topology->row(_selection);
		uint32 xv = ((_topology->width()+x)-_topology->col(_firstCell))%_topology->width();
		uint32 yv = ((_topology->height()+y)-_topology->row(_firstCell))%_topology->height();
		painter.drawRect(xv*cellWidth,yv*cellHeight,cellWidth,cellHeight);
		painter.setPen(Qt::black);
		painter.drawText(10,10, tr("%1: col %2, row %3").arg(_selection).arg(xv).arg(yv));
	}
	void paintCell(QPainter & painter, Bioscena::Toroid::Position pos, unsigned i, unsigned j)
	{
		QRect cell(i*cellWidth,j*cellHeight,cellWidth,cellHeight);
//		painter.drawRect(cell);
//		painter.drawText(cell,QString::number(pos));
		if (pos == 0)
		{
			painter.setPen(QColor(0x77,0xa7,0x77)); 
			painter.setBrush(QColor(0x70,0x7a,0x70));
			painter.drawRect(cell);
			painter.setBrush(Qt::transparent);
		}
		if (_substrat[pos])
			painter.drawPixmap(cell,_bugPixmap);
	}
protected:
	void mouseMoveEvent(QMouseEvent * event)
	{
		std::cout << "Move " << std::flush;
	}
	void mousePressEvent(QMouseEvent * event)
	{
		std::cout << "Press " << std::flush;
	}
	void mouseReleaseEvent(QMouseEvent * event)
	{
		std::cout << "Release " << std::flush;
	}
	void mouseDoubleClickEvent(QMouseEvent * event)
	{
		std::cout << "DoubleClick " << std::flush;
	}
	void keyPressEvent(QKeyEvent * event)
	{
		std::cout << "KeyPress '" << std::hex << event->key() << std::dec << "'"<< std::flush;
		QWidget::keyPressEvent(event);
	}

	void contextMenuEvent(QContextMenuEvent * event)
	{
		QMenu menu(this);
		menu.move(event->globalPos());
		menu.addAction(QIcon(":/icons/images/newprocessing.png"), "Add processing",
			this, SLOT(dropBug()))->setData(event->pos());
		menu.exec();
	}

	void wheelEvent(QWheelEvent * event)
	{
	}

	bool event(QEvent * event)
	{
		return QWidget::event(event);
	}

private:
	Bioscena::Toroid * _topology;
	Bioscena::Toroid::Position _firstCell;
	Bioscena::Toroid::Position _selection;
	Action _action;
	int _updatePending;
	QPixmap _bugPixmap;
	QTimer _timer;
	int lastPaintTime;
};



#endif//NetworkCanvas_hxx
