#include <set>
#include <cstdio>
#include "segmentation_interface.h"



template <class T>
void padZero(T* data, const size_t* dims, int pad_length, T** ppadded_data){

     // implemented only for 3D arrays

     unsigned long int newsize = (dims[0]+2*pad_length)*(dims[1]+2*pad_length)*(dims[2]+2*pad_length);
     *ppadded_data = new T[newsize];
     T* padded_data = *ppadded_data;

     memset((void*) padded_data, 0, sizeof(T)*newsize);

     unsigned int width, plane_size, width0, plane_size0, i0,j0,k0, i,j,k;

     for (i=pad_length, i0=0; i<= dims[0] ; i++, i0++)
         for (j=pad_length, j0=0; j<= dims[1]; j++, j0++)
             for(k=pad_length, k0=0; k<= dims[2]; k++, k0++){
                 plane_size = (dims[1]+2*pad_length)*(dims[2]+2*pad_length);
                 width = dims[2]+2*pad_length;

                 plane_size0 = dims[1]*dims[2];
                 width0 = dims[2];

                 padded_data[i*plane_size+ j*width + k] = data[i0*plane_size0 + j0*width0 + k0];
             }


}

bool endswith(std::string filename, std::string extn){
    unsigned found = filename.find_last_of(".");
    std::string fextn = filename.substr(found);
    if (fextn.compare(extn) == 0 )
        return true;
    else return false;
}

SegmentationInterface::SegmentationInterface(uint* pseg, float* pp,
                                             std::string classifier_filename,
                                             int d, int h, int w, int nchannels)
{
    segmentation_data = pseg;
    prediction_data = pp;
    depth = d;
    height = h;
    width =w;
    size_t dims[3];
    dims[0] = depth; dims[1] = height; dims[2] = width;

    uint pad_len=1;
    uint *zp_watershed_data=NULL;
    padZero(segmentation_data, dims,pad_len,&zp_watershed_data);
    stackp = new NeuroProof::StackPredict(zp_watershed_data, depth+2*pad_len, height+2*pad_len, width+2*pad_len, pad_len);
    stackp->set_feature_mgr(new FeatureMgr());


    double* prediction_single_ch = new double[depth*height*width];
    double* prediction_ch0 = new double[depth*height*width];


    size_t i,j,k,width_size, plane_size, element_size= nchannels;
    size_t position, count;
    for (int ch=0; ch < element_size; ch++){
        count = 0;
        for(i=0; i<depth; i++){
            plane_size = height*width*element_size;
            for(j=0; j<height; j++){
                width_size = width*element_size;
                for(k=0; k<width; k++){
                    position = i*plane_size + j*width_size + k*element_size + ch;
                    prediction_single_ch[count] = prediction_data[position];
                    count++;
                }
            }
        }

        double* zp_prediction_single_ch = NULL;
        padZero(prediction_single_ch,dims,pad_len,&zp_prediction_single_ch);
//        if (ch == 0)
//            memcpy(prediction_ch0, prediction_single_ch, depth*height*width*sizeof(double));


        stackp->add_prediction_channel(zp_prediction_single_ch);
    }

    stackp->set_basic_features();

    cout<<"Building RAG ...";
    stackp->build_rag();
    cout<<"done with "<< stackp->get_num_bodies()<< " regions\n";
    cout<<"Inclusion removal ...";
    stackp->remove_inclusions();
    cout<<"done with "<< stackp->get_num_bodies()<< " regions\n";

    all_region_ids.clear();
    for (NeuroProof::Rag<uint>::nodes_iterator iter = stackp->get_rag()->nodes_begin(); iter != stackp->get_rag()->nodes_end(); ++iter) {
        uint id1 = (*iter)->get_node_id();
	all_region_ids.push_back(id1);
	
    }
      
    
    EdgeClassifier* eclfr;
    if (endswith(classifier_filename, ".h5")){
        std::string nameonly = classifier_filename.substr(0, classifier_filename.find_last_of("."));
        //if (nameonly.find("parallel") != std::string::npos)
        //    eclfr = new VigraRFclassifierP(classifier_filename.c_str());
        //else
            eclfr = new VigraRFclassifier(classifier_filename.c_str());
    }
//     	eclfr = new VigraRFclassifier(classifier_filename.c_str());
    else if (endswith(classifier_filename, ".xml")){
        std::string nameonly = classifier_filename.substr(0, classifier_filename.find_last_of("."));
        //if (nameonly.find("parallel") != std::string::npos)
        //    eclfr = new OpencvRFclassifierP(classifier_filename.c_str());
        //else
            eclfr = new OpencvRFclassifier(classifier_filename.c_str());
    }
        //eclfr = new CompositeRFclassifier(classifier_filename.c_str());
// 	eclfr = new OpencvRFclassifier(classifier_filename.c_str());

    stackp->get_feature_mgr()->set_classifier(eclfr);


//    if (prediction_data)
//        delete[] prediction_data;
    delete[] prediction_single_ch;
    delete[] prediction_ch0;
}

std::vector<uint>& SegmentationInterface::get_all_region_ids(){
    return all_region_ids;
}

void SegmentationInterface::find_search_region(LeafStruct& endpt,
                                               double& minz, double& maxz,
                                               double& miny, double& maxy,
                                               double& minx, double& maxx)
{
    int search_width=50;

    minz = (endpt.nodez - search_width) < 0? 0: (endpt.nodez - search_width);
    miny = (endpt.nodey - search_width) < 0? 0: (endpt.nodey - search_width);
    minx = (endpt.nodex - search_width) < 0? 0: (endpt.nodex - search_width);

    maxz = (endpt.nodez + search_width) > depth ? depth: (endpt.nodez + search_width);
    maxy = (endpt.nodey + search_width) > height ? height: (endpt.nodey + search_width);
    maxx = (endpt.nodex + search_width) > height ? height: (endpt.nodex + search_width);
}

std::vector<uint>& SegmentationInterface::findAdjacentRegions(LeafStruct& endpt){
    double search_minz, search_maxz, search_miny, search_maxy, search_minx, search_maxx;

    uint current_id = segmentation_data[(uint)(endpt.nodez*(height*width) + endpt.nodey*width + endpt.nodex)];
    find_search_region(endpt, search_minz, search_maxz, search_miny, search_maxy, search_minx, search_maxx);

    std::set<uint> unique_ids;

    size_t plane_size = height*width;
    for(size_t z = search_minz; z < search_maxz; z++){
        for(size_t y = search_miny; y < search_maxy; y++){
            for(size_t x = search_minx; x < search_maxx; x++){
                size_t position = z*plane_size + y*width + x;

                unique_ids.insert(segmentation_data[position]);

            }
        }
    }

    std::set<uint> rag_nbrs;
    NeuroProof::RagNode<Label>* current_node = stackp->get_rag()->find_rag_node(current_id);
    typename NeuroProof::RagNode<Label>::edge_iterator iter = current_node->edge_begin();
    for(; iter != current_node->edge_end(); ++iter) {
        NeuroProof::RagNode<Label>* other_node = (*iter)->get_other_node(current_node);
        rag_nbrs.insert(other_node->get_node_id());
    }

    nearby_regions.clear();

    std::set<uint>::iterator sit = unique_ids.begin();
    for(; sit != unique_ids.end(); sit++){
        uint region_id = (*sit);
        if (rag_nbrs.find(region_id) != rag_nbrs.end())
            nearby_regions.push_back((*sit));
    }

    return nearby_regions;
}

