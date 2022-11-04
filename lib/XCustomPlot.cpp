#include "XCustomPlot.h"

XCustomPlot::XCustomPlot(QWidget* parent)
    : QCustomPlot(parent)
    , m_isShowTracer(false)
    , m_xTracer(Q_NULLPTR)
    , m_yTracer(Q_NULLPTR)
    , m_dataTracers(QList<XTracer *>())
    , m_lineTracer(Q_NULLPTR)
{
}

void XCustomPlot::mouseMoveEvent(QMouseEvent* event)
{
    QCustomPlot::mouseMoveEvent(event);

    if(m_isShowTracer)
    {
        //当前鼠标位置（像素坐标）
        int x_pos = event->pos().x();
        int y_pos = event->pos().y();

        //像素坐标转成实际的x,y轴的坐标
        float x_val = this->xAxis->pixelToCoord(x_pos);
        float y_val = this->yAxis->pixelToCoord(y_pos);

        if(Q_NULLPTR == m_xTracer)
        {
            m_xTracer = new XTracer(this, XTracer::XAxisTracer);    //x轴
        }
        m_xTracer->updatePosition(x_val, y_val);

        if(Q_NULLPTR == m_yTracer)
        {
            m_yTracer = new XTracer(this, XTracer::YAxisTracer);    //y轴
        }
        m_yTracer->updatePosition(x_val, y_val);

        int nTracerCount = m_dataTracers.count();
        int nGraphCount = graphCount();
        if(nTracerCount < nGraphCount)
        {
            for(int i = nTracerCount; i < nGraphCount; ++i)
            {
                XTracer* tracer = new XTracer(this, XTracer::DataTracer);
                m_dataTracers.append(tracer);
            }
        }
        else if(nTracerCount > nGraphCount)
        {
            for(int i = nGraphCount; i < nTracerCount; ++i)
            {
                XTracer* tracer = m_dataTracers[i];
                if(tracer)
                {
                    tracer->setVisible(false);
                }
            }
        }
        for (int i = 0; i < nGraphCount; ++i)
        {
            XTracer* tracer = m_dataTracers[i];
            if(!tracer)
            {
                tracer = new XTracer(this, XTracer::DataTracer);
            }
            tracer->setVisible(true);
            tracer->setPen(this->graph(i)->pen());
            tracer->setBrush(Qt::NoBrush);
            tracer->setLabelPen(this->graph(i)->pen());
            auto iter = this->graph(i)->data()->findBegin(x_val);
            double value = iter->mainValue();
//            double value = this->graph(i)->data()->findBegin(x_val)->value;
            tracer->updatePosition(x_val, value);
        }

        if(Q_NULLPTR == m_lineTracer)
        {
            m_lineTracer = new XTraceLine(this, XTraceLine::Both);    //直线
        }
        m_lineTracer->updatePosition(x_val, y_val);

        this->replot();//曲线重绘
    }
}
