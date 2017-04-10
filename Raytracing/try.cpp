//
//  try.cpp
//  Raytracing
//
//  Created by 张嘉恒 on 17/4/6.
//  Copyright © 2017年 张嘉恒. All rights reserved.
//

#include <algorithm>
#include <iostream>
#include <cmath>
#include <cstdio>
#include "lib/ObjLoad.h"
#include "lib/ObjInterleavedData.h"
#include "lib/ObjSave.h"
#include "lib/ObjData.h"
#include <cstdlib>
#include <cstring>
#include <vector>
#include <cstdio>
#include <random>
#include <atomic>
#include <thread>
#include <memory>
#include "concurrentqueue/concurrentqueue.h"


using namespace std;

const int num_workers = 4;

const float EPS = 1e-3;

const int Maxdepth = 2;

const float pi = 3.141592653;

const int samples = 50;

//std::atomic<int> cnt_pixels;


template<typename T>

class Vec3
{
public:
    T x, y, z;
    Vec3() : x(T(0)), y(T(0)), z(T(0)) {}
    Vec3(T xx) : x(xx), y(xx), z(xx) {}
    Vec3(T xx, T yy, T zz) : x(xx), y(yy), z(zz) {}
    
    

    Vec3& normalize()
    {
        T nor2 = length2();
        if (nor2 > 0) {
            T invNor = 1 / sqrt(nor2);
            x *= invNor, y *= invNor, z *= invNor;
        }
        return *this;
    }
    Vec3<T> operator * (const T &f) const { return Vec3<T>(x * f, y * f, z * f); }
    Vec3<T> operator * (const Vec3<T> &v) const { return Vec3<T>(x * v.x, y * v.y, z * v.z); }
    T dot(const Vec3<T> &v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3<T> crossProduct(const Vec3<T> &v) const {
        return Vec3<T>(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        
    }
    Vec3<T> operator - (const Vec3<T> &v) const { return Vec3<T>(x - v.x, y - v.y, z - v.z); }
    Vec3<T> operator + (const Vec3<T> &v) const { return Vec3<T>(x + v.x, y + v.y, z + v.z); }
    Vec3<T> operator / (const Vec3<T>& b)const{return Vec3<T>(x / b.x, y / b.y, z / b.z);}
    Vec3<T>& operator += (const Vec3<T> &v) { x += v.x, y += v.y, z += v.z; return *this; }
    Vec3<T>& operator *= (const Vec3<T> &v) { x *= v.x, y *= v.y, z *= v.z; return *this; }
    Vec3<T> operator - () const { return Vec3<T>(-x, -y, -z); }
    T length2() const { return x * x + y * y + z * z; }
    T length() const { return sqrt(length2()); }
    friend Vec3<T> operator * (const float &r, const Vec3<T> &v)
    { return Vec3<T>(v.x * r, v.y * r, v.z * r); }
    bool operator==(const Vec3<T>& b) const
    {
        Vec3<T> tmp = *this - b;
        return fabs(tmp.x) < EPS && fabs(tmp.y) < EPS && fabs(tmp.z) < EPS;
    }
    friend std::ostream & operator << (std::ostream &os, const Vec3<T> &v)
    {
        os << "[" << v.x << " " << v.y << " " << v.z << "]";
        return os;
    }
};

typedef Vec3<float> Vec3f;
typedef Vec3f Color;

struct Material {
    Color color;
    float k_reflect;
    //float k_diffuse;
    float k_diffuse_reflect;
    float k_specular;
    float k_refract;
    float k_refract_index;
    float k_ambient;
};

class Ray
{
public:
    Vec3f origin, direction;
    Ray(Vec3f origin, Vec3f direction){
        this->origin = origin;
        this->direction = direction.normalize();
    };
    Vec3f getpoint(float t){
        return this->origin + this->direction * t;
    };
};

class  Plane
{
public:
    float d;
    bool IsTransparent;
    Material material;
    vector<Vec3f> vertex;
    Vec3f normal;
    
    Plane(const Vec3f &a, const Vec3f &b, const Vec3f &c, const Vec3f &color, const Vec3f &delta){
        vertex.push_back(a + delta);
        vertex.push_back(b + delta);
        vertex.push_back(c + delta);
        if(vertex.size() > 2){
            
            Vec3f a = vertex[1] - vertex[0];
            Vec3f b = vertex[vertex.size() - 1] - vertex[0];
            normal = a.crossProduct(b);
            normal.normalize();
            d = -normal.dot(vertex[0]);
        }
        vertex.push_back(vertex[0]);
        //cout << "vertex.size() = " << vertex.size() << endl;
        material.color = color;
        material.k_diffuse_reflect = 1;
        //material.k_diffuse = 0.1;
        material.k_reflect = 1;
        material.k_specular = 0;
        material.k_refract = 1;
        material.k_refract_index = 1.2;
        material.k_ambient = 1;
        
    }
    
    bool in(const Vec3f& pt)
    {
        Vec3f a,b;
        //cout << vertex.size() << endl;
        for (int i = 1; i < vertex.size(); i++)
        {
            a = (vertex[i] - vertex[i - 1]).normalize();
            b = (pt - vertex[i - 1]).normalize();
            Vec3f tmp = a.crossProduct(b);
            //if(tmp == Vec3f(0)) cout << "i = " << i << " " << tmp << endl;
            //cout << "a = " << a.dot(normal) << " " << "b = " << b.dot(normal) << endl;
            //cout << "i = " << i << " " << a.crossProduct(b) << "normal = " << normal << endl;
            //cout << "i = " << i << " " << "(normal - tmp).length() = " << (normal - tmp).length() << endl;
            if (tmp.length() > 1e-4 && (normal - tmp.normalize()).length() > 1){
                //cout << tmp << (normal - tmp).length() << endl;
                return false;
            }
        }
        return true;
    }
    ~Plane(){}
};

class Light{
public:
    Vec3f pos;
    Color color;
    Light(const Vec3f& p, const Color I): pos(p), color(I){}
};

class Raytracing
{
public:
    std::vector<Plane> planes;
    Light light;
    Vec3f eye;
    Vec3f lightlb, lightrt;
    int lwidth, lheight;
    Color Ia;
    int width, height;
    //Color a[100000];
    vector<Color> buffer;
    Vec3f lb, rt;
    
    default_random_engine generatorr;
    uniform_real_distribution<double> distr;
    
    
    float erand()
    {
        return distr(generatorr);
    }
    
    Raytracing(const vector<Plane>& faces, const Light& orilight, const Color &color, const Vec3f &eyes, const Vec3f &lb0, const Vec3f &rt0, const int &width0, const int &height0, const Vec3f &lightlb0, const Vec3f &lightrt0, const int &lwight0, const int &lheight0):planes(faces), light(orilight), Ia(color), eye(eyes), lb(lb0), rt(rt0), width(width0), height(height0), lightlb(lightlb0), lightrt(lightrt0), lwidth(lwight0), lheight(lheight0){distr = uniform_real_distribution<double>(0, 1);};
    
     Raytracing(const vector<Plane>& faces, const Light& orilight, const Color &color, const Vec3f &eyes, const Vec3f &lb0, const Vec3f &rt0, const int &width0, const int &height0):planes(faces), light(orilight), Ia(color), eye(eyes), lb(lb0), rt(rt0), width(width0), height(height0){distr = uniform_real_distribution<double>(0, 1);};

    
    Vec3f crossRayPlane(const Ray& ray,const Plane& plane)
    {
        Vec3f p = ray.origin, n = plane.normal, d = ray.direction;
        return p - d * ((plane.d + n.dot(p)) / n.dot(d));
    }
    
    int find(const Ray& ray)
    {
        int nearest = -1;
        float min = 1e200;
        for (int i = 0; i < planes.size(); i++)
        {
            Vec3f X = crossRayPlane(ray, planes[i]);
            
            if (planes[i].in(X))
            {
                //cout << "i = " << i << endl;
                if(fabs((X - ray.origin).x) > 1e-2 || fabs((X - ray.origin).y) > 1e-2 || fabs((X - ray.origin).z) > 1e-2){
                    float t;
                    if(fabs(ray.direction.z) > 1e-4){
                        t = (X - ray.origin).z / ray.direction.z;}
                    else {
                        if(fabs(ray.direction.y) > 1e-4)
                            t = (X - ray.origin).y / ray.direction.y;
                        else{
                            if(fabs(ray.direction.x) > 1e-4)
                                t = (X - ray.origin).x / ray.direction.x;
                        }
                    }
                    //cout << t << " " << min << endl;
                    if(t > 0 && min > t + 1e-4)
                    {
                        min = t;
                        nearest = i;
                    }
                }
                //cout << nearest << endl;
            }
            
        }
        
        return nearest;
    }
    
    
    Vec3f reflect(const Vec3f &I, const Vec3f &N)
    {
        //cout << "verify" << " " << (I + I - 2 * (I.dot(N) * N)).dot(N) << endl;
        return I - 2 * (I.dot(N) * N);
    }
    
    Vec3f refract(const Vec3f &I, const Vec3f &N, const float &ior)
    {
        float cosi = I.dot(N);
        float etai = 1, etat = ior;
        Vec3f n = N;
        if (cosi < 0) { cosi = -cosi; } else { std::swap(etai, etat); n= -N; }
        float eta = etai / etat;
        float k = 1 - eta * eta * (1 - cosi * cosi);
        return k < 0 ? 0 : eta * I + (eta * cosi - sqrtf(k)) * n;
    }

    /*Color local(int k, const Vec3f& current_pos,const Ray& ray)
    {
        const Plane& p = planes[k];
        Color color = p.material.k_ambient * Ia;
        Vec3f normal = p.normal;
        if (find(Ray(light.pos, current_pos - light.pos)) == k)
        {
            Vec3f tmpkd(0), tmpks(0);
            tmpkd = light.color * p.material.color * p.material.k_diffuse_reflect * (light.pos - current_pos).normalize().dot(normal);
            if (tmpkd.x > 1e-3 || tmpkd.y > 1e-3 || tmpkd.z > 1e-3) color += tmpkd;
            
            if (tmpkd.x < -1e-3 || tmpkd.y < -1e-3 || tmpkd.z < -1e-3) color = color - tmpkd;
            
            
        }
        return color;
    }*/
    
   
    Color tracing(const Ray &ray, int depth){
        
        Color color(0);
        
        int nearest = find(ray);
        
        if(nearest == -1 && ray.direction.z < -1e-3){
            //cout << "come in " << light.color * (Vec3f(0, 0, -1).dot(ray.direction)) << endl;
            return light.color * (Vec3f(0, 0, -1).dot(ray.direction));
        }
        
        if(nearest != -1){
            //cout << "nearest = " << nearest << endl;
            Plane &p = planes[nearest];
            
            Vec3f cross = crossRayPlane(ray, p);
            //cout << p.normal.dot(ray.direction) << endl;
            
            if(depth == 0 && p.material.k_diffuse_reflect > 0){
                Color diffcolor;
                for(int i = 0; i < samples; i++){
                    double r1 = 2 * pi * erand(), r2 = erand(), r2s = sqrt(r2);
                    
                    Vec3f w = p.normal;
                    
                    if(p.normal.dot(ray.direction) > 1e-2){
                        //cout << "ddddsss " << endl;
                        w = (-1) * w;
                    
                    }
                    
                    Vec3f u = (fabs(w.x) > .1 ? Vec3f(0, 1, 0) : Vec3f(1, 0, 0)).crossProduct(w).normalize(), v = w.crossProduct(u);
                    
                    Vec3f d = (u * cos(r1) * r2s + v * sin(r1) * r2s + w * sqrt(1 - r2)).normalize();
                    
                    //cout << "weather it is right " << d.dot(p.normal) << endl;
                    
                    //cout << "d.z = " << d.z << endl;
                    
                    Ray diffuse_ray(cross, d);
                    
                    diffcolor += p.material.k_diffuse_reflect * tracing(diffuse_ray, depth + 1);
                }
                color += (diffcolor / Vec3f(samples, samples, samples));
                
            }
            
            if(depth < Maxdepth){
                //Ray ReflectRay(cross, reflect(ray.direction, p.normal).normalize());
                //color += p.material.k_reflect * tracing(ReflectRay, depth + 1);
                //if(p.material.k_refract > 0){
                    //Ray RefractRay(cross, refract(ray.direction, p.normal, p.material.k_refract_index));
                    //color += p.material.k_refract * tracing(RefractRay, depth + 1);
                //}
            }
            color = color * p.material.color;
            
            //if(color.y < 10) cout << "sss " << endl;
        }
        if (color.x > 255) color.x = 255;
        if (color.y > 255) color.y = 255;
        if (color.z > 255) color.z = 255;
        return color;
    };
    
    void show(){
        moodycamel::ConcurrentQueue<std::pair<int, int> > q;
        int t = 0;
        auto func = [&] {
            Vec3f screenSize(width, height, 1);
            for (std::pair<int, int> item;;) {
                q.try_dequeue(item);
                int i = item.first,
                    j = item.second;
                fprintf(stderr, "%d %d\n", i, j);
                if (i == -1) return;
                Vec3f s = Vec3f(i, j, 0) * (rt - lb) / screenSize + lb;
                s.z = lb.z;
                Ray ray(eye, (s - eye).normalize());
                
                //a[t++] = tracing(ray, 0);
                
                buffer[i * width + j] = tracing(ray, 0);
                //fprintf(stderr, "%f %f %f", buffer.back());
                //++cnt_pixels;
            }
        };
        
        //cnt_pixels = 0;
        std::vector<std::pair<int, int> > xys;
        
        for (int i = 0; i < height; i++)
            for (int j = 0; j < width; j++)
                xys.emplace_back(i, j);
        
        std::random_shuffle(xys.begin(), xys.end());
        
        for (const auto &xy: xys)
            q.enqueue(xy);
        
        std::vector<std::thread> workers;
        
        for (int i = 0; i < num_workers; ++i)
            workers.emplace_back(func);
        
        for (int i = 0; i < num_workers; ++i)
            q.enqueue(std::make_pair(-1, -1));
        
        
        
        for (auto &worker: workers) {
            worker.join();
        }
    };
    
    void print(){
        //cout << "haha" << endl;
        typedef unsigned char uint8_t;
        buffer.resize(height * width);
        show();
        FILE *f = fopen("/Users/zhangjiaheng/result.ppm", "w");
        fprintf(f, "P3\n");
        fprintf(f, "%d %d\n", width, height);
        fprintf(f, "255\n");
        for(int j = height - 1; j >= 0; j--){
            for(int i = 0; i < width; i++){
                //if(i == 100 && j == 80)
                //fprintf(f, "%d\t%d\n", i, j);
                fprintf(f, "%u\t%u\t%u\n", static_cast<uint8_t>(buffer[i * height + j].x), static_cast<uint8_t>(buffer[i * height + j].y), static_cast<uint8_t>(buffer[i * height + j].z));
            }
        }
        fprintf(f, "\n");
    };
    
    ~Raytracing(){};
    
    /* data */
};
