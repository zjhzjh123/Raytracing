//
//  scene.h
//  Raytracing
//
//  Created by 张嘉恒 on 17/4/5.
//  Copyright © 2017年 张嘉恒. All rights reserved.
//

#ifndef scene_h
#define scene_h

#include<cstring>
#include "lib/ObjLoad.h"
#include "lib/ObjInterleavedData.h"
#include "lib/ObjSave.h"
#include "lib/ObjData.h"



class scene{
public:
    bool initial;
    kick::ObjInterleavedData data;
    kick::ObjData alldatas;
    scene(){initial = false;}
    scene(const char *path, const char *filename){
        alldatas = kick::loadObjData(std::string(path), std::string(filename));
        data.init(alldatas);
        initial = true;
    }
};



#endif /* scene_h */
