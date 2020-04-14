#pragma once

#include <vector>
#include <cmath>
#include <math.h>
#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
        struct Sphere{
            glm::vec3 centre;
            double r;
            ofColor col;
            int specular;
        };

        struct Light{
            char type;
            double intensity;
            glm::vec3 position;
            glm::vec3 direction;
        };

        struct Intersection{
            Sphere sphere;
            double closest_t;
        };

        std::vector<Sphere> spheres;
        void makeSpheres();
        std::vector<Light> lights;
        void makeLights();

		void setup();
		void update();
		void draw();

        ~ofApp();

        void makeTexture(glm::vec2, ofColor col);

        ofColor bg;

        glm::vec3 canvasToViewport(glm::vec2 p);
        ofColor traceRay(glm::vec3 o, glm::vec3 D, double t_min, double t_max);
        double* intersectRaySphere(glm::vec3 o, glm::vec3 Dir, const Sphere& sphere);
        double computeLighting(glm::vec3 P, glm::vec3 N, glm::vec3 V, int s);
        Intersection closestIntersection(glm::vec3 o, glm::vec3 Dir, double t_min, double t_max);

        int Cw, Ch;
        int Vw, Vh;
        int d;
        int canvasPitch;

        int ticks;

        unsigned char* data;
        ofTexture tex;

        int nTotalBytes;

        glm::vec3 o;
		
};
