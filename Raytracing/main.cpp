//
//  main.cpp
//  Raytracing
//
//  Created by 张嘉恒 on 17/4/5.
//  Copyright © 2017年 张嘉恒. All rights reserved.
//

#include <iostream>
#include "scene.h"
#include "try.cpp"
#include <cstring>
#include "lib/ObjLoad.h"
#include "lib/ObjInterleavedData.h"
#include "lib/ObjSave.h"
#include "lib/ObjData.h"

const double MAX = 1e5;

int main(int argc, const char * argv[]) {
    // insert code here...
    scene ourscene("/Users/zhangjiaheng/Desktop", "teapot.obj");
    std::vector<kick::ObjFace> faces = ourscene.alldatas.faces;
    vector<Plane> planes;
    for(int i = 0; i < ourscene.data.indices.size(); ++i)
        for(int j = 0; j < ourscene.data.indices[i].vertexIndices.size(); j += 3)
        {
            int a_id, b_id, c_id;
            a_id = ourscene.data.indices[i].vertexIndices[j];
            b_id = ourscene.data.indices[i].vertexIndices[j + 1];
            c_id = ourscene.data.indices[i].vertexIndices[j + 2];
            auto a = ourscene.data.vertex(a_id), b = ourscene.data.vertex(b_id), c = ourscene.data.vertex(c_id);
            Vec3f aa(a[0], a[1], a[2]), bb(b[0], b[1], b[2]), cc(c[0], c[1],c[2]);
            planes.push_back(Plane(aa, bb, cc, Color(0, 1, 0), Vec3f(0, 0, 8)));
        }
    /*int delta = 5;
    Color wall(3, 3, 3);
    planes.push_back(Plane(Vec3f(-delta, -MAX, -MAX), Vec3f(-delta, MAX, -MAX), Vec3f(-delta, -MAX, MAX), wall,Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(-4, -MAX, -MAX), Vec3f(-4, MAX, -MAX), Vec3f(-4, -MAX, MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(-delta, MAX, MAX), Vec3f(-delta, -MAX, MAX), Vec3f(-delta, MAX, -MAX), wall, Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(-4, MAX, MAX), Vec3f(-4, -MAX, MAX), Vec3f(-4, MAX, -MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(delta, -MAX, -MAX), Vec3f(delta, -MAX, MAX), Vec3f(delta, MAX, -MAX), wall,Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(4, -MAX, -MAX), Vec3f(4, -MAX, MAX), Vec3f(4, MAX, -MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(delta, MAX, MAX), Vec3f(delta, MAX, -MAX), Vec3f(delta, -MAX, MAX), wall,Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(4, MAX, MAX), Vec3f(4, MAX, -MAX), Vec3f(4, -MAX, MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    
    planes.push_back(Plane(Vec3f(-MAX, -delta, -MAX), Vec3f(-MAX, -delta, MAX), Vec3f(MAX, -delta, -MAX), wall,Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(-MAX, -15, -MAX), Vec3f(-MAX, -15, MAX), Vec3f(MAX, -15, -MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(MAX, -delta, MAX), Vec3f(MAX, -delta, -MAX), Vec3f(-MAX, -delta, MAX), wall,Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(MAX, -15, MAX), Vec3f(MAX, -15, -MAX), Vec3f(-MAX, -15, MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(-MAX, delta, -MAX), Vec3f(MAX, delta, -MAX), Vec3f(-MAX, delta, MAX), wall,Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(-MAX, 15, -MAX), Vec3f(MAX, 15, -MAX), Vec3f(-MAX, 15, MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(MAX, delta, MAX), Vec3f(-MAX, delta, MAX), Vec3f(MAX, delta, -MAX), wall,Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(MAX, 15, MAX), Vec3f(-MAX, 15, MAX), Vec3f(MAX, 15, -MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
     
     planes.push_back(Plane(Vec3f(-MAX, -MAX, 20), Vec3f(-MAX, MAX, 20), Vec3f(MAX, -MAX, 20), Color(1,0,0),Vec3f(0, 0, 0)));
     cout << Plane(Vec3f(-MAX, -MAX, 20), Vec3f(-MAX, MAX, 20), Vec3f(MAX, -MAX, 20), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
     planes.push_back(Plane(Vec3f(MAX, MAX, 20), Vec3f(MAX, -MAX, 20), Vec3f(-MAX, MAX, 20), Color(1,0,0),Vec3f(0, 0, 0)));
     cout << Plane(Vec3f(MAX, MAX, 20), Vec3f(MAX, -MAX, 20), Vec3f(-MAX, MAX, 20), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
     
    */
    
    cout << planes.size() << endl;
    Color c(255,255,255);
    Vec3f ver(0, 0, -1);
    Light light(ver, c);
    Vec3f Ia(0, 0, 0);
    Vec3f eye(0, 0, 0);
    Vec3f lb(-1, -1, 1);
    Vec3f rt(1, 1, 1);
    Raytracing raytracing(planes, light, Ia, eye, lb, rt, 200, 200);
    
    //Ray trylight(Vec3f(0, 0, 0), Vec3f(0, 0.16, 1));
    
    //cout << raytracing.tracing(trylight, 0) << endl;
    raytracing.print();
    //std::cout << "Hello, World!\n";
    return 0;
}
