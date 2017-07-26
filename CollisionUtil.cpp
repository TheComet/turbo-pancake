#include "CollisionUtil.h"
#include "Globals.h"


Line::Line() : p1(),p2() { }
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

bool doesCirclePointCollide(Vector2 &circlepos,double r,Vector2 point) {
    return (circlepos-point).length()<r;
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