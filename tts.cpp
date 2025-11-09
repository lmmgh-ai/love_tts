// snpccfg.cc
#include "SynthesizerTrn.h"
#include "utils.h"
#include "summer_tts.h"

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <stdio.h>
#include <errno.h>
}

#ifdef _WIN32
#define LOVE_OR_LUA_LIB __declspec(dllexport)
#else
#define LOVE_OR_LUA_LIB
#endif
// typedef void (*Fun)(int, int);
//  extern "C" LUA_API int luaopen_libluatts(lua_State* L);//定义导出函数
/*
static int tts(lua_State* L)
{
    const char* txt = luaL_checkstring(L, 1);
    const char* cn = luaL_checkstring(L, 2);
    const char* en = luaL_checkstring(L, 3);
    const char* path = luaL_checkstring(L, 4);
    summer_tts tts;
    vector<retStruct> data = tts.toSong(txt, cn, en, path);
    lua_pushlightuserdata(L, &data);
    return 1;
}*/
// ai定义的结构体
typedef struct
{
    short *data; // PCM audio data
    int length;  // number of samples
} AudioChunk;

extern "C"
{
    LOVE_OR_LUA_LIB int fun()
    {
        printf("hello ffi");
        return 0;
    };

    LOVE_OR_LUA_LIB int tts(char *txt, char *cn, char *en, char *path)
    {

        summer_tts tts;
        vector<retStruct> data = tts.toSong(txt, cn, en, path);
        // lua_pushlightuserdata(L, &data);
        return 1;
    }
    LOVE_OR_LUA_LIB int ttsText(char *txt, SynthesizerTrn *cn, SynthesizerTrn *en, FILE *file)
    {

        summer_tts tts;
        int data = tts.ttsText(txt, cn, en, file);
        //  lua_pushlightuserdata(L, &data);
        return data;
    }
    LOVE_OR_LUA_LIB SynthesizerTrn *getCN(float *vec, int vecsize)
    {
        // ModelData cnModel(path);
        SynthesizerTrn *synthesizerCN = new SynthesizerTrn(vec, vecsize);
        return synthesizerCN;
    }
    LOVE_OR_LUA_LIB SynthesizerTrn *getEN(float *vec, int vecsize)
    {
        // ModelData cnModel(path);
        SynthesizerTrn *synthesizerCN = new SynthesizerTrn(vec, vecsize);
        return synthesizerCN;
    }
    LOVE_OR_LUA_LIB SynthesizerTrn *getlocalCN(char *path)
    {
        ModelData cnModel(path);
        SynthesizerTrn *synthesizerCN = new SynthesizerTrn(cnModel.get(), cnModel.size);
        return synthesizerCN;
    }
    LOVE_OR_LUA_LIB SynthesizerTrn *getlocalEN(char *path)
    {
        ModelData enModel(path);
        SynthesizerTrn *synthesizerEN = new SynthesizerTrn(enModel.get(), enModel.size); // 英文
        return synthesizerEN;
    }
    LOVE_OR_LUA_LIB int16_t *getCNtxt(SynthesizerTrn *cnsy, char *txt)
    {
        string text(txt);
        int len = text.length();
        int16_t *data = cnsy->infer(text, 0, 1.0, len);
        return data;
    }

    LOVE_OR_LUA_LIB int16_t *getENtxt(SynthesizerTrn *ensy, char *txt)
    {
        string text(txt);
        int len = text.length();
        int16_t *data = ensy->infer(text, 0, 1.0, len);
        return data;
    }

    LOVE_OR_LUA_LIB int16_t getDataf(int16_t *data, int i)
    {
        return *(data + 1);
    }

    LOVE_OR_LUA_LIB vector<retStruct> *getTextList(char *txt, SynthesizerTrn *cnsy, SynthesizerTrn *ensy)
    {
        TextSet textSet(txt, 1);
        vector<retStruct> *retList = new vector<retStruct>{};
        int32_t totalLen = 0;

        for (auto &t : textSet.textList)
        {
            retStruct ret;
            if (t.isCN)
                ret.wavData = cnsy->infer(t.text, 0, 1.0, ret.len);
            else
                ret.wavData = ensy->infer(t.text, 0, 1.0, ret.len);
            // cout << "\nnumber data" << ret.wavData;
            int32_t i = 0;

            while (i < ret.len && abs(ret.wavData[i]) < 50)
                i++;

            ret.startIdx = i;
            i = ret.len - 1;

            while (i > 0 && abs(ret.wavData[i]) < 50)
                i--;

            ret.endIdx = i;

            // 保留有效语音片段，尽量去除头尾空白音频
            if (ret.endIdx > ret.startIdx)
            {
                totalLen += ret.endIdx - ret.startIdx;
                retList->emplace_back(ret);
            }
        }
        return retList;
    };

    LOVE_OR_LUA_LIB int getListLen(vector<retStruct> *list)
    {
        return list->size(); // 返回数据列表长度
    };
    LOVE_OR_LUA_LIB retStruct *getListitem(vector<retStruct> *list, int i)
    {
        return &(list->at(i)); // 返回某个元素
    };
    LOVE_OR_LUA_LIB int lovestat(char *path)
    {
        struct stat st;

        int f = stat(path, &st);
        // strerror(errno);
        int s = st.st_size;
        return s;
    };
    LOVE_OR_LUA_LIB int loveopenstat(char *path)
    {
        auto p = fopen(path, "rb");
        if (!p)
            return 0;
        else
            return 11;
    };
    LOVE_OR_LUA_LIB char *geterron()
    {
        return strerror(errno);
    };
    LOVE_OR_LUA_LIB vector<float> *getVectorf(int size)
    {
        vector<float> *dataBuff = new vector<float>;
        dataBuff->reserve(size / 4 + (size % 4 == 0 ? 0 : 1));
        return dataBuff;
    };
    LOVE_OR_LUA_LIB float *getVectordata(vector<float> *v)
    {
        return v->data();
    };

    LOVE_OR_LUA_LIB int loveFUn(int (*Fun)(int, int), int a, int b)
    {
        return Fun(a, b);
    };
    // ai定义的函数
    LOVE_OR_LUA_LIB void *init_model_cn(const char *path)
    {
        ModelData *md = new ModelData(path);
        return new SynthesizerTrn(md->get(), md->size);
    };

    LOVE_OR_LUA_LIB void *init_model_en(const char *path)
    {
        ModelData *md = new ModelData(path);
        return new SynthesizerTrn(md->get(), md->size);
    };

    LOVE_OR_LUA_LIB AudioChunk *synthesize_text(void *cn_model, void *en_model, const char *text)
    {
        SynthesizerTrn *cn = static_cast<SynthesizerTrn *>(cn_model);
        SynthesizerTrn *en = static_cast<SynthesizerTrn *>(en_model);

        bool isCN = true; // 假设为中文，实际应判断语言
        int len;
        short *wavData = cn->infer(text, 0, 1.0f, len);

        AudioChunk *chunk = new AudioChunk();
        chunk->data = wavData;
        chunk->length = len;

        return chunk;
    };

    LOVE_OR_LUA_LIB void free_audio_chunk(AudioChunk *chunk)
    {
        delete[] chunk->data;
        delete chunk;
    };

    LOVE_OR_LUA_LIB void destroy_model(void *model)
    {
        delete static_cast<SynthesizerTrn *>(model);
    };
}

static int tts(lua_State *L)
{
    // cout << "hello luatts";
    return 0;
}
// part one: 要导出的函数列表
static const struct luaL_Reg luaLibs[] = {
    {"tts", tts},
    {NULL, NULL}};

// part two: DLL入口函数，Lua调用此DLL的入口函数.
extern "C"
{
    LOVE_OR_LUA_LIB int luaopen_libluatts(lua_State *L)
    {
        luaL_register(L, "liluatts", luaLibs);
        return 1; // 把myLib表压入了栈中，所以就需要返回1
    }
};