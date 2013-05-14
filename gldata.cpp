#include "gldata.h"

GLData::GLData()
{
    init();
}
void GLData::init()
{
    //data
    name = "";
    parent_of   = NULL;
    data        = NULL;

    //play state
    DRAW_CURVE = true;
    curr = 0;
    pause = true;
    valid = false;
    root_offset[0] = root_offset[1] = root_offset[2] = 0.0f;


}
void GLData::destroy()
{
    if( parent_of != NULL )
    {
        delete [] parent_of; parent_of = NULL;
    }

    if( data != NULL )
    {
        delete [] data; data = NULL;
    }
}
GLData::~GLData()
{
    destroy();
}
void GLData::process(const char *dir,QString suffix)
{
    if(valid == true)
    {
        destroy();
        init();
    }
    //ASE file
    if( suffix == "ASE")
    {
        CASEParser *ap = new CASEParser;
        ActionHeader *actHead = new ActionHeader;
        ap->getActionHeader(dir,actHead);
        assert(actHead->m_boneAnmCount+actHead->m_helpAnmCount < 100);
        AnmHeader *anmHead = new AnmHeader[actHead->m_boneAnmCount];
        HAnmHeader *hanmHead = 0;
        hanmHead = new HAnmHeader[actHead->m_helpAnmCount];
        int nClipNum = actHead->m_lastFrame-actHead->m_firstFrame+1;
        CQuaternion *rotkey = new CQuaternion[actHead->m_boneRotCount*nClipNum];
        CAAP *aapkey = new CAAP[actHead->m_boneRotCount*nClipNum];
        CQuaternion *hrotkey = 0;
        CAAP *haapkey = 0;
        hrotkey = new CQuaternion[actHead->m_helpRotCount*nClipNum];
        haapkey = new CAAP[actHead->m_helpRotCount*nClipNum];
        CVector3f *poskey = new CVector3f[actHead->m_bonePosCount*nClipNum];
        CVector3f *hposkey = 0;
        hposkey = new CVector3f[actHead->m_helpPosCount*nClipNum];
        ap->parse(dir,poskey,hposkey,rotkey,hrotkey,aapkey,haapkey,anmHead,hanmHead,actHead);

        //
        for(int i = 0;i < actHead->m_boneRotCount;i++)
        {
            int index = i*nClipNum;
            for(int j = 0;j < nClipNum;j++)
            {
                if(rotkey[index+j].w < 0)
                {
                    rotkey[index+j] = -rotkey[index+j];
                }
            }
        }

        //
        for(int i = 1;i < nClipNum;i++)
        {
            for(int j = 0;j < actHead->m_boneRotCount;j++)
            {
                rotkey[j*nClipNum+i] = rotkey[j*nClipNum+i-1]* rotkey[j*nClipNum+i];
                rotkey[j*nClipNum+i].normalize();
            }
        }
        GLPOS *gl = new GLPOS;
        CVector3f *glpos = new CVector3f[actHead->m_boneAnmCount*nClipNum];
        gl->getGLPos(glpos,actHead,anmHead,rotkey,poskey);


        //copy value
        frameNum = nClipNum;
        boneNum = actHead->m_boneAnmCount;
        parent_of = new int[boneNum];
        gl->__getParentNodesID(parent_of,actHead,anmHead);
        data = new float[actHead->m_boneAnmCount*nClipNum*3];
        for(int i = 0;i < nClipNum;i++)
        {
            for(int j = 0;j < actHead->m_boneAnmCount;j++)
            {
                int index = i*(actHead->m_boneAnmCount*3) + (j*3);
                data[index] = glpos[j*nClipNum + i].x;
                data[index+1] = glpos[j*nClipNum + i].y;
                data[index+2] = glpos[j*nClipNum + i].z;
            }
        }
        HCordAnmHeader *cah = new HCordAnmHeader;
        cah->m_boneNum = boneNum;
        cah->m_frameNum = frameNum;
        strcpy(cah->m_name,"song");

        CordAnm *ca = new CordAnm;
        ca->restore("t.caf",cah,parent_of,data);

        delete ca; ca = NULL;
        delete cah; cah = 0;

        //free memory
        delete [] glpos; glpos = NULL;
        delete gl; gl = NULL;

        delete actHead; actHead = 0;
        delete [] anmHead; anmHead = 0;
        delete [] rotkey; rotkey = 0;
        delete [] aapkey; aapkey = 0;
        delete [] poskey; poskey = 0;
        delete [] hanmHead; hanmHead = 0;
        delete [] hrotkey; hrotkey = 0;
        delete [] haapkey; haapkey = 0;
        delete [] hposkey; hposkey = 0;
        delete ap; ap = 0;

        valid = true;
        pause = false;
    }

    else if( suffix == "BVH" )
    {
        //valid = true;
        //pause = false;
    }
    else if(suffix == "CAF")
    {
        HCordAnmHeader *h = new HCordAnmHeader;
        CordAnm *ca = new CordAnm;
        ca->getCordAnmHeader(dir,h);
        parent_of = new int[h->m_boneNum];
        data = new float[h->m_boneNum*h->m_frameNum*3];
        ca->parse(dir,h,parent_of,data);
        frameNum = h->m_frameNum;
        boneNum = h->m_boneNum;
        name = h->m_name;
        delete ca; ca = NULL;
        delete h; h = NULL;

        valid = true;
        pause = false;
    }
    else if( suffix == "TRC")
    {
        HTRCHeader th;
        CTRCParser trcp;
        trcp.getTRCHeader(dir,&th);
        parent_of = new int[th.m_numMarkers];
        memset(parent_of,-1,sizeof(int)*th.m_numMarkers);

        frameNum = th.m_numFrames;
        boneNum = th.m_numMarkers;
        name = "TRC";
        th.alloc();
        double *mat = new double[th.m_numFrames*th.m_numMarkers*3];
        trcp.parse(dir,&th,mat);
        data = new float[th.m_numFrames*th.m_numMarkers*3];
        for(int i = 0;i < th.m_numFrames;i++)
        {
            for(int j = 0;j < th.m_numMarkers;j++)
            {
                float factor = 1.0f/400;
                int index = i*(th.m_numMarkers*3) + (j*3);
                data[index] = mat[index + 2]* factor;
                data[index+1] = mat[index+0] * factor;
                data[index+2] = mat[index+1] * factor;
            }
        }
        th.dealloc();
        delete [] mat; mat = 0;

        HCordAnmHeader *cah = new HCordAnmHeader;
        cah->m_boneNum = boneNum;
        cah->m_frameNum = frameNum;
        strcpy(cah->m_name,"song");

        CordAnm *ca = new CordAnm;
        ca->restore("t.caf",cah,parent_of,data);

        delete ca; ca = NULL;
        delete cah; cah = 0;

        valid = true;
        pause = false;
    }


}
