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
    scene ourscene("/Users/zhangjiaheng/Desktop", "sphere.obj");
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
            planes.push_back(Plane(aa, bb, cc, Color(1, 1, 1), Vec3f(0, 0, 4), 0.6, 0.6, 1.3));
        }
    
    int delta = 2;
    int low = -2;
    int far = 7;
    Color wall(1, 1, 1);
    
   planes.push_back(Plane(Vec3f(-delta, -MAX, -MAX), Vec3f(-delta, MAX, -MAX), Vec3f(-delta, -MAX, MAX), Color(1, 0, 0),Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(-4, -MAX, -MAX), Vec3f(-4, MAX, -MAX), Vec3f(-4, -MAX, MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(-delta, MAX, MAX), Vec3f(-delta, -MAX, MAX), Vec3f(-delta, MAX, -MAX), Color(1, 0, 0), Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(-4, MAX, MAX), Vec3f(-4, -MAX, MAX), Vec3f(-4, MAX, -MAX), Color(1,1,1),Vec3f(1, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(delta, -MAX, -MAX), Vec3f(delta, -MAX, MAX), Vec3f(delta, MAX, -MAX), Color(0, 1, 0),Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(4, -MAX, -MAX), Vec3f(4, -MAX, MAX), Vec3f(4, MAX, -MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(delta, MAX, MAX), Vec3f(delta, MAX, -MAX), Vec3f(delta, -MAX, MAX), Color(0, 1, 0), Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(4, MAX, MAX), Vec3f(4, MAX, -MAX), Vec3f(4, -MAX, MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    
    //planes.push_back(Plane(Vec3f(-MAX, -delta, -MAX), Vec3f(-MAX, -delta, MAX), Vec3f(MAX, -delta, -MAX), wall,Vec3f(0, 0, 0)));
    //cout << Plane(Vec3f(-MAX, -15, -MAX), Vec3f(-MAX, -15, MAX), Vec3f(MAX, -15, -MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    //planes.push_back(Plane(Vec3f(MAX, -delta, MAX), Vec3f(MAX, -delta, -MAX), Vec3f(-MAX, -delta, MAX), wall,Vec3f(0, 0, 0)));
    //cout << Plane(Vec3f(MAX, -15, MAX), Vec3f(MAX, -15, -MAX), Vec3f(-MAX, -15, MAX), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(-MAX, low, -MAX), Vec3f(MAX, low, -MAX), Vec3f(-MAX, low, MAX), Color(1, 1, 1),Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(-MAX, 15, -MAX), Vec3f(MAX, 15, -MAX), Vec3f(-MAX, 15, MAX), Color(0,0,0),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(MAX, low, MAX), Vec3f(-MAX, low, MAX), Vec3f(MAX, low, -MAX), Color(1, 1, 1),Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(MAX, 15, MAX), Vec3f(-MAX, 15, MAX), Vec3f(MAX, 15, -MAX), Color(0,0,0),Vec3f(0, 0, 0)).normal << endl;
     
    planes.push_back(Plane(Vec3f(-MAX, -MAX, far), Vec3f(-MAX, MAX, far), Vec3f(MAX, -MAX, far), Color(0,0,1),Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(-MAX, -MAX, 20), Vec3f(-MAX, MAX, 20), Vec3f(MAX, -MAX, 20), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    planes.push_back(Plane(Vec3f(MAX, MAX, far), Vec3f(MAX, -MAX, far), Vec3f(-MAX, MAX, far), Color(0,0,1),Vec3f(0, 0, 0)));
    cout << Plane(Vec3f(MAX, MAX, 20), Vec3f(MAX, -MAX, 20), Vec3f(-MAX, MAX, 20), Color(1,1,1),Vec3f(0, 0, 0)).normal << endl;
    
    cout << planes.size() << endl;
    Color c(255 * 2,255 * 2,255 * 2);
    //Vec3f ver(0, 0, -1);
    
//    for(int i = 0; i < 2; i++){
  //      for(int j = 0; j < 2; j++){
    //        Vec3f ver(i * 0.2, j * 0.2, -1);
      //      light.push_back(Light(ver, c));
       // }
    //}
    
    //light.push_back(Light(Vec3f(-0.2, 0, -1), c));
    //light.push_back(Light(Vec3f(0, 0, -1), c));
    //light.push_back(Light(Vec3f(0.2, 0, -1), c));
    //light.push_back(Light(Vec3f(0, 0, -1), c));
    
    //cout << light.size() << endl;
    
    //Light light(ver, c);
    
    vector<Light> light;
    
    light.push_back(Light(Vec3f(0, 4, 4), c));
  
    Vec3f Ia(0, 0, 0);
    Vec3f eye(0, 0, -4);
    Vec3f lb(-2, -2, 1);
    Vec3f rt(2, 2, 1);
    Raytracing raytracing(planes, light, Ia, eye, lb, rt, 200, 200);
    
    cout << "size = " << raytracing.light.size() << endl;
    raytracing.samples_lights(10,  0.75);
    
    cout << "size = " << raytracing.light.size() << endl;
    
    
    //Ray trylight(Vec3f(0, 0, 0), Vec3f(0, 0.16, 1));
    
    //cout << raytracing.tracing(trylight, 0) << endl;
    raytracing.print();
    //std::cout << "Hello, World!\n";
    return 0;
}
