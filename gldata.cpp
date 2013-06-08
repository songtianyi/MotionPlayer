#include "gldata.h"

GLData::GLData()
{
    init();
}
void GLData::init()
{
    //data
    objectName = "";
    parent_of   = NULL;
    data        = NULL;

    //play state
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
    int len = strlen(dir);
    for(int i = 0;i < len;i++)
    {
        if( dir[i] == '?')
        {
            return ;
        }
    }
    if(valid == true)
    {
        destroy();
        init();
    }
    //ASE file
    if( suffix == "ASE" || suffix == "BZ")
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
        data = new CVector3f[actHead->m_boneAnmCount*nClipNum];
        for(int i = 0;i < nClipNum;i++)
        {
            for(int j = 0;j < actHead->m_boneAnmCount;j++)
            {
                int index = i*(actHead->m_boneAnmCount) + (j);
                data[index].x = glpos[j*nClipNum + i].x;
                data[index].y = glpos[j*nClipNum + i].y;
                data[index].z = glpos[j*nClipNum + i].z;
            }
        }


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
        //parse bvh file
        CBVHParser bp;
        HBVHHead pHead;
        HBVHJoint bjoint[BVH_MAX_JOINT];
        pHead.alloc();
        bp.getBVHHeader(dir,&pHead,bjoint);
        CVector3f *datamat = new CVector3f[pHead.m_columNum/3*pHead.m_frameNum];
        bp.parse(dir,&pHead,bjoint,datamat);

        //calculate 3d coord
        GLPOS gl;
        data = new CVector3f[pHead.m_jointNum*pHead.m_frameNum];
        parent_of = new int[pHead.m_jointNum];
        memcpy(parent_of,pHead.m_parentOf,sizeof(int)*pHead.m_jointNum);
        frameNum = pHead.m_frameNum;
        boneNum = pHead.m_jointNum;

        gl.getGLPos(data,&pHead ,bjoint,datamat);
        float scale = 0.125;
        for(int i = 0;i < pHead.m_frameNum;i++)
        {
            int index = i*pHead.m_jointNum;
            for(int j = 0;j < pHead.m_jointNum;j++)
            {
                CVector3f t(data[index + j].x,data[index + j].z,data[index + j].y);
                data[index+j] = t * scale;
            }
        }

        delete [] datamat; datamat = 0;
        pHead.dealloc();
        valid = true;
        pause = false;
    }
    else if(suffix == "CAF")
    {
        HCordAnmHeader *h = new HCordAnmHeader;
        CordAnm *ca = new CordAnm;
        ca->getCordAnmHeader(dir,h);
        parent_of = new int[h->m_boneNum];
        data = new CVector3f[h->m_boneNum*h->m_frameNum];
        ca->parse(dir,h,parent_of,data);
        frameNum = h->m_frameNum;
        boneNum = h->m_boneNum;
        objectName = h->m_name;
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

        th.alloc();
        data = new CVector3f[th.m_numFrames*th.m_numMarkers];
        trcp.parse(dir,&th,data);
        for(int i = 0;i < th.m_numFrames;i++)
        {
            for(int j = 0;j < th.m_numMarkers;j++)
            {
                float factor = 1.0f/400;
                int index = i*(th.m_numMarkers) + (j);
                CVector3f t(data[index].z * factor,\
                            data[index].x * factor,\
                            data[index].y * factor);
                data[index] = t;
            }
        }
        //export to caf
        HCordAnmHeader *cah = new HCordAnmHeader;
        cah->m_boneNum = boneNum;
        cah->m_frameNum = frameNum;
        strcpy(cah->m_name,"song");
        memset(parent_of,-1,sizeof(int)*cah->m_boneNum);

        CordAnm *ca = new CordAnm;
        ca->restore("t.caf",cah,parent_of,data);
        delete ca; ca = NULL;
        delete cah; cah = 0;

        th.dealloc();

        valid = true;
        pause = false;
    }


}
