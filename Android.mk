LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
# 模块名称（不能包含空格）
LOCAL_MODULE := lutts 
# 输出路径
LOCAL_MODULE_PATH := $(LOCAL_PATH)/../build/$(TARGET_ARCH_ABI)
# 指定目标ABI
APP_ABI := armeabi-v7a arm64-v8a x86 x86_64
# 源文件列表
LOCAL_SRC_FILES :=${LOCAL_PATH}/tts.cpp\
               ${LOCAL_PATH}/test/summer_tts.cpp\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/compat.cc\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/flags.cc\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/fst.cc\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/fst-types.cc\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/mapped-file.cc\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/properties.cc\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/symbol-table.cc\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/symbol-table-ops.cc\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/util.cc\
               ${LOCAL_PATH}/src/tn/openfst/src/lib/weight.cc\
               ${LOCAL_PATH}/src/tn/processor.cc\
               ${LOCAL_PATH}/src/tn/token_parser.cc\
               ${LOCAL_PATH}/src/tn/utf8_string.cc\
               ${LOCAL_PATH}/src/engipa/EnglishText2Id.cpp\
               ${LOCAL_PATH}/src/engipa/InitIPASymbols.cpp\
               ${LOCAL_PATH}/src/engipa/alphabet.cpp\
               ${LOCAL_PATH}/src/engipa/ipa.cpp\
               ${LOCAL_PATH}/src/hz2py/hanzi2phoneid.cpp\
               ${LOCAL_PATH}/src/hz2py/Hanz2Piny.cpp\
               ${LOCAL_PATH}/src/hz2py/pinyinmap.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_conv1d.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_softmax.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_layer_norm.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_relu.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_gelu.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_tanh.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_flip.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_cumsum.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_softplus.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_clamp_min.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_sigmoid.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_conv1d_transposed.cpp\
               ${LOCAL_PATH}/src/nn_op/nn_leaky_relu.cpp\
               ${LOCAL_PATH}/src/platform/tts_file_io.cpp\
               ${LOCAL_PATH}/src/platform/tts_logger.cpp\
               ${LOCAL_PATH}/src/utils/utils.cpp\
               ${LOCAL_PATH}/src/modules/iStft.cpp\
               ${LOCAL_PATH}/src/modules/hann.cpp\
               ${LOCAL_PATH}/src/modules/attention_encoder.cpp\
               ${LOCAL_PATH}/src/modules/multi_head_attention.cpp\
               ${LOCAL_PATH}/src/modules/ffn.cpp\
               ${LOCAL_PATH}/src/modules/ConvFlow.cpp\
               ${LOCAL_PATH}/src/modules/DDSConv.cpp\
               ${LOCAL_PATH}/src/modules/ElementwiseAffine.cpp\
               ${LOCAL_PATH}/src/modules/random_gen.cpp\
               ${LOCAL_PATH}/src/modules/ResidualCouplingLayer.cpp\
               ${LOCAL_PATH}/src/modules/ResBlock1.cpp\
               ${LOCAL_PATH}/src/modules/WN.cpp\
               ${LOCAL_PATH}/src/modules/pqmf.cpp\
               ${LOCAL_PATH}/src/models/TextEncoder.cpp\
               ${LOCAL_PATH}/src/models/StochasticDurationPredictor.cpp\
               ${LOCAL_PATH}/src/models/FixDurationPredictor.cpp\
               ${LOCAL_PATH}/src/models/DurationPredictor_base.cpp\
               ${LOCAL_PATH}/src/models/ResidualCouplingBlock.cpp\
               ${LOCAL_PATH}/src/models/Generator_base.cpp\
               ${LOCAL_PATH}/src/models/Generator_hifigan.cpp\
               ${LOCAL_PATH}/src/models/Generator_MS.cpp\
               ${LOCAL_PATH}/src/models/Generator_Istft.cpp\
               ${LOCAL_PATH}/src/models/Generator_MBB.cpp\
               ${LOCAL_PATH}/src/models/SynthesizerTrn.cpp

# C编译器标志 
LOCAL_LDFLAGS := -L./ 
# 静态库依赖 
LOCAL_STATIC_LIBRARIES :=-L./lib/ -l./
# 链接库 (系统)
LOCAL_LDLIBS := -llog -landroid
APP_ALLOW_MISSING_DEPS=true#允许丢失依赖
# 包含的头文件路径 
LOCAL_C_INCLUDES := ${LOCAL_PATH}/eigen-3.4.0\
                    ${LOCAL_PATH}/src/tn/header\
                    ${LOCAL_PATH}/include\
					${LOCAL_PATH}/src/header

include $(BUILD_SHARED_LIBRARY)
