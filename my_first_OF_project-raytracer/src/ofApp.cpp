#include "ofApp.h"

using namespace glm;
using namespace std;

//--------------------------------------------------------------
void ofApp::setup(){
    ticks = 0;
    Cw = ofGetWidth();
    Ch = ofGetHeight();
    Vw = Vh = d = 1;

    data = new unsigned char[Cw * Ch* 3];
    tex.allocate(Cw, Ch, GL_RGB);

    nTotalBytes = Cw * Ch * 3;

    o = vec3(0.0, 0.0, 0.0);

    bg = ofColor(240, 240, 240);

    makeSpheres();
    makeLights();
}

glm::vec3 ofApp::canvasToViewport(glm::vec2 p){
    return vec3(p.x * Vw/Cw, p.y * Vh/Ch, d);
}

void ofApp::makeSpheres(){
    Sphere red;
    red.centre = vec3(0.0, -1.0, 3.0);
    red.r = 1.0;
    red.col = ofColor(255, 0, 0);
    red.specular = 500;

    spheres.push_back(red);

    Sphere blue;
    blue.centre = vec3(2.0, 0.0, 4.0);
    blue.r = 1.0;
    blue.col = ofColor(0, 0, 255);
    blue.specular = 100;

    spheres.push_back(blue);

    Sphere green;
    green.centre = vec3(-2.0, 0.0, 4.0);
    green.r = 1.0;
    green.col = ofColor(0, 255, 0);
    green.specular = 1000;

    spheres.push_back(green);

    Sphere yellow;
    yellow.centre = vec3(0, -5001, 0);
    yellow.r = 5000;
    yellow.col = ofColor(255, 255, 0);
    yellow.specular = 300;

    spheres.push_back(yellow);
}

void ofApp::makeLights(){
    Light ambient;
    ambient.type = 'a';
    ambient.intensity = 0.2;

    Light point;
    point.type = 'p';
    point.intensity = 0.6;
    point.position = vec3(2.0, 1.0, 0.0);

    Light directional;
    directional.type = 'd';
    directional.intensity = 0.2;
    directional.direction = vec3(1.0, 4.0, 4.0);

    lights.push_back(ambient);
    lights.push_back(point);
    lights.push_back(directional);
}


double* ofApp::intersectRaySphere(glm::vec3 o, glm::vec3 Dir, const Sphere& sphere){
    vec3 C = sphere.centre;
    double r = sphere.r;
    vec3 oc = o - C;

    double k1 = dot(Dir, Dir);
    double k2 = 2*dot(oc, Dir);
    double k3 = dot(oc, oc) - r*r;

    double discriminant = k2*k2 - 4*k1*k3;

    static double ret[2];

    if(discriminant < 0) {
        ret[0] = INFINITY;
        ret[1] = INFINITY;
        return ret;
    }

    double t1 = (-k2 + sqrt(discriminant)) / (2*k1);
    double t2 = (-k2 - sqrt(discriminant)) / (2*k1);

    ret[0] = t1;
    ret[1] = t2;

    return ret;
}

ofApp::Intersection ofApp::closestIntersection(glm::vec3 o, glm::vec3 Dir, double t_min, double t_max){
    double closest_t = INFINITY;
    Sphere closest_sphere;
    closest_sphere.r = 0.0; // Setting this to 0.0 to make a NULL sphere

    for(vector<Sphere>::const_iterator it=spheres.begin(); it!=spheres.end(); ++it){
        double* t = intersectRaySphere(o, Dir, *it); 
        if((t[0] > t_min && t[0] > t_max) && t[0] < closest_t){
            closest_t = t[0];
            closest_sphere = *it;
        }
        if((t[1] > t_min && t[1] < t_max) && t[1] < closest_t){
            closest_t = t[1];
            closest_sphere = *it;
        }
    }

    Intersection intersect;
    intersect.sphere = closest_sphere;
    intersect.closest_t = closest_t;

    return intersect;
}

ofColor ofApp::traceRay(vec3 o, vec3 Dir, double t_min, double t_max){
    Intersection intersect = closestIntersection(o, Dir, t_min, t_max);

    if(!intersect.sphere.r) return bg;
    else {
        vec3 P = o + intersect.closest_t*Dir; // Compute intersection
        vec3 N = P - intersect.sphere.centre; // Sphere normal at intersection
        N = N / length(N);
        return intersect.sphere.col*computeLighting(P, N, -Dir, intersect.sphere.specular);
    }
}

double ofApp::computeLighting(glm::vec3 P, glm::vec3 N, glm::vec3 V, int s){
    double i = 0.0;
    double t_max;
    vec3 L;
    for(vector<Light>::const_iterator l=lights.begin(); l!=lights.end(); ++l){
        if(l->type == 'a') i += l->intensity;
        else {
            if(l->type == 'p') {
                L = l->position - P;
                t_max = 1;
            } else {
                L = l->direction;
                t_max = INFINITY;
            }

            // Shadow Check
            Intersection shadow_intersect = closestIntersection(P, L, 0.001, t_max);

            if(shadow_intersect.sphere.r) continue;

            // Diffuse
            double n_dot_l = dot(N, L);
            if(n_dot_l > 0) i += l->intensity * n_dot_l/(length(N) * length(L));

            // Specular
            if(s != -1){
                glm::vec3 R = 2*N*dot(N, L) - L;
                double r_dot_v = dot(R, V);
                if(r_dot_v > 0) i += l->intensity*pow(r_dot_v/(length(R) * length(V)), s);
            }
        }
    }
    return i;
}

void ofApp::makeTexture(glm::vec2 p, ofColor col){
    int x = Cw/2 + p.x;
    int y = Ch/2 - p.y - 1;

    if(x<0 || x>=Cw || y<0 || y>Ch) return;

    int offset = 3*x + Cw*3 * y;

    data[offset++] = col.r;
    data[offset++] = col.g;
    data[offset++] = col.b;

}

//--------------------------------------------------------------
void ofApp::update(){

    int count = 0; 
    for(vector<Sphere>::iterator it=spheres.begin(); it!=spheres.end(); ++it){
        if(count == 0) it->centre.y += sin(ticks*0.1) * 0.05;
        else if(count == 1) it->centre.y += cos(ticks*0.1) * 0.08;
        else if(count == 2) it->centre.z += sin(ticks*0.2) * 0.09;
        else it->centre = it->centre;

        count++;
    }



    for(int x=-Cw/2; x<Cw/2; x++){
        for(int y=-Ch/2; y<Ch/2; y++){
            vec3 Dir = canvasToViewport(vec2(x, y));
            ofColor pixCol = traceRay(o, Dir, 1, INFINITY);
            makeTexture(vec2(x,y), pixCol);
        }
    }

    tex.loadData(data, Cw, Ch, GL_RGB);

    ticks++;

}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(100, 100, 100);
    tex.draw(0,0);

}

ofApp::~ofApp(){
    delete[] data;
}
