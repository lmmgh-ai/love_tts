#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#include <fcntl.h>
#include <unistd.h>

#include "SynthesizerTrn.h"
#include "utils.h"

#include "summer_tts.h"

#include <time.h>

using std::cerr;
using std::cout;
using std::endl;
using std::vector;

summer_tts::summer_tts(){

};

summer_tts::summer_tts(char *cn, char *en)
{
    setvbuf(stdout, NULL, _IONBF, 0); // 禁用输出缓冲
    ModelData cnModel(cn);
    ModelData enModel(en);
    SynthesizerTrn *sCN = new SynthesizerTrn(cnModel.get(), cnModel.size); // 中文
    SynthesizerTrn *sEN = new SynthesizerTrn(enModel.get(), enModel.size); // 英文
    synthesizerCN = sCN;
    synthesizerEN = sEN;
    // SynthesizerTrn synthesizerCN = SynthesizerTrn(cnModel.get(), cnModel.size); // 中文
    // SynthesizerTrn synthesizerEN = SynthesizerTrn(enModel.get(), enModel.size); // 英文
};

summer_tts::~summer_tts()
{
}

void summer_tts::loadTxt(const char *txt)
{
    textSet = TextSet(txt, 1);
}

vector<retStruct> summer_tts::toSong(const char *txt, const char *cn, const char *en, const char *path)
// TextSet  textSet(txt, 1);
{
    cout << "Loading text... [" << txt;
    // TextSet textSet(argv[1]);
    TextSet textSet(txt, 1);
    cout << "] Done.\n";

    cout << "loading cn en.\n";
    ModelData cnModel(cn);
    ModelData enModel(en);
    cout << "Done.\n";
    SynthesizerTrn synthesizerCN(cnModel.get(), cnModel.size); // 中文
    SynthesizerTrn synthesizerEN(enModel.get(), enModel.size); // 英文

    retList = {};
    cnt = 0, totalLen = 0;
    cout << "Loading text... [" << textSet.textList.size() << "]";
    for (auto &t : textSet.textList)
    {
        retStruct ret;
        cout << " for.\n";
        if (t.isCN)
        {
            ret.wavData = synthesizerCN.infer(t.text, 0, 1.0, ret.len);
        }
        else
        {
            ret.wavData = synthesizerEN.infer(t.text, 0, 1.0, ret.len);
        }
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
            retList.emplace_back(ret);
        }

        cnt++;
        int percent = (cnt * 50 / textSet.textList.size());
        cout << "\rInferring [";
        for (int i = 0; i < 50; i++)
            cout << (i < percent ? '>' : '-');
        cout << "] [" << cnt << '/' << textSet.textList.size() << ']';
    }
    FILE *fpOut = fopen(path, "wb");
    summer_tts::saveToWavFile(fpOut, retList, totalLen * sizeof(int16_t));

    return retList;
}

int summer_tts::ttsText(char *txt, SynthesizerTrn *cn, SynthesizerTrn *en, FILE *file)
{
    cout << "Loading text... [" << txt;
    // TextSet textSet(argv[1]);
    TextSet textSet(txt, 1);
    cout << "] Done.\n";
    /*
        cout << "loading cn en.\n";
        ModelData cnModel(cn);
        ModelData enModel(en);
        cout << "Done.\n";
        SynthesizerTrn synthesizerCN(cnModel.get(), cnModel.size); // 中文
        SynthesizerTrn synthesizerEN(enModel.get(), enModel.size); // 英文
    */
    vector<retStruct> retList;
    int32_t cnt = 0, totalLen = 0;
    cout << "Loading text... [" << textSet.textList.size() << "]";
    for (auto &t : textSet.textList)
    {
        retStruct ret;
        cout << " for.\n";
        if (t.isCN)
        {
            ret.wavData = cn->infer(t.text, 0, 1.0, ret.len);
        }
        else
        {
            ret.wavData = en->infer(t.text, 0, 1.0, ret.len);
        }
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
            retList.emplace_back(ret);
        }

        cnt++;
        int percent = (cnt * 50 / textSet.textList.size());
        cout << "\rInferring [";
        for (int i = 0; i < 50; i++)
            cout << (i < percent ? '>' : '-');
        cout << "] [" << cnt << '/' << textSet.textList.size() << ']';
    }

    summer_tts::saveToWavFile(file, retList, totalLen * sizeof(int16_t));

    return 1;
}