// $Id$
// QtLobby released under the GPLv3, see COPYING for details.
#ifndef MAPRENDERERWIDGET_H
#define MAPRENDERERWIDGET_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QImage>
#include <QVector>
#include <QTime>
#include "RawHeightMap.h"
#include "glextensions.h"
#include "ShaderSet.h"

class Vertex {
public:
    float x;
    float y;
    float z;
    Vertex();
    Vertex(float x, float y, float z);
    void setXYZ(float x, float y, float z);
    void add(Vertex v2);
    void sub(Vertex v2);
    static Vertex getNormal(Vertex& v1, Vertex& v2);
    static Vertex getNormal(Vertex& a, Vertex& b, Vertex& c);
    void normalize();
};

struct TexCoord {
    float u;
    float v;
};

class MapRendererWidget : public QGLWidget {
    Q_OBJECT
public:
    MapRendererWidget(QWidget* parent = 0);
    ~MapRendererWidget();
    void setSource(QString mapName, QImage minimap, QImage metalmap, RawHeightMap heightmap);
    void setDrawStartPositions(bool b);

signals:
    void updateDebugInfo(const QString& info);

public slots:
    void setRotation(int xAngle, int yAngle, int zAngle);
    void addStartRect(int ally, QRect r);
    void setMyAllyTeam(int n);
    void removeStartRect(int ally);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);
    void hideEvent(QHideEvent * event);
private:
    void normalizeAngle(int *angle);
    void makeObject();
    void computeNormals();
    void generateIndexes();
    void generateTexCoords();
    void drawStartRecs();
    void initPermTexture();
private:
    bool m_computedNormals;
    QString currentMap;
    unsigned int* m_indexes;
    int m_numIndexes;
    GLuint m_texture;
    GLuint m_permTexture;
    bool compileObject;
    QPoint lastPos;
    float lastZoom;
    QImage m_minimap;
    QImage m_metalmap;
    QImage m_withRects;
    RawHeightMap m_heightmap;
    QVector<QVector<Vertex> > vertexes;
    unsigned int m_vertexNumber;
    Vertex* m_vertexes;
    Vertex* m_normals;
    TexCoord* m_texCoords;
    unsigned int m_VBOVertices;
    unsigned int m_VBOTexCoords;
    unsigned int m_VBONormals;
    bool blockRerender;
    int xRot;
    int yRot;
    int zRot;
    float dx;
    float dy;
    QMap<int, QRect> startRects;
    int myAlly;
    bool m_redrawStartRects;
    bool m_drawStartPositions;
    Qt::BrushStyle m_brushStyle;
    int m_borderWidth;
    int m_alpha;
    QTime m_time;
    QTimer m_timer;
    QTime m_lightTime;
    QString m_debugInfo;
    bool m_perspective;
    ShaderSet m_waterShaderSet;
    GLint m_waterTimeLoc;
    GLint m_waterpermTextureLoc;
    GLint m_lightSourceLoc;
};

inline void MapRendererWidget::setDrawStartPositions(bool b) {
    m_drawStartPositions = b;
}

/*inline double findnoise2(double x,double y) {
    int n=(int)x+(int)y*57;
    n=(n<<13)^n;
    int nn=(n*(n*n*60493+19990303)+1376312589)&0x7fffffff;
    return 1.0-((double)nn/1073741824.0);
}

inline double interpolate(double a,double b,double x) {
    double ft=x * 3.1415927;
    double f=(1.0-cos(ft))* 0.5;
    return a*(1.0-f)+b*f;
}
*/


#endif // MAPRENDERERWIDGET_H
