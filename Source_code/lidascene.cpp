#include "lidascene.h"

LidaScene::LidaScene()
{
//    pixmap_item = new QGraphicsPixmapItem();
}

void LidaScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}

void LidaScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    int x = mouseEvent->scenePos().toPoint().x();
    int y = mouseEvent->scenePos().toPoint().y();

    cout << "x: " << x << " "
         << "y: " << y << " "
//         << endl;
         // << "r: " << image->pixelColor(x,y).red() << " "
         // << "g: " << image->pixelColor(x,y).green() << " "
         // << "b: " << image->pixelColor(x,y).blue() << endl;

        // << lidaimage << endl;


         << "r: " << lidaimage->matrix3D[0][x][y] << " "
         << "g: " << lidaimage->matrix3D[1][x][y] << " "
         << "b: " << lidaimage->matrix3D[2][x][y] << endl;
////         << "r: " << std::abs(freq->frequency[0][x][y]) << " "
////         << "g: " << std::abs(freq->frequency[1][x][y]) << " "
////         << "b: " << std::abs(freq->frequency[2][x][y]) << endl;
}

void LidaScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{

}
