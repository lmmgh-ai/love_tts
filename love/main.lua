require("fun")
local ffi = require("ffi")

-- 定义结构体与函数签名
ffi.cdef [[
int tts(char *txt, char *cn, char *en, char *path);

//int ttsText(char *txt, SynthesizerTrn *cn, SynthesizerTrn *en, FILE *file);

void* getCN(float *vec, int vecsize);//获取中文ai模型对象
void* getlocalCN(char* path);//获取英文ai模型对象
int16_t* getCNtxt(void*,char *txt);//纯中文转文字数据
void* getEN(float *vec, int vecsize);//获取英文ai模型对象
void* getlocalEN(char* path);//获取英文ai模型对象
int16_t* getENtxt(void*,char *txt);//纯英文转文字数据
float getDataf(int16_t *data,int i);//转换数据

void* getTextList(char *txt, void *cnsy, void *ensy);//获取数据数组
int getListLen(void *list);//获取数组大小
void* getListitem(void *list, int i);//获取数组某个元素

int lovestat(char *path);//文件信息
int loveopenstat(char *path);//文件打开成功
char *geterron();//获取错误信息

void* getVectorf(int size);//获取数组内存地址
float *getVectordata(void *vector);//获取写入内存地址


int loveFUn(int (*Fun)(int, int),int a,int b);

    //ai
    typedef struct {
        short* data;
        int length;
    } AudioChunk;

    void* init_model_cn(const char* path);
    void* init_model_en(const char* path);
    AudioChunk* synthesize_text(void* cn_model, void* en_model, const char* text);
    void free_audio_chunk(AudioChunk* chunk);
    void destroy_model(void* model);
]]

-- 加载动态链接库
local tts = ffi.load("libluatts") -- 注意路径正确性

local cn = char(ffi, "./models/single_speaker_fast.bin")
local en = char(ffi, "./models/single_speaker_english_fast.bin")
-- 初始化模型
local cn_model = tts.init_model_cn(cn)
local en_model = tts.init_model_en(en)



-- 合成语音
local text = "你好世界123"
local chunk = tts.synthesize_text(cn_model, en_model, char(ffi, text))



-- 将 PCM 数据复制到 Lua 表格中以便后续处理
local pcm_data = {}
for i = 0, chunk.length - 1 do
    table.insert(pcm_data, chunk.data[i])
end

-- 打印长度测试是否成功获取数据
print("Got", #pcm_data, "samples")

-- 可选：释放内存
--tts.free_audio_chunk(chunk)
--tts.destroy_model(cn_model)
--tts.destroy_model(en_model)


-- 假设采样率是 22050Hz、单声道、16-bit signed integer
local sampleRate = 16000 --22050
local channels = 1
local bitDepth = 16
local soundData = love.sound.newSoundData(#pcm_data, sampleRate, bitDepth, channels)

for i = 1, #pcm_data do
    soundData:setSample(i - 1, pcm_data[i] / 32768) -- 归一化 [-1, 1]
end

-- 创建 Source 并播放
local source = love.audio.newSource(soundData)
source:play()




function love.mousemoved(x, y, dx, dy)
    -- 记录开始时间
    local starttime = os.clock()
    print(string.format("start time : %.4f", starttime))
    -- 可以在这里添加其他逻辑
    local chunk = tts.synthesize_text(cn_model, en_model, char(ffi, text))
    -- 将 PCM 数据复制到 Lua 表格中以便后续处理
    local pcm_data = {}
    for i = 0, chunk.length - 1 do
        table.insert(pcm_data, chunk.data[i])
    end
    -- 假设采样率是 22050Hz、单声道、16-bit signed integer
    local sampleRate = 16000 --22050
    local channels = 1
    local bitDepth = 16
    local soundData = love.sound.newSoundData(#pcm_data, sampleRate, bitDepth, channels)

    for i = 1, #pcm_data do
        soundData:setSample(i - 1, pcm_data[i] / 32768) -- 归一化 [-1, 1]
    end

    -- 创建 Source 并播放
    local source = love.audio.newSource(soundData)
    source:play()
    -- 记录结束时间
    local endtime = os.clock()
    print(string.format("end time : %.4f", endtime))
    print(string.format("cost time : %.4f", endtime - starttime))
end
