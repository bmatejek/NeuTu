#ifndef SKELETON_UTILITY_H
#define SKELETON_UTILITY_H

#include <string>

typedef unsigned int uint;
typedef unsigned long ulong;


class LeafStruct{
    double nodex;
    double nodey;
    double nodez;

    uint endpt_id;
    uint segment_id;
    

public:
    LeafStruct(){
        nodez = 0;
        nodey = 0;
        nodex = 0;
        
        endpt_id=0;
        segment_id=0;
    }
    LeafStruct(double pz, double py, double px){
        nodez = pz;
        nodey = py;
        nodex = px;
        
        endpt_id=0;
        segment_id=0;
    }
    
    void setSegmentId(uint ppid){
        segment_id=ppid;
    }
    
    void setEndptId(uint ppid){
        endpt_id=ppid;
    }
    void getEndPoints(double &pz, double &py, double &px){
        pz = nodez;
        py = nodey;
        px = nodex;
    }
    void setEndPoints(double pz, double py, double px){
        nodez = pz;
        nodey = py;
        nodex = px;
    }
    
    double getz(){return nodez;}
    double gety(){return nodey;}
    double getx(){return nodex;}
};

class SegmentStruct{
    unsigned int id;
    unsigned long sz;

public:
    SegmentStruct(uint ppid, uint psz){
        id = ppid;
        sz = psz;
    }
};





#endif // SKELETON_UTILITY_H

