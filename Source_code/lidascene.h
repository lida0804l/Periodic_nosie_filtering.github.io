#ifndef LIDASCENE_H
#define LIDASCENE_H

#include <QGraphicsView>
#include <iostream>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPixmapItem>
#include <lidaimage.h>
#include <fouriertransform.h>
//#include <lida.h>

using namespace std;

class LidaScene : public QGraphicsScene
{
public:
    LidaScene();

    QImage* image;
    FourierTransform* freq;

    LidaImage* lidaimage;
    QGraphicsPixmapItem* pixmap_item;

//private:


protected:
    void mousePressEvent(QGraphicsSceneMouseEvent*);

    void mouseMoveEvent(QGraphicsSceneMouseEvent*);

    void mouseReleaseEvent(QGraphicsSceneMouseEvent*);
};

#endif // LIDASCENE_H
