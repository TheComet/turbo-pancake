#include "CollisionUtil.h"
#include "Globals.h"



Line::Line() : p1(),p2() { }
Line::Line(Vector2 a,Vector2 b) : p1(a),p2(b) { }
Line::Line(double a,double b,double c,double d) : p1(a,b),p2(c,d) { }
void Line::render(const Camera& c) const {
    Vector2 a=c.worldToPixels(p1);
    Vector2 b=c.worldToPixels(p2);
    SDL_RenderDrawLine(g.renderer,(int)a.x,(int)a.y,(int)b.x,(int)b.y);
}

void Line::project(Vector2 &p,double &t) const {
    double d2=(p2-p1).length2(); //length squared
    t=(p2-p1)*(p-p1)/d2; 
    //note: t=0 when p=p1, t=1 when p=p2, varies linearly inbetween, and is a projection
    //onto (p2-p1), so this must be the function we're looking for.

    p=lerp(t,p1,p2);
}

Rectangle::Rectangle() : x0(0),y0(0),x1(0),y1(0) { }
Rectangle::Rectangle(double a,double b,double c,double d) : x0(a),y0(b),x1(c),y1(d) { }
double Rectangle::getWidth() { return x1-x0; }
double Rectangle::getHeight() { return y1-y0; }
void Rectangle::render(const Camera& c) { }

Quadrilateral::Quadrilateral() : p0(),p1(),p2(),p3() { }
Quadrilateral::Quadrilateral(Vector2 a,Vector2 b,Vector2 c,Vector2 d) : p0(a),p1(b),p2(c),p3(d) { }
void Quadrilateral::render(const Camera& cam) {
    Vector2 a=cam.worldToPixels(p0);
    Vector2 b=cam.worldToPixels(p1);
    Vector2 c=cam.worldToPixels(p2);
    Vector2 d=cam.worldToPixels(p3);
    SDL_RenderDrawLine(g.renderer,(int)a.x,(int)a.y,(int)b.x,(int)b.y);
    SDL_RenderDrawLine(g.renderer,(int)b.x,(int)b.y,(int)c.x,(int)c.y);
    SDL_RenderDrawLine(g.renderer,(int)c.x,(int)c.y,(int)d.x,(int)d.y);
    SDL_RenderDrawLine(g.renderer,(int)d.x,(int)d.y,(int)a.x,(int)a.y);
}
bool pnpoly(std::vector<Vector2> vert,Vector2 test)
{
    size_t nvert=vert.size();
    size_t i,j=0;
    bool c = false;
    for (i = 0,j = nvert-1; i < nvert; j = i++) {
        if (((vert[i].y>test.y) != (vert[j].y>test.y)) &&
            (test.x < (vert[j].x-vert[i].x) * (test.y-vert[i].y) / (vert[j].y-vert[i].y) + vert[i].x))
            c = !c;
    }
    return c;
}

bool doesCirclePointCollide(Vector2 circlepos,double r,Vector2 point) {
    return (circlepos-point).length()<r;
}
bool doesCircleLineCollide(Vector2 circlepos,double r,Line line) {
    double t=0; Vector2 p=circlepos;
    line.project(p,t); //populate the p and t values.
    if (t<0) {
        return doesCirclePointCollide(circlepos,r,line.p1);
    }
    else if (t>1) {
        return doesCirclePointCollide(circlepos,r,line.p2);
    }
    else {
        return doesCirclePointCollide(circlepos,r,p);
    }
}
bool doesPointQuadCollide(Vector2 pointpos,Quadrilateral quad) {
    std::vector<Vector2> lst;
    lst.push_back(quad.p0);
    lst.push_back(quad.p1);
    lst.push_back(quad.p2);
    lst.push_back(quad.p3);
    return pnpoly(lst,pointpos);
}
bool doesCircleQuadCollide(Vector2 circlepos,double r,Quadrilateral quad) {
    return doesPointQuadCollide(circlepos,quad)||
        doesCircleLineCollide(circlepos,r,Line(quad.p0,quad.p1))||
        doesCircleLineCollide(circlepos,r,Line(quad.p1,quad.p2))||
        doesCircleLineCollide(circlepos,r,Line(quad.p2,quad.p3))||
        doesCircleLineCollide(circlepos,r,Line(quad.p3,quad.p0));
}


//Pushes a circle so it is no longer intersect with a point. So (  .)  -> (   ). :)
void circlePointCollide(Vector2 &circlepos,double r,Vector2 point) {
    Vector2 diff=circlepos-point;
    double distance=diff.length();
    if (distance<r && distance>0)
        circlepos=point+diff*(r/distance); //put the circle a distance r away from point, without changing direction.
}

//Pushes a circle so it is no longer intersect with a line segment or its endpoints.
void circleLineCollide(Vector2 &circlepos,double r,Line line) {
    double t=0; Vector2 p=circlepos;
    line.project(p,t); //populate the p and t values.
    if (t<0) { 
        //collide with one endpoint.
        circlePointCollide(circlepos,r,line.p1);
    }
    else if (t>1) { 
        //collide with the other endpoint.
        circlePointCollide(circlepos,r,line.p2);
    }
    else { 
        //collide with the line. This is equivalent to colliding with the projected point.
        circlePointCollide(circlepos,r,p);
    }
}

//Pushes a circle so it is no longer intersecting with a rectangle
void circleRectangleCollide(Vector2 &circlepos,double r,Rectangle rect) {
    //Taking the lazy way out :)
    //the two horizontal lines
    Line line1=Line(rect.x0,rect.y0,rect.x1,rect.y0);
    Line line2=Line(rect.x0,rect.y1,rect.x1,rect.y1);
    //the two vertical lines
    Line line3=Line(rect.x0,rect.y1,rect.x0,rect.y0);
    Line line4=Line(rect.x1,rect.y1,rect.x1,rect.y0);
    circleLineCollide(circlepos,r,line1);
    circleLineCollide(circlepos,r,line2);
    circleLineCollide(circlepos,r,line3);
    circleLineCollide(circlepos,r,line4);
}

//Pushes a circle so it is no longer intersecting with a circle
void circleCircleCollide(Vector2 &c1,double r1,Vector2 c2,double r2) {
    //A circle hitting another circle acts like a circle with twice the radius hitting a point!
    circlePointCollide(c1,r1+r2,c2);
}

//Pushes two circles to make sure they are no longer intersecting.
//Pushing conserves "center of mass", c1*r1^2+c2*r2^2. This means big circles
//push small circles more easily, and it's difficult for small circles to push big circles.
void circleCirclePush(Vector2 &c1,double r1,Vector2 &c2,double r2) {
    Vector2 diff=c1-c2;
    double distance=diff.length();
    if (distance<r1+r2 && distance>0) {
        diff=diff/distance; //normalize the difference vectors
        Vector2 COM=(c1*r1*r1+c2*r2*r2)/(r1*r1+r2*r2); //center of mass of the vectors.
        //Solve the equations in center of mass frame
        //  c1.diff-c2.diff=r1+r2 (they are exactly distance r1+r2 away)
        //  c1*r1^2=-c2.diff*r2^2
        double c1dotdiff=(r1+r2)*r2*r2/(r1*r1+r2*r2);
        double c2dotdiff=-c1dotdiff*r1*r1/(r2*r2);
        c1=c1dotdiff*diff+COM;
        c2=c2dotdiff*diff+COM;
        //and that's math for you.
    }
}