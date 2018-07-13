#ifndef SEGMENTATION_INTERFACE_H
#define SEGMENTATION_INTERFACE_H

#include "skeleton_utility.h"
#include <vector>
#include <map>
//#include <DataStructures/Stack.h>


class SegmentationInterface{
    uint* segmentation_data;
    float* prediction_data;
    //NeuroProof::StackPredict* stackp;
    std::vector<uint> nearby_regions;
    std::vector<uint> all_region_ids;
    std::map<uint,ulong> all_regions;

    unsigned int depth;
    unsigned int height;
    unsigned int width;
public:
    SegmentationInterface(uint* pseg, float* pp, std::string, int d, int h, int w, int nchannels);
    void find_search_region(LeafStruct& endpt, LeafStruct &minpt, LeafStruct &maxpt);
    std::vector<uint>& findAdjacentRegions(LeafStruct& enpt);

    std::vector<uint>& get_all_region_ids() ;
    std::map<uint,ulong>& get_all_regions() ;

};


#endif // SEGMENTATION_INTERFACE_H

